#include "ruby_libxml.h"
#include "ruby_xml_schema.h"

/* 
 * Document-class: LibXML::XML::Schema
 *
 * The XML::Schema class is used to prepare XML Schemas for validation of xml
 * documents.
 * 
 * Schemas can be created from XML documents, strings or URIs using the
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

// Rdoc needs to know 
#ifdef RDOC_NEVER_DEFINED
  mLibXML = rb_define_module("LibXML");
  mXML = rb_define_module_under(mLibXML, "XML");
#endif

static void
rxml_schema_mark(rxml_schema *rxschema) {
  return;
}

static void
rxml_schema_free(rxml_schema *rxschema) {
  if (rxschema->schema != NULL) {
    xmlSchemaFree(rxschema->schema);
    rxschema->schema = NULL;
  }

  ruby_xfree(rxschema);
}

/*
 * call-seq:
 *    XML::Schema.new(schema_uri) -> schema
 * 
 * Create a new schema from the specified URI.
 */
static VALUE
rxml_schema_init_from_uri(VALUE class, VALUE uri) {
  xmlSchemaParserCtxtPtr parser;
  rxml_schema *schema;

  Check_Type(uri, T_STRING);

  parser = xmlSchemaNewParserCtxt(StringValuePtr(uri));
  schema = ALLOC(rxml_schema);
  schema->schema = xmlSchemaParse(parser);
  xmlSchemaFreeParserCtxt(parser);

  return Data_Wrap_Struct(cXMLSchema, rxml_schema_mark, rxml_schema_free, schema);
}

/*
 * call-seq:
 *    XML::Schema.document(document) -> schema
 * 
 * Create a new schema from the specified document.
 */
static VALUE
rxml_schema_init_from_document(VALUE class, VALUE document) {
  xmlDocPtr xdoc;
  rxml_schema *schema;
  xmlSchemaParserCtxtPtr parser;

  Data_Get_Struct(document, xmlDoc, xdoc);

  parser = xmlSchemaNewDocParserCtxt(xdoc);
  schema = ALLOC(rxml_schema);
  schema->schema = xmlSchemaParse(parser);
  xmlSchemaFreeParserCtxt(parser);

  return Data_Wrap_Struct(cXMLSchema, rxml_schema_mark, rxml_schema_free, schema);
}

/*
 * call-seq:
 *    XML::Schema.string("schema_data") -> "value"
 * 
 * Create a new schema using the specified string.
 */
static VALUE
rxml_schema_init_from_string(VALUE self, VALUE schema_str) {
  xmlSchemaParserCtxtPtr  parser;
  rxml_schema *rxschema;

  Check_Type(schema_str, T_STRING);

  parser = xmlSchemaNewMemParserCtxt(StringValuePtr(schema_str), strlen(StringValuePtr(schema_str)));
  rxschema = ALLOC(rxml_schema);
  rxschema->schema = xmlSchemaParse(parser);
  xmlSchemaFreeParserCtxt(parser);

  return Data_Wrap_Struct(cXMLSchema, rxml_schema_mark, rxml_schema_free, rxschema);
}
  
/* TODO what is this patch doing here?
 
 	xmlSchemaParserCtxtPtr  parser;
 	xmlSchemaPtr            sptr;
 	xmlSchemaValidCtxtPtr   vptr;
+	int                     is_invalid;
 	
 	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &source) == FAILURE) {
 		return;
@@ -598,26 +598,24 @@
 			convert_to_string_ex(&source);
 			parser = xmlSchemaNewParserCtxt(Z_STRVAL_P(source));
 			sptr = xmlSchemaParse(parser);
 			break;
 		case SCHEMA_BLOB:
 			convert_to_string_ex(&source);
 			parser = xmlSchemaNewMemParserCtxt(Z_STRVAL_P(source), Z_STRLEN_P(source));
 			sptr = xmlSchemaParse(parser);
 			break;
 	}
 
 	vptr = xmlSchemaNewValidCtxt(sptr);
+	is_invalid = xmlSchemaValidateDoc(vptr, (xmlDocPtr) sxe->document->ptr);
 	xmlSchemaFree(sptr);
 	xmlSchemaFreeValidCtxt(vptr);
 	xmlSchemaFreeParserCtxt(parser);
 
-	if (is_valid) {
-		RETURN_TRUE;
-	} else {
+	if (is_invalid) {
 		RETURN_FALSE;
+	} else {
+		RETURN_TRUE;
 	}
 }
  }}} 
@@ -695,7 +693,7 @@
 {
 	if (!strcmp(method, "xsearch")) {
 		simplexml_ce_xpath_search(INTERNAL_FUNCTION_PARAM_PASSTHRU);
-#ifdef xmlSchemaParserCtxtPtr
+#ifdef LIBXML_SCHEMAS_ENABLED
 	} else if (!strcmp(method, "validate_schema_file")) {
 		simplexml_ce_schema_validate(INTERNAL_FUNCTION_PARAM_PASSTHRU, SCHEMA_FILE);	
 	} else if (!strcmp(method, "validate_schema_buffer")) {
*/

void  ruby_init_xml_schema(void) {
  cXMLSchema = rb_define_class_under(mXML, "Schema", rb_cObject);
  rb_define_singleton_method(cXMLSchema, "new",         rxml_schema_init_from_uri, 1);
  rb_define_singleton_method(cXMLSchema, "from_string", rxml_schema_init_from_string, 1);
  rb_define_singleton_method(cXMLSchema, "document",    rxml_schema_init_from_document, 1);
}

