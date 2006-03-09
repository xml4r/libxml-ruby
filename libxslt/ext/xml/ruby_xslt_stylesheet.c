/* $Id$ */

/* See the LICENSE file for copyright and distribution information. */

#include "libxslt.h"
#include "ruby_xslt_stylesheet.h"

VALUE cXSLTStylesheet;

/* call-seq: 
 *   sheet.apply
 */
VALUE
ruby_xslt_stylesheet_apply(int argc, VALUE *argv, VALUE self) {
  ruby_xslt_stylesheet *xss;
  ruby_xml_document *rxd;
  const char **params;
  VALUE parameter, tmp;
  int i, len;

  Data_Get_Struct(self, ruby_xslt_stylesheet, xss);

  if (NIL_P(xss->xml_doc_obj))
    rb_raise(rb_eArgError, "Need a document object");

  Data_Get_Struct(xss->xml_doc_obj, ruby_xml_document, rxd);

  params = NULL;

  switch(argc) {
  case 0:
    break;
  case 1:
    parameter = argv[0];
#if RUBY_VERSION_CODE >= 180
    if (TYPE(parameter) == T_HASH) {
      /* Convert parameter to an array */
      parameter = rb_hash_to_a(parameter);
    }
#endif

    if (TYPE(parameter) == T_ARRAY) {
      /* A hash is better than an array, but we can live with an array of arrays */
      len = RARRAY(parameter)->len;
      params = (void *)ALLOC_N(char *, (len * 2) + 2);
      for (i=0; i < RARRAY(parameter)->len; i++) {
	tmp = RARRAY(parameter)->ptr[i];

	Check_Type(tmp, T_ARRAY);
	Check_Type(RARRAY(tmp)->ptr[0], T_STRING);
	Check_Type(RARRAY(tmp)->ptr[1], T_STRING);

	params[2*i] = RSTRING(RARRAY(tmp)->ptr[0])->ptr;
	params[2*i+1] = RSTRING(RARRAY(tmp)->ptr[1])->ptr;
      }
      params[2*i] = params[2*i+1] = 0;
    } else {
      /* I should test to see if the object responds to to_a and to_h before calling this, but oh well */
      rb_raise(rb_eTypeError, "xslt_stylesheet_appy: expecting a hash or an array of arrays as a parameter");
    }

    break;
  default:
    rb_raise(rb_eArgError, "wrong number of arguments (0 or 1)");
  }

  xss->parsed = ruby_xml_document_new(cXMLDocument,
				      xsltApplyStylesheet(xss->xsp,
							  rxd->doc, params));

  if (params) {
    free(params);
  }

  if (xss->parsed == Qnil)
    return(Qfalse);
  else
    return(Qtrue);
}


/* call-seq: 
 *   sheet.debug
 */
VALUE
ruby_xslt_stylesheet_debug(int argc, VALUE *argv, VALUE self) {
#ifdef LIBXML_DEBUG_ENABLED
  OpenFile *fptr;
  VALUE io;
  FILE *out;
  ruby_xml_document *parsed;
  ruby_xslt_stylesheet *xss;

  Data_Get_Struct(self, ruby_xslt_stylesheet, xss);
  if (NIL_P(xss->parsed))
    rb_raise(eXMLXSLTStylesheetRequireParsedDoc, "must have a parsed XML result");

  switch (argc) {
  case 0:
    io = rb_stdout;
    break;
  case 1:
    io = argv[0];
    if (rb_obj_is_kind_of(io, rb_cIO) == Qfalse)
      rb_raise(rb_eTypeError, "need an IO object");
    break;
  default:
    rb_raise(rb_eArgError, "wrong number of arguments (0 or 1)");
  }

  Data_Get_Struct(xss->parsed, ruby_xml_document, parsed);
  if (parsed->doc == NULL)
    return(Qnil);

  GetOpenFile(io, fptr);
  rb_io_check_writable(fptr);
  out = GetWriteFile(fptr);
  xmlDebugDumpDocument(out, parsed->doc);
  return(Qtrue);
#else
  rb_warn("libxml/libxslt was compiled without debugging support.  Please recompile libxml/libxslt and their Ruby modules");
  return(Qfalse);
#endif
}


void
ruby_xslt_stylesheet_free(ruby_xslt_stylesheet *xss) {
  if (xss->xsp != NULL) {
    xsltFreeStylesheet(xss->xsp);
    xss->xsp = NULL;
  }

  free(xss);
}


void
ruby_xslt_stylesheet_mark(ruby_xslt_stylesheet *xss) {
  if (!NIL_P(xss->parsed))      rb_gc_mark(xss->parsed);
  if (!NIL_P(xss->xml_doc_obj)) rb_gc_mark(xss->xml_doc_obj);

  switch (xss->data_type) {
  case RUBY_LIBXSLT_SRC_TYPE_FILE:
    if (xss->data != NULL)
      rb_gc_mark((VALUE)xss->data);
    break;
  }
}


