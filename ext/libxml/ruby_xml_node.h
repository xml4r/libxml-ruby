/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

#ifndef __RUBY_XML_NODE__
#define __RUBY_XML_NODE__

extern VALUE cXMLNode;
extern VALUE eXMLNodeSetNamespace;
extern VALUE eXMLNodeFailedModify;
extern VALUE eXMLNodeUnknownType;

VALUE
ruby_xml_node2_wrap(VALUE class, xmlNodePtr xnode);

void ruby_xml_node_free(xmlNodePtr xnode);
void ruby_xml_node_mark_common(xmlNodePtr xnode);
void ruby_init_xml_node(void);

VALUE check_string_or_symbol(VALUE val);

VALUE ruby_xml_node_child_set(VALUE self, VALUE obj);
VALUE ruby_xml_node_name_get(VALUE self);
VALUE ruby_xml_node_property_get(VALUE self, VALUE key);
VALUE ruby_xml_node_property_set(VALUE self, VALUE key, VALUE val);
#endif
