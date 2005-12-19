/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

#ifndef __RUBY_XML_SAX_PARSER__
#define __RUBY_XML_SAX_PARSER__

extern VALUE cXMLSaxParser;

typedef struct ruby_xml_sax_parser {
  xmlParserCtxtPtr xpc;
  xmlSAXHandlerPtr xsh;
  VALUE filename;
  VALUE str;
} ruby_xml_sax_parser;

void ruby_xml_sax_parser_free(ruby_xml_sax_parser *rxsp);
void ruby_init_xml_sax_parser(void);
VALUE ruby_xml_sax_parser_new(VALUE class);

#endif
