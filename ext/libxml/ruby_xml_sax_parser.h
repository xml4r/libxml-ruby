/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

#ifndef __rxml_SAX_PARSER__
#define __rxml_SAX_PARSER__

extern VALUE cXMLSaxParser;

typedef struct rxml_sax_parser {
  xmlParserCtxtPtr xpc;
  xmlSAXHandlerPtr xsh;
  VALUE callbackHandler;
  VALUE filename;
  VALUE str;
} rxml_sax_parser;

void ruby_init_xml_sax_parser(void);

#endif
