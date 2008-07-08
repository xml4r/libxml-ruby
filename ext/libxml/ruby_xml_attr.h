/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

#ifndef __RUBY_XML_ATTR__
#define __RUBY_XML_ATTR__

extern VALUE cXMLAttr;

VALUE ruby_xml_attr_new(xmlAttrPtr xattr);
void ruby_xml_attr_free(xmlAttrPtr xattr);
VALUE ruby_xml_attr_wrap(xmlAttrPtr xattr);
#endif
