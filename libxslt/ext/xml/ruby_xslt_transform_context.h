/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

#ifndef __RUBY_XSLT_TRANSFORM_CONTEXT__
#define __RUBY_XSLT_TRANSFORM_CONTEXT__

extern VALUE cXSLTTransformContext;

typedef struct ruby_xslt_transform_context {
  xsltTransformContextPtr ctxt;
  VALUE xslt;
} ruby_xslt_transform_context;

void ruby_init_xslt_transform_context(void);
void ruby_xslt_transform_context_free(ruby_xslt_transform_context *ctxt);
void ruby_xslt_transform_context_mark(ruby_xslt_transform_context *ctxt);
VALUE ruby_xslt_transform_context_new(VALUE class, VALUE xslt, xsltTransformContextPtr ctxt);
VALUE ruby_xslt_transform_context_new2(VALUE class, VALUE xslt);
VALUE ruby_xslt_transform_context_new3(VALUE xslt);

#endif
