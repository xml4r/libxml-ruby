/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

#ifndef __RUBY_XML_ATTRIBUTE__
#define __RUBY_XML_ATTRIBUTE__

extern VALUE cXMLAttribute;

typedef struct ruby_xml_attribute {
  xmlAttributePtr attribute;
  VALUE xd;
  int is_ptr;
} ruby_xml_attribute;

void ruby_xml_attribute_free(ruby_xml_attribute *rxa);
void ruby_init_xml_attribute(void);
VALUE ruby_xml_attribute_new(VALUE class, VALUE xd, xmlAttributePtr attribute);
VALUE ruby_xml_attribute_new2(VALUE class, VALUE xd, xmlAttributePtr attribute);
VALUE ruby_xml_attribute_name_get(VALUE self);
#endif
