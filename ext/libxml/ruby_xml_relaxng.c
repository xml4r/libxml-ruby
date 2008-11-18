#include "ruby_libxml.h"
#include "ruby_xml_relaxng.h"

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

// Rdoc needs to know 
#ifdef RDOC_NEVER_DEFINED
  mLibXML = rb_define_module("LibXML");
  mXML = rb_define_module_under(mLibXML, "XML");
#endif

static void
ruby_xml_relaxng_mark(ruby_xml_relaxng *rxrelaxng) {
  return;
}

void
ruby_xml_relaxng_free(ruby_xml_relaxng *rxrelaxng) {
  if (rxrelaxng->relaxng != NULL) {
    xmlRelaxNGFree(rxrelaxng->relaxng);
    rxrelaxng->relaxng = NULL;
  }

  ruby_xfree(rxrelaxng);
}

/*
 * call-seq:
 *    XML::Relaxng.new(relaxng_uri) -> relaxng
 * 
 * Create a new relaxng from the specified URI.
 */
VALUE
ruby_xml_relaxng_init_from_uri(VALUE class, VALUE uri) {
  xmlRelaxNGParserCtxtPtr parser;
  ruby_xml_relaxng *relaxng;

  Check_Type(uri, T_STRING);

  parser = xmlRelaxNGNewParserCtxt(StringValuePtr(uri));
  relaxng = ALLOC(ruby_xml_relaxng);
  relaxng->relaxng = xmlRelaxNGParse(parser);
  xmlRelaxNGFreeParserCtxt(parser);

  return Data_Wrap_Struct(cXMLRelaxNG, ruby_xml_relaxng_mark, ruby_xml_relaxng_free, relaxng);
}

/*
 * call-seq:
 *    XML::RelaxNG.document(document) -> relaxng
 * 
 * Create a new relaxng from the specified document.
 */
VALUE
ruby_xml_relaxng_init_from_document(VALUE class, VALUE document) {
  xmlDocPtr xdoc;
  ruby_xml_relaxng *relaxng;
  xmlRelaxNGParserCtxtPtr parser;

  Data_Get_Struct(document, xmlDoc, xdoc);

  parser = xmlRelaxNGNewDocParserCtxt(xdoc);
  relaxng = ALLOC(ruby_xml_relaxng);
  relaxng->relaxng = xmlRelaxNGParse(parser);
  xmlRelaxNGFreeParserCtxt(parser);

  return Data_Wrap_Struct(cXMLRelaxNG, ruby_xml_relaxng_mark, ruby_xml_relaxng_free, relaxng);
}

/*
 * call-seq:
 *    XML::RelaxNG.string("relaxng_data") -> "value"
 * 
 * Create a new relaxng using the specified string.
 */
VALUE
ruby_xml_relaxng_init_from_string(VALUE self, VALUE relaxng_str) {
  xmlRelaxNGParserCtxtPtr  parser;
  ruby_xml_relaxng *rxrelaxng;

  Check_Type(relaxng_str, T_STRING);

  parser = xmlRelaxNGNewMemParserCtxt(StringValuePtr(relaxng_str), strlen(StringValuePtr(relaxng_str)));
  rxrelaxng = ALLOC(ruby_xml_relaxng);
  rxrelaxng->relaxng = xmlRelaxNGParse(parser);
  xmlRelaxNGFreeParserCtxt(parser);

  return Data_Wrap_Struct(cXMLRelaxNG, ruby_xml_relaxng_mark, ruby_xml_relaxng_free, rxrelaxng);
}
  
void  ruby_init_xml_relaxng(void) {
  cXMLRelaxNG = rb_define_class_under(mXML, "RelaxNG", rb_cObject);
  rb_define_singleton_method(cXMLRelaxNG, "new",         ruby_xml_relaxng_init_from_uri, 1);
  rb_define_singleton_method(cXMLRelaxNG, "from_string", ruby_xml_relaxng_init_from_string, 1);
  rb_define_singleton_method(cXMLRelaxNG, "document",    ruby_xml_relaxng_init_from_document, 1);
}

