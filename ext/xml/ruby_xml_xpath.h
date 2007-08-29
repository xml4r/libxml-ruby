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
VALUE ruby_xml_xpath_find(int argc, VALUE *argv, VALUE class);
VALUE ruby_xml_xpath_find2(int argc, VALUE *argv);
VALUE ruby_xml_xpath_new(VALUE class, VALUE xd, VALUE xxpc,
			 xmlXPathObjectPtr xpop);
void ruby_init_xml_xpath(void);

#endif
