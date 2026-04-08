#ifndef __RXML_XPATH_OBJECT__
#define __RXML_XPATH_OBJECT__

extern VALUE cXMLXPathObject;
extern const rb_data_type_t rxml_xpath_object_data_type;

typedef struct rxml_xpath_object
{
  VALUE document;
  xmlDocPtr xdoc;
  xmlXPathObjectPtr xpop;
  VALUE nsnodes;
} rxml_xpath_object;


void rxml_init_xpath_object(void);
VALUE rxml_xpath_object_wrap(VALUE document, xmlDocPtr xdoc, xmlXPathObjectPtr xpop);

#endif
