/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

#ifndef __RUBY_XML_XPATH__
#define __RUBY_XML_XPATH__

extern VALUE cXMLXPath;
extern VALUE eXMLXPathInvalidPath;

typedef struct ruby_xml_xpath {
  VALUE xd;
  VALUE ctxt;
  xmlXPathObjectPtr xpop;
} ruby_xml_xpath;

void ruby_xml_xpath_free(ruby_xml_xpath *rxxp);
VALUE ruby_xml_xpath_find(VALUE class, VALUE anode, VALUE xpath_expr, VALUE nslist);
VALUE ruby_xml_xpath_find2(VALUE anode, VALUE xpath_expr, VALUE nslist);

void ruby_init_xml_xpath(void);

#endif
