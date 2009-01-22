/* $Id $ */

#ifndef __RXML_XPATH_OBJECT__
#define __RXML_XPATH_OBJECT__

extern VALUE cXMLXPathObject;

typedef struct rxml_xpath_object
{
  VALUE nodes;
  VALUE xpath_type;
  VALUE xpath_string;
} rxml_xpath_object;


void ruby_init_xml_xpath_object(void);
VALUE rxml_xpath_object_wrap(xmlXPathObjectPtr xpop);

#endif
