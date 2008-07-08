/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

#ifndef __RUBY_XML_NODE__
#define __RUBY_XML_NODE__

extern VALUE cXMLNode;
extern VALUE eXMLNodeSetNamespace;
extern VALUE eXMLNodeFailedModify;
extern VALUE eXMLNodeUnknownType;

VALUE ruby_xml_node2_wrap(VALUE class, xmlNodePtr xnode);
VALUE check_string_or_symbol(VALUE val);
#endif
