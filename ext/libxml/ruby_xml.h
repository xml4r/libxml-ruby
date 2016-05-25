/* Please see the LICENSE file for copyright and distribution information */

#ifndef __RUBY_XML_H__
#define __RUBY_XML_H__

extern VALUE mXML;
int rxml_libxml_default_options();
void rxml_init_xml(void);

void rxml_private_set(void *node, VALUE private);
void rxml_private_del(void *node);
VALUE rxml_private_get(void *node);
void rxml_private_mark(void *node);

#endif
