/* Please see the LICENSE file for copyright and distribution information */

#ifndef __RUBY_XML_H__
#define __RUBY_XML_H__

extern VALUE mXML;
int rxml_libxml_default_options();
void rxml_init_xml(void);

void rxml_owned_add(void *private);
void rxml_owned_del(void *private);
int rxml_owned_p(void *private);

#define SET_PRIV(obj, private)    rxml_owned_add(obj->_private = (void*)private)

#define CLEAR_PRIV(obj) do {      \
  if (obj->_private)              \
    rxml_owned_del(obj->_private);\
  obj->_private = NULL;           \
} while (0)

#define OWNED(obj) rxml_owned_p(obj->_private)

#endif
