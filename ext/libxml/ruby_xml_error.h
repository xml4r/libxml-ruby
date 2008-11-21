/* $Id: rxml_ns.h 324 2008-07-08 23:00:02Z cfis $ */

/* Please see the LICENSE file for copyright and distribution information */

#ifndef __rxml_ERROR__
#define __rxml_ERROR__

extern VALUE eXMLError;

void ruby_init_xml_error();
void rxml_raise(xmlErrorPtr xerror);

#endif
