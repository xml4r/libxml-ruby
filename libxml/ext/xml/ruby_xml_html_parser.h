/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

#ifndef __RUBY_XML_HTML_PARSER__
#define __RUBY_XML_HTML_PARSER__

extern int ruby_xml_html_parser_count;
extern VALUE cXMLHTMLParser;

typedef struct ruby_xml_html_parser {
  VALUE ctxt;
  int parsed;
  void *data;
  int data_type;
} ruby_xml_html_parser;

/*
 * VALUE ruby_xml_html_parser_filename_get(VALUE self);
VALUE ruby_xml_html_parser_filename_set(VALUE self, VALUE filename);
VALUE ruby_xml_html_parser_new(VALUE class);
*/
VALUE ruby_xml_html_parser_parse(VALUE self);
VALUE ruby_xml_html_parser_str_get(VALUE self);
VALUE ruby_xml_html_parser_str_set(VALUE self, VALUE str);

void ruby_init_html_parser(void);

#endif
