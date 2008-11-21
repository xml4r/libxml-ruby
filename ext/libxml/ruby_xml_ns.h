/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

#ifndef __rxml_NS__
#define __rxml_NS__

extern VALUE cXMLNS;

void ruby_init_xml_ns(void);
VALUE rxml_ns_wrap(xmlNsPtr ns);
#endif
