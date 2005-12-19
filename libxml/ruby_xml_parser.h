/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

#ifndef __RUBY_XML_PARSER__
#define __RUBY_XML_PARSER__

#define MAX_LIBXML_FEATURES_LEN 50

extern int ruby_xml_parser_count;
extern VALUE cXMLParser;
extern VALUE eXMLParserParseError;

typedef struct ruby_xml_parser {
  VALUE ctxt;
  int parsed;
  void *data;
  int data_type;
} ruby_xml_parser;

VALUE ruby_xml_parser_default_load_external_dtd_set(VALUE class, VALUE bool);
VALUE ruby_xml_parser_default_substitute_entities_set(VALUE class, VALUE bool);
VALUE ruby_xml_parser_features(VALUE self);
VALUE ruby_xml_parser_filename_get(VALUE self);
VALUE ruby_xml_parser_filename_set(VALUE self, VALUE filename);
VALUE ruby_xml_parser_new(VALUE class);
VALUE ruby_xml_parser_parse(VALUE self);
VALUE ruby_xml_parser_str_get(VALUE self);
VALUE ruby_xml_parser_str_set(VALUE self, VALUE str);

#endif
