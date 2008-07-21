#ifndef __RUBY_XML_RELAXNG__
#define __RUBY_XML_RELAXNG__

#include <libxml/relaxng.h>

extern VALUE cXMLRelaxNG;

typedef struct rxp_relaxng {
  xmlRelaxNGPtr relaxng;   /* RelaxNG interface */
} ruby_xml_relaxng;

void  ruby_init_xml_relaxng(void);
void  ruby_relaxng_free(ruby_xml_relaxng *rxs);
#endif

