/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

#include <stdarg.h>
#include "ruby_libxml.h"

VALUE cXMLParser;
VALUE mXMLParserOptions;
static ID CONTEXT_ATTR;

/*
 * Document-class: LibXML::XML::Parser
 *
 * The XML::Parser provides a tree based API for processing
 * xml documents, in contract to XML::Reader's stream
 * based api and XML::SaxParser callback based API.
 *
 * As a result, parsing a document creates an in-memory document object
 * that consist of any number of XML::Node instances.  This is simple
 * and powerful model, but has the major limitation that the size of
 * the document that can be processed is limited by the amount of
 * memory available.  In such cases, it is better to use the XML::Reader.
 *
 * Using the parser is simple:
 *
 *   parser = XML::Parser.new
 *   parser.file = 'my_file'
 *   doc = parser.parse
 *
 * You can also parse strings (see XML::Parser.string) and io objects (see
 * XML::Parser.io).
 */

/*
 * call-seq:
 *    parser.initialize(context) -> XML::Parser
 *
 * Creates a new XML::Parser from the specified 
 * XML::Parser::Context.
 */
static VALUE rxml_parser_initialize(int argc, VALUE *argv, VALUE self)
{
  VALUE context = Qnil;

  rb_scan_args(argc, argv, "01", &context);

  if (context == Qnil)
  {
    rb_warn("Passing no parameters to XML::Parser.new is deprecated.  Pass an instance of XML::Parser::Context instead.");
    context = rb_class_new_instance(0, Qnil, cXMLParserContext);
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
static VALUE rxml_parser_parse(VALUE self)
{
  xmlParserCtxtPtr ctxt;
  xmlDocPtr xdoc;
  VALUE context = rb_ivar_get(self, CONTEXT_ATTR);
  
  Data_Get_Struct(context, xmlParserCtxt, ctxt);

  if (xmlParseDocument(ctxt) == -1 || !ctxt->wellFormed)
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

void ruby_init_parser(void)
{
  cXMLParser = rb_define_class_under(mXML, "Parser", rb_cObject);

  /* Atributes */
  CONTEXT_ATTR = rb_intern("@context");
  rb_define_attr(cXMLParser, "input", 1, 0);
  rb_define_attr(cXMLParser, "context", 1, 0);

  /* Instance Methods */
  rb_define_method(cXMLParser, "initialize", rxml_parser_initialize, -1);
  rb_define_method(cXMLParser, "parse", rxml_parser_parse, 0);


  /* Constants */
  mXMLParserOptions = rb_define_module_under(cXMLParser, "Options");

  /* recover on errors */  
  rb_define_const(mXMLParserOptions, "RECOVER", INT2NUM(XML_PARSE_RECOVER));
  /* substitute entities */
  rb_define_const(mXMLParserOptions, "NOENT", INT2NUM(XML_PARSE_NOENT));
  /* load the external subset */
  rb_define_const(mXMLParserOptions, "DTDLOAD", INT2NUM(XML_PARSE_DTDLOAD));
  /* default DTD attributes */
  rb_define_const(mXMLParserOptions, "DTDATTR", INT2NUM(XML_PARSE_DTDATTR));
  /* validate with the DTD */
  rb_define_const(mXMLParserOptions, "DTDVALID", INT2NUM(XML_PARSE_DTDVALID));
  /* suppress error reports */
  rb_define_const(mXMLParserOptions, "NOERROR", INT2NUM(XML_PARSE_NOERROR));
  /* suppress warning reports */
  rb_define_const(mXMLParserOptions, "NOWARNING", INT2NUM(XML_PARSE_NOWARNING));
  /* pedantic error reporting */
  rb_define_const(mXMLParserOptions, "PEDANTIC", INT2NUM(XML_PARSE_PEDANTIC));
  /* remove blank nodes */
  rb_define_const(mXMLParserOptions, "NOBLANKS", INT2NUM(XML_PARSE_NOBLANKS));
  /* use the SAX1 interface internally */
  rb_define_const(mXMLParserOptions, "SAX1", INT2NUM(XML_PARSE_SAX1));
  /* Implement XInclude substitition  */
  rb_define_const(mXMLParserOptions, "XINCLUDE", INT2NUM(XML_PARSE_XINCLUDE));
  /* Forbid network access */
  rb_define_const(mXMLParserOptions, "NONET", INT2NUM(XML_PARSE_NONET));
  /* Do not reuse the context dictionnary */
  rb_define_const(mXMLParserOptions, "NODICT", INT2NUM(XML_PARSE_NODICT));
  /* remove redundant namespaces declarations */
  rb_define_const(mXMLParserOptions, "NSCLEAN", INT2NUM(XML_PARSE_NSCLEAN));
  /* merge CDATA as text nodes */
  rb_define_const(mXMLParserOptions, "NOCDATA", INT2NUM(XML_PARSE_NOCDATA));
  /* do not generate XINCLUDE START/END nodes */
  rb_define_const(mXMLParserOptions, "NOXINCNODE", INT2NUM(XML_PARSE_NOXINCNODE));
  /* compact small text nodes */
  rb_define_const(mXMLParserOptions, "COMPACT", INT2NUM(XML_PARSE_COMPACT));
  /* do not fixup XINCLUDE xml:base uris */
  rb_define_const(mXMLParserOptions, "NOBASEFIX", INT2NUM(XML_PARSE_NOBASEFIX));
  /* relax any hardcoded limit from the parser */
  rb_define_const(mXMLParserOptions, "HUGE", INT2NUM(XML_PARSE_HUGE));
}
