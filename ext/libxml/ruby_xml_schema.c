#include "ruby_libxml.h"
#include "ruby_xml_schema.h"

#include "ruby_xml_schema_type.h"
#include "ruby_xml_schema_element.h"
#include "ruby_xml_schema_attribute.h"
#include "ruby_xml_schema_facet.h"

#include <libxml/xmlschemas.h>

typedef struct _xmlSchemaBucket xmlSchemaBucket;
typedef xmlSchemaBucket *xmlSchemaBucketPtr;

/**
 * xmlSchemaSchemaRelation:
 *
 * Used to create a graph of schema relationships.
 */
typedef struct _xmlSchemaSchemaRelation xmlSchemaSchemaRelation;
typedef xmlSchemaSchemaRelation *xmlSchemaSchemaRelationPtr;
struct _xmlSchemaSchemaRelation {
    xmlSchemaSchemaRelationPtr next;
    int type;
    /* E.g. XML_SCHEMA_SCHEMA_IMPORT */
    const xmlChar *importNamespace;
    xmlSchemaBucketPtr bucket;
};

struct _xmlSchemaBucket {
    int type;
    int flags;
    const xmlChar *schemaLocation;
    const xmlChar *origTargetNamespace;
    const xmlChar *targetNamespace;
    xmlDocPtr doc;
    xmlSchemaSchemaRelationPtr relations;
    int located;
    int parsed;
    int imported;
    int preserveDoc;
    xmlSchemaItemListPtr globals;
    /* Global components. */
    xmlSchemaItemListPtr locals; /* Local components. */
};

/**
 * xmlSchemaImport:
 * (extends xmlSchemaBucket)
 *
 * Reflects a schema. Holds some information
 * about the schema and its toplevel components. Duplicate
 * toplevel components are not checked at this level.
 */
typedef struct _xmlSchemaImport xmlSchemaImport;
typedef xmlSchemaImport *xmlSchemaImportPtr;
struct _xmlSchemaImport {
    int type;
    /* Main OR import OR include. */
    int flags;
    const xmlChar *schemaLocation; /* The URI of the schema document. */
    /* For chameleon includes, @origTargetNamespace will be NULL */
    const xmlChar *origTargetNamespace;
    /*
    * For chameleon includes, @targetNamespace will be the
    * targetNamespace of the including schema.
    */
    const xmlChar *targetNamespace;
    xmlDocPtr doc; /* The schema node-tree. */
    /* @relations will hold any included/imported/redefined schemas. */
    xmlSchemaSchemaRelationPtr relations;
    int located;
    int parsed;
    int imported;
    int preserveDoc;
    xmlSchemaItemListPtr globals;
    xmlSchemaItemListPtr locals;
    /* The imported schema. */
    xmlSchemaPtr schema;
};

/*
 * Document-class: LibXML::XML::Schema
 *
 * The XML::Schema class is used to prepare XML Schemas for validation of xml
 * documents.
 *
 * Schemas can be created from XML documents, strinings or URIs using the
 * corresponding methods (new for URIs).
 *
 * Once a schema is prepared, an XML document can be validated by the
 * XML::Document#validate_schema method providing the XML::Schema object
 * as parameter. The method return true if the document validates, false
 * otherwise.
 *
 * Basic usage:
 *
 *  # parse schema as xml document
 *  schema_document = XML::Document.file('schema.rng')
 *
 *  # prepare schema for validation
 *  schema = XML::Schema.document(schema_document)
 *
 *  # parse xml document to be validated
 *  instance = XML::Document.file('instance.xml')
 *
 *  # validate
 *  instance.validate_schema(schema)
 */

VALUE cXMLSchema;

static void rxml_schema_free(xmlSchemaPtr xschema)
{
  xmlSchemaFree(xschema);
}

VALUE rxml_wrap_schema(xmlSchemaPtr xschema)
{
  VALUE result;

  if (!xschema)
    rb_raise(rb_eArgError, "XML::Schema is required!");

  result = Data_Wrap_Struct(cXMLSchema, NULL, rxml_schema_free, xschema);

  /*
   * Create these as instance variables to provide the output of inspect/to_str some
   * idea of what schema this class contains.
   */
  rb_iv_set(result, "@target_namespace", QNIL_OR_STRING(xschema->targetNamespace));
  rb_iv_set(result, "@name", QNIL_OR_STRING(xschema->name));
  rb_iv_set(result, "@id", QNIL_OR_STRING(xschema->id));
  rb_iv_set(result, "@version", QNIL_OR_STRING(xschema->name));

  return result;
}

