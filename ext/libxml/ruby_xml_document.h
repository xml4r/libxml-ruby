/* Please see the LICENSE file for copyright and distribution information */

#ifndef __RXML_DOCUMENT__
#define __RXML_DOCUMENT__

extern VALUE cXMLDocument;
extern const rb_data_type_t rxml_document_data_type;
void rxml_init_document(void);
VALUE rxml_document_wrap(xmlDocPtr xnode);

typedef xmlChar * xmlCharPtr;
#endif
