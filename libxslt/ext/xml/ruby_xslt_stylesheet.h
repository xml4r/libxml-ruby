/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information. */

#ifndef __RUBY_LIBXSLT_STYLESHEET__
#define __RUBY_LIBXSLT_STYLESHEET__

extern VALUE cXSLTStylesheet;

typedef struct ruby_xslt_stylesheet {
  int data_type;
  void *data;
  VALUE parsed;      /* XML::Document # parsed xml document after xsl apply */
  VALUE xml_doc_obj; /* XML::Document */
  xsltStylesheetPtr xsp;
} ruby_xslt_stylesheet;

void ruby_init_xslt_stylesheet(void);
VALUE ruby_xslt_stylesheet_new(VALUE class, xsltStylesheetPtr xsp);

#endif /* __RUBY_LIBXSLT_STYLESHEET__ */