static VALUE rxml_schema_init(VALUE class, xmlSchemaParserCtxtPtr xparser)
{
  xmlSchemaPtr xschema;

  xschema = xmlSchemaParse(xparser);
  xmlSchemaFreeParserCtxt(xparser);

  if (!xschema)
    rxml_raise(xmlGetLastError());

  return rxml_wrap_schema(xschema);
}

/*
 * call-seq:
 *    XML::Schema.initialize(schema_uri) -> schema
 *
 * Create a new schema from the specified URI.
 */
static VALUE rxml_schema_init_from_uri(VALUE class, VALUE uri)
{
  xmlSchemaParserCtxtPtr xparser;

  Check_Type(uri, T_STRING);

  xmlResetLastError();
  xparser = xmlSchemaNewParserCtxt(StringValuePtr(uri));
  if (!xparser)
    rxml_raise(xmlGetLastError());

  return rxml_schema_init(class, xparser);
}

/*
 * call-seq:
 *    XML::Schema.document(document) -> schema
 *
 * Create a new schema from the specified document.
 */
static VALUE rxml_schema_init_from_document(VALUE class, VALUE document)
{
  xmlDocPtr xdoc;
  xmlSchemaParserCtxtPtr xparser;

  Data_Get_Struct(document, xmlDoc, xdoc);

  xmlResetLastError();
  xparser = xmlSchemaNewDocParserCtxt(xdoc);
  if (!xparser)
    rxml_raise(xmlGetLastError());

  return rxml_schema_init(class, xparser);
}

/*
 * call-seq:
 *    XML::Schema.from_string("schema_data") -> "value"
 *
 * Create a new schema using the specified string.
 */
static VALUE rxml_schema_init_from_string(VALUE class, VALUE schema_str)
{
  xmlSchemaParserCtxtPtr xparser;

  Check_Type(schema_str, T_STRING);

  xmlResetLastError();
  xparser = xmlSchemaNewMemParserCtxt(StringValuePtr(schema_str), (int)strlen(StringValuePtr(schema_str)));
  if (!xparser)
    rxml_raise(xmlGetLastError());

  return rxml_schema_init(class, xparser);
}

/*
 * call-seq:
 *    XML::Schema.document -> document
 *
 * Return the Schema XML Document
 */
static VALUE rxml_schema_document(VALUE self)
{
  xmlSchemaPtr xschema;

  Data_Get_Struct(self, xmlSchema, xschema);

  return rxml_node_wrap(xmlDocGetRootElement(xschema->doc));
}

static void scan_namespaces(xmlSchemaImportPtr ximport, VALUE array, const xmlChar *nsname)
{
  xmlNodePtr xnode;
  xmlNsPtr xns;

  if (ximport->doc)
  {
    xnode = xmlDocGetRootElement(ximport->doc);
    xns = xnode->nsDef;

    while (xns)
    {
      VALUE namespace = rxml_namespace_wrap(xns);
      rb_ary_push(array, namespace);
      xns = xns->next;
    }
  }
}

/*
 * call-seq:
 *    XML::Schema.namespaces -> array
 *
 * Returns an array of Namespaces defined by the schema
 */
static VALUE rxml_schema_namespaces(VALUE self)
{
  VALUE result;
  xmlSchemaPtr xschema;

  Data_Get_Struct(self, xmlSchema, xschema);

  result = rb_ary_new();
  xmlHashScan(xschema->schemasImports, (xmlHashScanner)scan_namespaces, (void *)result);

  return result;
}

static void scan_schema_element(xmlSchemaElementPtr xelement, VALUE hash, const xmlChar *name)
{
  VALUE element = rxml_wrap_schema_element(xelement);
  rb_hash_aset(hash, rb_str_new2((const char*)name), element);
}

static VALUE rxml_schema_elements(VALUE self)
{
  VALUE result = rb_hash_new();
  xmlSchemaPtr xschema;

  Data_Get_Struct(self, xmlSchema, xschema);
  xmlHashScan(xschema->elemDecl, (xmlHashScanner)scan_schema_element, (void *)result);

  return result;
}

static void collect_imported_ns_elements(xmlSchemaImportPtr import, VALUE result, const xmlChar *name)
{
  if (import->imported && import->schema)
  {
    VALUE elements = rb_hash_new();
    xmlHashScan(import->schema->elemDecl, (xmlHashScanner)scan_schema_element, (void *)elements);
    rb_hash_aset(result, QNIL_OR_STRING(import->schema->targetNamespace), elements);
  }
}

/*
 * call-seq:
 *    XML::Schema.imported_ns_elements -> hash
 *
 * Returns a hash by namespace of a hash of schema elements within the entire schema including imports
 */
