/* $Id: ruby_xml_ns.h 324 2008-07-08 23:00:02Z cfis $ */

/* Please see the LICENSE file for copyright and distribution information */

#ifndef __RUBY_XML_ENCODING__
#define __RUBY_XML_ENCODING__

extern VALUE mXMLEncoding;

void ruby_init_xml_encoding(void);
VALUE ruby_xml_encoding_to_s(VALUE self, VALUE encoding);

#endif
