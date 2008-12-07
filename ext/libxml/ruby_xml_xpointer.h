/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

#ifndef __RXML_XPOINTER__
#define __RXML_XPOINTER__

extern VALUE cXMLXPointer;

void ruby_init_xml_xpointer(void);
VALUE rxml_xpointer_point2(VALUE node, VALUE xptr_str);

#endif
