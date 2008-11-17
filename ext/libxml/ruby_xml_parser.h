/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

#ifndef __RUBY_XML_PARSER__
#define __RUBY_XML_PARSER__

#define MAX_LIBXML_FEATURES_LEN 50


extern VALUE cXMLParser;

void ruby_init_parser();

VALUE ruby_xml_parser_default_substitute_entities_set(VALUE class, VALUE bool);
VALUE ruby_xml_parser_default_load_external_dtd_set(VALUE class, VALUE bool);


#endif
