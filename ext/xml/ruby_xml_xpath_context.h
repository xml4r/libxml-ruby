/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

#ifndef __RUBY_XML_XPATH_CONTEXT__
#define __RUBY_XML_XPATH_CONTEXT__

extern VALUE cXMLXPathContext;

typedef struct ruby_xml_xpath_context {
  VALUE xd;
  xmlXPathContextPtr ctxt;
} ruby_xml_xpath_context;

VALUE ruby_xml_xpath_context_wrap(xmlXPathContextPtr ctxt);
VALUE ruby_xml_xpath_context_new(VALUE anode);
VALUE ruby_xml_xpath_context_register_namespace(VALUE self, VALUE prefix, VALUE uri);
void ruby_init_xml_xpath_context(void);

#endif
