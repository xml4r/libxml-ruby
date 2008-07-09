/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

#ifndef __RUBY_XML_ATTR__
#define __RUBY_XML_ATTR__

extern VALUE cXMLAttr;

void ruby_init_xml_attr(void);
VALUE ruby_xml_attr_new(xmlAttrPtr xattr);
VALUE ruby_xml_attr_value_get(VALUE self);
VALUE ruby_xml_attr_value_set(VALUE self, VALUE val);
void ruby_xml_attr_free(xmlAttrPtr xattr);
VALUE ruby_xml_attr_wrap(xmlAttrPtr xattr);
#endif
