/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

#ifndef __RUBY_XML_DOCUMENT__
#define __RUBY_XML_DOCUMENT__

extern VALUE cXMLDocument;

void ruby_init_xml_document();
int ruby_xml_document_incr(xmlDocPtr xdoc);
int ruby_xml_document_decr(xmlDocPtr xdoc);


#if defined(_WIN32)
__declspec(dllexport) 
#endif
VALUE ruby_xml_document_wrap(xmlDocPtr xnode);

#endif
