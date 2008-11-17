/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

#ifndef __RUBY_XML_PARSER_CONTEXT__
#define __RUBY_XML_PARSER_CONTEXT__

extern VALUE cXMLParserContext;

void ruby_init_xml_parser_context(void);
VALUE ruby_xml_parser_context_wrap(xmlParserCtxtPtr ctxt);

#endif
