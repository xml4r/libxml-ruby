/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

#ifndef __rxml_XPATH__
#define __rxml_XPATH__

extern VALUE mXPath;

typedef struct rxml_xpath {
  VALUE xd;
  VALUE ctxt;
  xmlXPathObjectPtr xpop;
} rxml_xpath;

void rxml_xpath_free(rxml_xpath *rxxp);
VALUE rxml_xpath_find(VALUE class, VALUE anode, VALUE xpath_expr, VALUE nslist);

void ruby_init_xml_xpath(void);

#endif
