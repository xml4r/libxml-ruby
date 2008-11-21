/* $Id $ */

#ifndef __rxml_XPATH_OBJECT__
#define __rxml_XPATH_OBJECT__

extern VALUE cXMLXPathObject;

typedef struct rxml_xpath_object_s {
  xmlXPathObjectPtr xpop;
} rxml_xpath_object;

void ruby_init_xml_xpath_object(void);
VALUE rxml_xpath_object_wrap(xmlXPathObjectPtr xpop);

VALUE rxml_xpath_object_first(VALUE self);
VALUE rxml_xpath_object_to_a(VALUE self);
VALUE rxml_xpath_object_each(VALUE self);
VALUE rxml_xpath_object_empty_q(VALUE self);
VALUE rxml_xpath_object_first(VALUE self);
VALUE rxml_xpath_object_length(VALUE self);

#endif
