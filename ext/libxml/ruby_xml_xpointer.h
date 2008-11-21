/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

#ifndef __rxml_XPOINTER__
#define __rxml_XPOINTER__

extern VALUE cXMLXPointer;
extern VALUE eXMLXPointerInvalidExpression;

typedef struct rxml_xpointer {
  VALUE xd;
  VALUE ctxt;
  /*
   * This needs to go into a xpointer data struct:
   *
   * xmlLocationSetPtr xptr;
   *
   * I also need an xpointer data struct type.
  */
} rxml_xpointer;

VALUE rxml_xpointer_point(VALUE class, VALUE node, VALUE xptr_string);
VALUE rxml_xpointer_point2(VALUE node, VALUE xptr_string);
void ruby_init_xml_xpointer(void);

#endif
