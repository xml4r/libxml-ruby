/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

#ifndef __rxml_XPATH_CONTEXT__
#define __rxml_XPATH_CONTEXT__

extern VALUE cXMLXPathContext;
void ruby_init_xml_xpath_context(void);

VALUE rxml_xpath_context_register_namespaces(VALUE self, VALUE nslist);
VALUE rxml_xpath_context_register_namespaces_from_node(VALUE self, VALUE node);
VALUE rxml_xpath_context_find(VALUE self, VALUE xpath_expr);

#endif
