/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

#ifndef __rxml_PARSER__
#define __rxml_PARSER__

#define MAX_LIBXML_FEATURES_LEN 50


extern VALUE cXMLParser;

void ruby_init_parser();

VALUE rxml_parser_default_substitute_entities_set(VALUE class, VALUE bool);
VALUE rxml_parser_default_load_external_dtd_set(VALUE class, VALUE bool);


#endif
