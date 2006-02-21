/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

#ifndef __RUBY_XML_NODE_SET__
#define __RUBY_XML_NODE_SET__

extern VALUE cXMLNodeSet;

typedef struct ruby_xml_node_set {
  xmlNodeSetPtr node_set;
  VALUE xd;
  VALUE xpath;
  int data_type;
  void *data;
} ruby_xml_node_set;

void ruby_xml_node_set_free(ruby_xml_node_set *rxnset);
void ruby_init_xml_node_set(void);
VALUE ruby_xml_node_set_new(VALUE class, VALUE xd, VALUE xpath,
			    xmlNodeSetPtr node_set);
VALUE ruby_xml_node_set_new2(VALUE xd, VALUE xpath,
			     xmlNodeSetPtr node_set);
VALUE ruby_xml_node_set_each(VALUE self);

#endif
