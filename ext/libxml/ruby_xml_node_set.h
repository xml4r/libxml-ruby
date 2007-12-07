/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

#ifndef __RUBY_XML_NODE_SET__
#define __RUBY_XML_NODE_SET__

extern VALUE cXMLNodeSet;

typedef struct ruby_xml_node_set {
  VALUE rxpop;
} ruby_xml_node_set;

void ruby_init_xml_node_set(void);
VALUE ruby_xml_node_set_new(VALUE class, VALUE xpop);
VALUE ruby_xml_node_set_new2(VALUE xpop);
VALUE ruby_xml_node_set_each(VALUE self);
VALUE ruby_xml_node_set_first(VALUE self);

#endif
