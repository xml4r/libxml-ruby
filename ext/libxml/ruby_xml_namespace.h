/* Please see the LICENSE file for copyright and distribution information */

#ifndef __RXML_NAMESPACE__
#define __RXML_NAMESPACE__

extern VALUE cXMLNamespace;
extern const rb_data_type_t rxml_namespace_type;

void rxml_init_namespace(void);
VALUE rxml_namespace_wrap(xmlNsPtr xns);
#endif
