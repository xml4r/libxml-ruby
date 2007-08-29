/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

#ifndef __RUBY_XML_NODE__
#define __RUBY_XML_NODE__

extern VALUE cXMLNode;
extern VALUE eXMLNodeSetNamespace;
extern VALUE eXMLNodeFailedModify;
extern VALUE eXMLNodeUnknownType;

typedef struct ruby_xml_node {
  xmlNodePtr node;
  VALUE xd;
  //int is_ptr;
} ruby_xml_node;

void ruby_xml_node_free(ruby_xml_node *rxn);
void ruby_init_xml_node(void);
VALUE ruby_xml_node_child_set(VALUE self, VALUE obj);
VALUE ruby_xml_node_new(VALUE class, xmlNodePtr node);
VALUE ruby_xml_node_new_ptr(VALUE class, VALUE xd, xmlNodePtr node);
VALUE ruby_xml_node_new3(VALUE class, VALUE xd, xmlNodePtr node, int ptr);
VALUE ruby_xml_node_name_get(VALUE self);
VALUE ruby_xml_node_property_get(VALUE self, VALUE key);
VALUE ruby_xml_node_property_set(VALUE self, VALUE key, VALUE val);
VALUE ruby_xml_node_set_ptr(VALUE node, int is_ptr);
#endif
