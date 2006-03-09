/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

#include "libxslt.h"
#include "ruby_xslt_transform_context.h"
#include "libxml-ruby/libxml.h"

VALUE cXSLTTransformContext;

void
ruby_xslt_transform_context_free(ruby_xslt_transform_context *rxtc) {
  if (rxtc->ctxt != NULL) {
    xsltFreeTransformContext(rxtc->ctxt);
    rxtc->ctxt = NULL;
  }
  free(rxtc);
}

void
ruby_xslt_transform_context_mark(ruby_xslt_transform_context *rxtc) {
  if (rxtc == NULL) return;
  if (!NIL_P(rxtc->xslt)) rb_gc_mark(rxtc->xslt);
}


VALUE
ruby_xslt_transform_context_new(VALUE class, VALUE xslt,
				      xsltTransformContextPtr ctxt) {
  ruby_xslt_transform_context *rxtc;
  rxtc = ALLOC(ruby_xslt_transform_context);
  ruby_xml_parser_count++;
  rxtc->ctxt = ctxt;
  rxtc->xslt = xslt;
   //fprintf(stderr,"ruby_xslt_transform_context_new 2\n");
   //if (class == Qfalse)
    //fprintf(stderr,"ruby_xslt_transform_context_new: EEEEK!\n");
  return(Data_Wrap_Struct(class, ruby_xslt_transform_context_mark,
			  ruby_xslt_transform_context_free, rxtc));
}


VALUE
ruby_xslt_transform_context_new2(VALUE class, VALUE xslt) {
  return(ruby_xslt_transform_context_new(class, xslt, NULL));
}


VALUE
ruby_xslt_transform_context_new3(VALUE xslt) {
  return(ruby_xslt_transform_context_new2(cXSLTTransformContext, xslt));
}

#ifdef RDOC_NEVER_DEFINED
  mXML = rb_define_module("XML");
  cXSLT = rb_define_class_under(mXML, "XSLT", rb_cObject);
#endif

void
ruby_init_xslt_transform_context(void) {
  cXSLTTransformContext =
    rb_define_class_under(cXSLT, "TransformContext", rb_cObject);
}