static VALUE rxml_schema_imported_ns_elements(VALUE self)
{
  xmlSchemaPtr xschema;
  VALUE result = rb_hash_new();

  Data_Get_Struct(self, xmlSchema, xschema);

  if (xschema)
  {
    xmlHashScan(xschema->schemasImports, (xmlHashScanner)collect_imported_ns_elements, (void *)result);
  }

  return result;
}

static void scan_schema_type(xmlSchemaTypePtr xtype, VALUE hash, const xmlChar *name)
{
  VALUE type = rxml_wrap_schema_type(xtype);
  rb_hash_aset(hash, rb_str_new2((const char*)name), type);
}

static VALUE rxml_schema_types(VALUE self)
{
  VALUE result = rb_hash_new();
  xmlSchemaPtr xschema;

  Data_Get_Struct(self, xmlSchema, xschema);

  if (xschema != NULL && xschema->typeDecl != NULL)
  {
    xmlHashScan(xschema->typeDecl, (xmlHashScanner)scan_schema_type, (void *)result);
  }

  return result;
}

static void collect_imported_types(xmlSchemaImportPtr import, VALUE result, const xmlChar *name)
{
  if (import->imported && import->schema)
  {
    xmlHashScan(import->schema->typeDecl, (xmlHashScanner)scan_schema_type, (void *)result);
  }
}

/*
 * call-seq:
 *    XML::Schema.imported_types -> hash
 *
 * Returns a hash of all types within the entire schema including imports
 */
static VALUE rxml_schema_imported_types(VALUE self)
{
  xmlSchemaPtr xschema;
  VALUE result = rb_hash_new();

  Data_Get_Struct(self, xmlSchema, xschema);

  if (xschema)
  {
    xmlHashScan(xschema->schemasImports, (xmlHashScanner)collect_imported_types, (void *)result);
  }

  return result;
}

static void collect_imported_ns_types(xmlSchemaImportPtr import, VALUE result, const xmlChar *name)
{
  if (import->imported && import->schema)
  {
    VALUE types = rb_hash_new();
    xmlHashScan(import->schema->typeDecl, (xmlHashScanner)scan_schema_type, (void *)types);
    rb_hash_aset(result, QNIL_OR_STRING(import->schema->targetNamespace), types);
  }
}

/*
 * call-seq:
 *    XML::Schema.imported_ns_types -> hash
 *
 * Returns a hash by namespace of a hash of schema types within the entire schema including imports
 */
static VALUE rxml_schema_imported_ns_types(VALUE self)
{
  xmlSchemaPtr xschema;
  VALUE result = rb_hash_new();

  Data_Get_Struct(self, xmlSchema, xschema);

  if (xschema)
  {
    xmlHashScan(xschema->schemasImports, (xmlHashScanner)collect_imported_ns_types, (void *)result);
  }

  return result;
}

void rxml_init_schema(void)
{
  cXMLSchema = rb_define_class_under(mXML, "Schema", rb_cObject);
  rb_undef_alloc_func(cXMLSchema);

  rb_define_singleton_method(cXMLSchema, "new", rxml_schema_init_from_uri, 1);
  rb_define_singleton_method(cXMLSchema, "from_string", rxml_schema_init_from_string, 1);
  rb_define_singleton_method(cXMLSchema, "document", rxml_schema_init_from_document, 1);

  /* Create attr_reader methods for the above instance variables */
  rb_define_attr(cXMLSchema, "target_namespace", 1, 0);
  rb_define_attr(cXMLSchema, "name", 1, 0);
  rb_define_attr(cXMLSchema, "id", 1, 0);
  rb_define_attr(cXMLSchema, "version", 1, 0);

  // These are just methods so as to hide their values and not overly clutter the output of inspect/to_str
  rb_define_method(cXMLSchema, "document", rxml_schema_document, 0);
  rb_define_method(cXMLSchema, "namespaces", rxml_schema_namespaces, 0);
  rb_define_method(cXMLSchema, "elements", rxml_schema_elements, 0);
  rb_define_method(cXMLSchema, "imported_ns_elements", rxml_schema_imported_ns_elements, 0);
  rb_define_method(cXMLSchema, "types", rxml_schema_types, 0);
  rb_define_method(cXMLSchema, "imported_types", rxml_schema_imported_types, 0);
  rb_define_method(cXMLSchema, "imported_ns_types", rxml_schema_imported_ns_types, 0);

  rxml_init_schema_facet();
  rxml_init_schema_element();
  rxml_init_schema_attribute();
  rxml_init_schema_type();
}