VALUE
ruby_xslt_stylesheet_new(VALUE class, xsltStylesheetPtr xsp) {
  ruby_xslt_stylesheet *xss;

  xss = ALLOC(ruby_xslt_stylesheet);
  xss->xsp = xsp;
  xss->xml_doc_obj = Qnil;
  xss->parsed = Qnil;
  xss->data_type = RUBY_LIBXSLT_SRC_TYPE_NULL;
  xss->data = NULL;

  return(Data_Wrap_Struct(cXSLTStylesheet, ruby_xslt_stylesheet_mark,
			  ruby_xslt_stylesheet_free, xss));
}


/* call-seq: 
 *   sheet.print
 */
VALUE
ruby_xslt_stylesheet_print(int argc, VALUE *argv, VALUE self) {
  OpenFile *fptr;
  VALUE io;
  FILE *out;
  ruby_xml_document *parsed;
  ruby_xslt_stylesheet *xss;
  int bytes;

  Data_Get_Struct(self, ruby_xslt_stylesheet, xss);
  if (NIL_P(xss->parsed))
    rb_raise(eXMLXSLTStylesheetRequireParsedDoc, "must have a parsed XML result");

  switch (argc) {
  case 0:
    io = rb_stdout;
    break;
  case 1:
    io = argv[0];
    if (rb_obj_is_kind_of(io, rb_cIO) == Qfalse)
      rb_raise(rb_eTypeError, "need an IO object");
    break;
  default:
    rb_raise(rb_eArgError, "wrong number of arguments (0 or 1)");
  }

  Data_Get_Struct(xss->parsed, ruby_xml_document, parsed);
  if (parsed->doc == NULL)
    return(Qnil);

  GetOpenFile(io, fptr);
  rb_io_check_writable(fptr);
  out = GetWriteFile(fptr);
  bytes = xsltSaveResultToFile(out, parsed->doc, xss->xsp);

  return(INT2NUM(bytes));
}

/* call-seq: 
 *   sheet.to_s
 */
VALUE
ruby_xslt_stylesheet_to_s(VALUE self) {
  ruby_xml_document *parsed;
  ruby_xslt_stylesheet *xss;
  xmlChar *str;
  int len;

  Data_Get_Struct(self, ruby_xslt_stylesheet, xss);
  if (NIL_P(xss->parsed))
    rb_raise(eXMLXSLTStylesheetRequireParsedDoc, "must have a parsed XML result");
  Data_Get_Struct(xss->parsed, ruby_xml_document, parsed);
  if (parsed->doc == NULL)
    return(Qnil);

  xsltSaveResultToString(&str, &len, parsed->doc, xss->xsp);
  if (str == NULL)
    return(Qnil);
  else
    return(rb_str_new((const char*)str,len));
}

/* call-seq: 
 *   sheet.save
 */
VALUE
ruby_xslt_stylesheet_save(VALUE self, VALUE io) {
  ruby_xml_document *parsed;
  ruby_xslt_stylesheet *xss;
  OpenFile *fptr;

  if (rb_obj_is_kind_of(io, rb_cIO) == Qfalse)
    rb_raise(rb_eArgError, "Only accept IO objects for saving");

  GetOpenFile(io, fptr);

  Data_Get_Struct(self, ruby_xslt_stylesheet, xss);
  Data_Get_Struct(xss->parsed, ruby_xml_document, parsed);

  xsltSaveResultToFile(fptr->f, parsed->doc, xss->xsp);

  return(Qtrue);
}

#ifdef RDOC_NEVER_DEFINED
  mXML = rb_define_module("XML");
  cXSLT = rb_define_class_under(mXML, "XSLT", rb_cObject);
#endif

void
ruby_init_xslt_stylesheet(void) {
  cXSLTStylesheet = rb_define_class_under(cXSLT, "Stylesheet", rb_cObject);
  eXMLXSLTStylesheetRequireParsedDoc =
    rb_define_class_under(cXSLTStylesheet, "RequireParsedDoc", rb_eException);

  rb_define_method(cXSLTStylesheet, "apply", ruby_xslt_stylesheet_apply, -1);
  rb_define_method(cXSLTStylesheet, "debug", ruby_xslt_stylesheet_debug, -1);
  rb_define_method(cXSLTStylesheet, "print", ruby_xslt_stylesheet_print, -1);
  rb_define_method(cXSLTStylesheet, "to_s", ruby_xslt_stylesheet_to_s, 0);
  rb_define_method(cXSLTStylesheet, "save", ruby_xslt_stylesheet_save, 1);
}
