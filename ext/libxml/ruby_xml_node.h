/* Please see the LICENSE file for copyright and distribution information */

#ifndef __RXML_NODE__
#define __RXML_NODE__

extern VALUE cXMLNode;
extern const rb_data_type_t rxml_node_data_type;
extern const rb_data_type_t rxml_node_unmanaged_data_type;

void rxml_init_node(void);
void rxml_node_mark(xmlNodePtr xnode);
VALUE rxml_node_wrap(xmlNodePtr xnode);
void rxml_node_manage(xmlNodePtr xnode, VALUE node);
void rxml_node_unmanage(xmlNodePtr xnode, VALUE node);
#endif
