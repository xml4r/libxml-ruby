/* $Id: ruby_xml_ns.h 324 2008-07-08 23:00:02Z cfis $ */

/* Please see the LICENSE file for copyright and distribution information */

#ifndef __RUBY_XML_ERROR__
#define __RUBY_XML_ERROR__

extern VALUE eXMLError;

void ruby_init_xml_error();
void ruby_xml_raise(xmlErrorPtr xerror);

#endif
