/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

#ifndef __RUBY_XML_ATTR__
#define __RUBY_XML_ATTR__

extern VALUE cXMLAttr;

typedef struct ruby_xml_attr {
  xmlAttrPtr attr;
} ruby_xml_attr_t;

void ruby_xml_attr_free(ruby_xml_attr_t *rxn);
VALUE ruby_xml_attr_wrap(VALUE class, xmlAttrPtr xnode);
void ruby_init_xml_attr(void);
VALUE ruby_xml_attr_name_get(VALUE self);
#endif
