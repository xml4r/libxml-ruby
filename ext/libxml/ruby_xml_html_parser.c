/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

#include "ruby_libxml.h"

VALUE cXMLHTMLParser;
ID INPUT_ATTR;
ID CONTEXT_ATTR;


/*
* Document-class: LibXML::XML::HTMLParser
*
* The HTML parser implements an HTML 4.0 non-verifying parser with an API 
* compatible with the XML::Parser.  In contrast with the XML::Parser, 
* it can parse "real world" HTML, even if it severely broken from a
* specification point of view. */


/*
 * call-seq:
 *    XML::HTMLParser.initialize -> parser
 * 
 * Initializes a new parser instance with no pre-determined source.
 */
VALUE
ruby_xml_html_parser_initialize(VALUE self) {
  VALUE input = rb_class_new_instance(0, NULL, cXMLInput);
  rb_iv_set(self, "@input", input);
  rb_iv_set(self, "@context", Qnil);
  return self;
}

htmlParserCtxtPtr
ruby_xml_html_parser_file_ctxt(VALUE input) {
  VALUE file = rb_ivar_get(input, FILE_ATTR);
  VALUE encoding = rb_ivar_get(input, ENCODING_ATTR);
  VALUE encodingStr = ruby_xml_encoding_to_s(Qnil, encoding);

  return htmlCreateFileParserCtxt(StringValuePtr(file), StringValuePtr(encodingStr));
}

htmlParserCtxtPtr
ruby_xml_html_parser_str_ctxt(VALUE input) {
  VALUE data = rb_ivar_get(input, STRING_ATTR);
  return htmlCreateMemoryParserCtxt(StringValuePtr(data), RSTRING_LEN(data));
}

/*
htmlParserCtxtPtr
ruby_xml_html_parser_io_ctxt(VALUE input) {
  VALUE io = rb_ivar_get(input, IO_ATTR);
  VALUE encoding = rb_ivar_get(input, ENCODING_ATTR);
  xmlCharEncoding xmlEncoding = NUM2INT(encoding);

  OpenFile *fptr;
  FILE *f;

  GetOpenFile(io, fptr);
  rb_io_check_readable(fptr);
  f = GetWriteFile(fptr);

  return htmlCreateIOParserCtxt(NULL, NULL,
				     (xmlInputReadCallback) ctxtRead,
				     NULL, f, xmlEncoding);
}
*/

/*
 * call-seq:
 *    parser.parse -> document
 * 
 * Parse the input XML and create an XML::Document with
 * it's content. If an error occurs, XML::Parser::ParseError
 * is thrown.
 */
VALUE
ruby_xml_html_parser_parse(VALUE self) {
  xmlParserCtxtPtr ctxt;
  VALUE context;
  VALUE input = rb_ivar_get(self, INPUT_ATTR);

  context = rb_ivar_get(self, CONTEXT_ATTR);
  if (context != Qnil)
    rb_raise(rb_eRuntimeError, "You cannot parse a data source twice");

  if (rb_ivar_get(input, FILE_ATTR) != Qnil)
    ctxt = ruby_xml_html_parser_file_ctxt(input);
  else if (rb_ivar_get(input, STRING_ATTR) != Qnil)
    ctxt = ruby_xml_html_parser_str_ctxt(input);
  /*else if (rb_ivar_get(input, DOCUMENT_ATTR) != Qnil)
    ctxt = ruby_xml_html_parser_parse_document(input);
  else if (rb_ivar_get(input, IO_ATTR) != Qnil)
    ctxt = ruby_xml_html_parser_io_ctxt(input);*/
  else
    rb_raise(rb_eArgError, "You must specify a parser data source");
  
  if (!ctxt)
    ruby_xml_raise(&xmlLastError);

  context = ruby_xml_parser_context_wrap(ctxt);
  rb_ivar_set(self, CONTEXT_ATTR, context);
 
  if (htmlParseDocument(ctxt) == -1 || !ctxt->wellFormed) {
    xmlFreeDoc(ctxt->myDoc);
    ruby_xml_raise(&ctxt->lastError);
  }

  return ruby_xml_document_wrap(ctxt->myDoc);
}

// Rdoc needs to know 
#ifdef RDOC_NEVER_DEFINED
  mLibXML = rb_define_module("LibXML");
  mXML = rb_define_module_under(mLibXML, "XML");
#endif

void
ruby_init_html_parser(void) {	
  INPUT_ATTR = rb_intern("@input");
  CONTEXT_ATTR = rb_intern("@context");

  cXMLHTMLParser = rb_define_class_under(mXML, "HTMLParser", rb_cObject);
 
  /* Atributes */
  rb_define_attr(cXMLHTMLParser, "input", 1, 0);
  rb_define_attr(cXMLHTMLParser, "context", 1, 0);

  /* Instance methods */
  rb_define_method(cXMLHTMLParser, "initialize", ruby_xml_html_parser_initialize, 0);
  rb_define_method(cXMLHTMLParser, "parse", ruby_xml_html_parser_parse, 0);
}
