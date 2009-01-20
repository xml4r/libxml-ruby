/* $Id: ruby_xml_html_parser.c 710 2009-01-20 05:30:51Z cfis $ */

/* Please see the LICENSE file for copyright and distribution information */

#include "ruby_libxml.h"


/* Document-class: LibXML::XML::HTMLParserOptions
 *
 * The HTML parser implements an HTML 4.0 non-verifying parser with an API
 * compatible with the XML::Parser.  In contrast with the XML::Parser,
 * it can parse "real world" HTML, even if it severely broken from a
 * specification point of view.
*/

VALUE mXMLHtmlParserOptions;


// Rdoc needs to know
#ifdef RDOC_NEVER_DEFINED
mLibXML = rb_define_module("LibXML");
mXML = rb_define_module_under(mLibXML, "XML");
cXMLHtmlParser = rb_define_class_under(mXML, "HTMLParser", rb_cObject);
#endif

void ruby_init_html_parser_options(void)
{
  mXMLHtmlParserOptions = rb_define_module_under(cXMLHtmlParser, "Options");

  /* Relaxed parsing */
  rb_define_const(mXMLHtmlParserOptions, "RECOVER", INT2NUM(HTML_PARSE_RECOVER)); 
  /* suppress error reports */
  rb_define_const(mXMLHtmlParserOptions, "NOERROR", INT2NUM(HTML_PARSE_NOERROR)); 
  /* suppress warning reports */
  rb_define_const(mXMLHtmlParserOptions, "NOWARNING", INT2NUM(HTML_PARSE_NOWARNING));
  /* pedantic error reporting */
  rb_define_const(mXMLHtmlParserOptions, "PEDANTIC", INT2NUM(HTML_PARSE_PEDANTIC)); 
  /* remove blank nodes */
  rb_define_const(mXMLHtmlParserOptions, "NOBLANKS", INT2NUM(HTML_PARSE_NOBLANKS)); 
  /* Forbid network access */
  rb_define_const(mXMLHtmlParserOptions, "NONET", INT2NUM(HTML_PARSE_NONET)); 
  /* compact small text nodes */
  rb_define_const(mXMLHtmlParserOptions, "COMPACT", INT2NUM(HTML_PARSE_COMPACT));
}
