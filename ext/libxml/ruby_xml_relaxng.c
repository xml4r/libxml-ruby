#include "ruby_libxml.h"
#include "ruby_xml_relaxng.h"

#include <libxml/relaxng.h>

/*
 * Document-class: LibXML::XML::RelaxNG
 *
 * The XML::RelaxNG class is used to prepare RelaxNG schemas for validation
 * of xml documents.
 *
 * Schemas can be created from XML documents, strings or URIs using the
 * corresponding methods (new for URIs).
 *
 * Once a schema is prepared, an XML document can be validated by the
 * XML::Document#validate_relaxng method providing the XML::RelaxNG object
 * as parameter. The method will raise an exception if the document is
 * not valid.
 *
 * Basic Usage:
 *
 *  # parse schema as xml document
 *  relaxng_document = XML::Document.file('schema.rng')
 *
 *  # prepare schema for validation
 *  relaxng_schema = XML::RelaxNG.document(relaxng_document)
 *
 *  # parse xml document to be validated
 *  instance = XML::Document.file('instance.xml')
 *
 *  # validate
 *  instance.validate_relaxng(relaxng_schema)
 */

VALUE cXMLRelaxNG;

static void rxml_relaxng_free(void* data)
{
  xmlRelaxNGPtr xrelaxng = (xmlRelaxNGPtr)data;
  xmlRelaxNGFree(xrelaxng);
}

const rb_data_type_t rxml_relaxng_data_type = {
  .wrap_struct_name = "LibXML::XML::RelaxNG",
  .function = { .dmark = NULL, .dfree = rxml_relaxng_free },
  .flags = RUBY_TYPED_FREE_IMMEDIATELY,
};

/*
 * call-seq:
 *    XML::Relaxng.new(relaxng_uri) -> relaxng
 *
 * Create a new relaxng from the specified URI.
 */
static VALUE rxml_relaxng_init_from_uri(VALUE class, VALUE uri)
{
  xmlRelaxNGParserCtxtPtr xparser;
  xmlRelaxNGPtr xrelaxng;

  Check_Type(uri, T_STRING);

  xparser = xmlRelaxNGNewParserCtxt(StringValuePtr(uri));
  xrelaxng = xmlRelaxNGParse(xparser);
  xmlRelaxNGFreeParserCtxt(xparser);

  return TypedData_Wrap_Struct(cXMLRelaxNG, &rxml_relaxng_data_type, xrelaxng);
}

/*
 * call-seq:
 *    XML::RelaxNG.document(document) -> relaxng
 *
 * Create a new relaxng from the specified document.
 */
static VALUE rxml_relaxng_init_from_document(VALUE class, VALUE document)
{
  xmlDocPtr xdoc;
  xmlRelaxNGPtr xrelaxng;
  xmlRelaxNGParserCtxtPtr xparser;

  TypedData_Get_Struct(document, xmlDoc, &rxml_document_data_type, xdoc);

  xparser = xmlRelaxNGNewDocParserCtxt(xdoc);
  xrelaxng = xmlRelaxNGParse(xparser);
  xmlRelaxNGFreeParserCtxt(xparser);

  return TypedData_Wrap_Struct(cXMLRelaxNG, &rxml_relaxng_data_type, xrelaxng);
}

/*
 * call-seq:
 *    XML::RelaxNG.string("relaxng_data") -> "value"
 *
 * Create a new relaxng using the specified string.
 */
static VALUE rxml_relaxng_init_from_string(VALUE self, VALUE relaxng_str)
{
  xmlRelaxNGParserCtxtPtr xparser;
  xmlRelaxNGPtr xrelaxng;

  Check_Type(relaxng_str, T_STRING);

  xparser = xmlRelaxNGNewMemParserCtxt(StringValuePtr(relaxng_str), (int)strlen(StringValuePtr(relaxng_str)));
  xrelaxng = xmlRelaxNGParse(xparser);
  xmlRelaxNGFreeParserCtxt(xparser);

  return TypedData_Wrap_Struct(cXMLRelaxNG, &rxml_relaxng_data_type, xrelaxng);
}

void rxml_init_relaxng(void)
{
  cXMLRelaxNG = rb_define_class_under(mXML, "RelaxNG", rb_cObject);
  rb_undef_alloc_func(cXMLRelaxNG);
  rb_define_singleton_method(cXMLRelaxNG, "new", rxml_relaxng_init_from_uri, 1);
  rb_define_singleton_method(cXMLRelaxNG, "from_string",
      rxml_relaxng_init_from_string, 1);
  rb_define_singleton_method(cXMLRelaxNG, "document",
      rxml_relaxng_init_from_document, 1);
}

