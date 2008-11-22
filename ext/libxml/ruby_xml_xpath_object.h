/* $Id $ */

#ifndef __rxml_XPATH_OBJECT__
#define __rxml_XPATH_OBJECT__

extern VALUE cXMLXPathObject;

void ruby_init_xml_xpath_object(void);
VALUE rxml_xpath_object_wrap(xmlXPathObjectPtr xpop);

#endif
