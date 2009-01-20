/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

#include "ruby_libxml.h"

/* Document-class: LibXML::XML::HTMLParser
 *
 * The HTML parser implements an HTML 4.0 non-verifying parser with an API
 * compatible with the XML::Parser.  In contrast with the XML::Parser,
 * it can parse "real world" HTML, even if it severely broken from a
 * specification point of view.
*/

VALUE cXMLHtmlParser;

/* Document-class: LibXML::XML::HTMLParser::Options
 *
 * The HTML parser implements an HTML 4.0 non-verifying parser with an API
 * compatible with the XML::Parser.  In contrast with the XML::Parser,
 * it can parse "real world" HTML, even if it severely broken from a
 * specification point of view.
*/
VALUE mXMLHtmlParserOptions;
static ID CONTEXT_ATTR;


/* call-seq:
 *    XML::HTMLParser.initialize -> parser
 *
 * Initializes a new parser instance with no pre-determined source.
 */
static VALUE rxml_html_parser_initialize(int argc, VALUE *argv, VALUE self)
{
  VALUE context = Qnil;

  rb_scan_args(argc, argv, "01", &context);

  if (context == Qnil)
  {
    rb_warn("Passing no parameters to XML::HTMLParser.new is deprecated.  Pass an instance of XML::Parser::Context instead.");
    context = rb_class_new_instance(0, NULL, cXMLParserContext);
  }

  rb_ivar_set(self, CONTEXT_ATTR, context);
  return self;
}

/*
 * call-seq:
 *    parser.parse -> XML::Document
 *
 * Parse the input XML and create an XML::Document with
 * it's content. If an error occurs, XML::Parser::ParseError
 * is thrown.
 */
static VALUE rxml_html_parser_parse(VALUE self)
{
  xmlParserCtxtPtr ctxt;
  VALUE context = rb_ivar_get(self, CONTEXT_ATTR);
  
  Data_Get_Struct(context, xmlParserCtxt, ctxt);

  if (htmlParseDocument(ctxt) == -1)
  {
    if (ctxt->myDoc)
      xmlFreeDoc(ctxt->myDoc);
    rxml_raise(&ctxt->lastError);
  }

  return rxml_document_wrap(ctxt->myDoc);
}

// Rdoc needs to know
#ifdef RDOC_NEVER_DEFINED
mLibXML = rb_define_module("LibXML");
mXML = rb_define_module_under(mLibXML, "XML");
#endif

void ruby_init_html_parser(void)
{
  CONTEXT_ATTR = rb_intern("@context");

  cXMLHtmlParser = rb_define_class_under(mXML, "HTMLParser", rb_cObject);

  /* Atributes */
  rb_define_attr(cXMLHtmlParser, "input", 1, 0);

  /* Instance methods */
  rb_define_method(cXMLHtmlParser, "initialize", rxml_html_parser_initialize, -1);
  rb_define_method(cXMLHtmlParser, "parse", rxml_html_parser_parse, 0);
}
