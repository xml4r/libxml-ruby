/* $Id: ruby_xml_attributes.h 282 2008-07-01 06:44:30Z cfis $ */

/* Please see the LICENSE file for copyright and distribution information */

#ifndef __RUBY_XML_ATTRIBUTES__
#define __RUBY_XML_ATTRIBUTES__

extern VALUE cXMLAttributesibutes;

void ruby_init_xml_attributes(void);
VALUE ruby_xml_attributes_new(xmlNodePtr xnode);

VALUE ruby_xml_attributes_attribute_get(VALUE self, VALUE name);
VALUE ruby_xml_attributes_attribute_set(VALUE self, VALUE name, VALUE value);


#endif
