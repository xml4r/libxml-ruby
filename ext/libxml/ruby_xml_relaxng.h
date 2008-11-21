#ifndef __rxml_RELAXNG__
#define __rxml_RELAXNG__

#include <libxml/relaxng.h>

extern VALUE cXMLRelaxNG;

typedef struct rxp_relaxng {
  xmlRelaxNGPtr relaxng;   /* RelaxNG interface */
} rxml_relaxng;

void  ruby_init_xml_relaxng(void);
void  ruby_relaxng_free(rxml_relaxng *rxs);
#endif

