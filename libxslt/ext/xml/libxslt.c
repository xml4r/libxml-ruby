/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

#include "libxslt.h"
#include "libxml/xmlversion.h"

VALUE cXSLT;
VALUE eXMLXSLTStylesheetRequireParsedDoc;

VALUE
ruby_xslt_doc_get(VALUE self) {
  ruby_xslt *rxslt;
  Data_Get_Struct(self, ruby_xslt, rxslt);
  return(rxslt->xml_doc_obj);
}


VALUE
ruby_xslt_doc_set(VALUE self, VALUE xml_doc_obj) {
  ruby_xslt *rxslt;
  ruby_xml_document *rxd;
  ruby_xslt_transform_context *rxtc;

  if (rb_obj_is_kind_of(xml_doc_obj, cXMLDocument) == Qfalse)
    rb_raise(rb_eTypeError, "Invalid argument: must be of type XML::Document");

  Data_Get_Struct(self, ruby_xslt, rxslt);
  rxslt->xml_doc_obj = xml_doc_obj;

  Data_Get_Struct(xml_doc_obj, ruby_xml_document, rxd);
  if (rxd->doc == NULL)
      return(Qnil);

  rxslt->ctxt = ruby_xslt_transform_context_new3(self);
  Data_Get_Struct(rxslt->ctxt, ruby_xslt_transform_context, rxtc);

  if (rxslt->xsp == NULL || rxd->doc == NULL)
    return(Qnil);

  rxtc->ctxt = xsltNewTransformContext(rxslt->xsp, rxd->doc) ;

  return(rxslt->xml_doc_obj);
}


VALUE
ruby_xslt_filename_get(VALUE self) {
  ruby_xslt *rxslt;
  Data_Get_Struct(self, ruby_xslt, rxslt);

  if (rxslt->data_type != RUBY_LIBXSLT_SRC_TYPE_FILE)
    return(Qnil);
  else
    return((VALUE)rxslt->data);
}


VALUE
ruby_xslt_filename_set(VALUE self, VALUE filename) {
  ruby_xslt *rxslt;
  Check_Type(filename, T_STRING);
  Data_Get_Struct(self, ruby_xslt, rxslt);

  if (rxslt->data_type != RUBY_LIBXSLT_SRC_TYPE_NULL)
    return(Qnil);

  rxslt->data_type = RUBY_LIBXSLT_SRC_TYPE_FILE;
  rxslt->data = (void *)filename;
  return(filename);
}


void
ruby_xslt_free(ruby_xslt *rxslt) {
  if (rxslt != NULL)
    free(rxslt);
}


void
ruby_xslt_mark(ruby_xslt *rxslt) {
  if (rxslt == NULL) return;
  if (!NIL_P(rxslt->ctxt))        rb_gc_mark(rxslt->ctxt);
  if (!NIL_P(rxslt->str))         rb_gc_mark(rxslt->str);
  if (!NIL_P(rxslt->xml_doc_obj)) rb_gc_mark(rxslt->xml_doc_obj);

  switch(rxslt->data_type) {
  case RUBY_LIBXSLT_SRC_TYPE_FILE:
    if (rxslt->data != NULL)
      rb_gc_mark((VALUE)rxslt->data);
    break;
  }
}


VALUE
ruby_xslt_new(VALUE class) {
  ruby_xslt *rxslt;

  rxslt = (ruby_xslt *)malloc(sizeof(ruby_xslt));
  if (rxslt == NULL)
    rb_raise(rb_eNoMemError, "No memory left for XSLT struct");

  rxslt->ctxt = Qnil;
  rxslt->data = NULL;
  rxslt->data_type = RUBY_LIBXSLT_SRC_TYPE_NULL;
  rxslt->str = Qnil;
  rxslt->xml_doc_obj = Qnil;
  rxslt->xsp = NULL;

  return(Data_Wrap_Struct(class, ruby_xslt_mark, ruby_xslt_free, rxslt));
}


VALUE
ruby_xslt_new_file(VALUE class, VALUE filename) {
  VALUE xslt;

  xslt = ruby_xslt_new(class);
  ruby_xslt_filename_set(xslt, filename);
  return(xslt);
}


VALUE
ruby_xslt_parse(VALUE self) {
  ruby_xml_document *rxd;
  ruby_xslt *rxslt;
  ruby_xslt_stylesheet *xss;
  VALUE xssobj;
  xsltStylesheetPtr sheet;

  Data_Get_Struct(self, ruby_xslt, rxslt);

  if (rxslt->data_type == RUBY_LIBXSLT_SRC_TYPE_FILE) {
    /*xssobj = ruby_xslt_stylesheet_new(cXSLTStylesheet,
				      xsltParseStylesheetFile((const xmlChar *)
							      STR2CSTR(rxslt->data)));*/
	sheet = xsltParseStylesheetFile((const xmlChar *) STR2CSTR(rxslt->data));

    if (sheet) {
        xssobj = ruby_xslt_stylesheet_new(cXSLTStylesheet, sheet);
        Data_Get_Struct(xssobj, ruby_xslt_stylesheet, xss);
        xss->data = (void *)rb_obj_dup((VALUE)rxslt->data);
        xss->xml_doc_obj = rxslt->xml_doc_obj;
    } else
        xssobj = Qnil;

  } else if (rxslt->xml_doc_obj != Qnil) {
    Data_Get_Struct(rxslt->xml_doc_obj, ruby_xml_document, rxd);
    /*xssobj = ruby_xslt_stylesheet_new(cXSLTStylesheet,
				      xsltParseStylesheetDoc(rxd->doc));*/
    sheet = xsltParseStylesheetDoc(rxd->doc);
    if (sheet) {
        xssobj = ruby_xslt_stylesheet_new(cXSLTStylesheet,sheet);
        Data_Get_Struct(xssobj, ruby_xslt_stylesheet, xss);
        xss->xml_doc_obj = rxslt->xml_doc_obj;
    } else
        xssobj = Qnil;

  } else {
    xssobj = Qnil;
  }

  return(xssobj);
}


void
Init_libxslt(void) {
  LIBXML_TEST_VERSION;  
  mXML = rb_const_get(rb_cObject, rb_intern("XML"));
  cXMLDocument = rb_const_get(mXML, rb_intern("Document"));

  cXSLT = rb_define_class_under(mXML, "XSLT", rb_cObject);

  rb_define_const(cXSLT, "MAX_DEPTH", INT2NUM(xsltMaxDepth));
  rb_define_const(cXSLT, "MAX_SORT", INT2NUM(XSLT_MAX_SORT));
  rb_define_const(cXSLT, "ENGINE_VERSION", rb_str_new2(xsltEngineVersion));
  rb_define_const(cXSLT, "LIBXSLT_VERSION", INT2NUM(xsltLibxsltVersion));
  rb_define_const(cXSLT, "LIBXML_VERSION", INT2NUM(xsltLibxmlVersion));
  rb_define_const(cXSLT, "XSLT_NAMESPACE", rb_str_new2((const char*)XSLT_NAMESPACE));
  rb_define_const(cXSLT, "DEFAULT_VENDOR", rb_str_new2(XSLT_DEFAULT_VENDOR));
  rb_define_const(cXSLT, "DEFAULT_VERSION", rb_str_new2(XSLT_DEFAULT_VERSION));
  rb_define_const(cXSLT, "DEFAULT_URL", rb_str_new2(XSLT_DEFAULT_URL));
  rb_define_const(cXSLT, "NAMESPACE_LIBXSLT", rb_str_new2((const char*)XSLT_LIBXSLT_NAMESPACE));
  rb_define_const(cXSLT, "NAMESPACE_NORM_SAXON", rb_str_new2((const char*)XSLT_NORM_SAXON_NAMESPACE));
  rb_define_const(cXSLT, "NAMESPACE_SAXON", rb_str_new2((const char*)XSLT_SAXON_NAMESPACE));
  rb_define_const(cXSLT, "NAMESPACE_XT", rb_str_new2((const char*)XSLT_XT_NAMESPACE));
  rb_define_const(cXSLT, "NAMESPACE_XALAN", rb_str_new2((const char*)XSLT_XALAN_NAMESPACE));

  ruby_init_xslt_stylesheet();
  ruby_init_xslt_transform_context();


  rb_define_singleton_method(cXSLT, "file", ruby_xslt_new_file, 1);
  rb_define_singleton_method(cXSLT, "new", ruby_xslt_new, 0);

  rb_define_method(cXSLT, "doc", ruby_xslt_doc_get, 0);
  rb_define_method(cXSLT, "doc=", ruby_xslt_doc_set, 1);
  rb_define_method(cXSLT, "filename", ruby_xslt_filename_get, 0);
  rb_define_method(cXSLT, "filename=", ruby_xslt_filename_set, 1);
  rb_define_method(cXSLT, "parse", ruby_xslt_parse, 0);

  exsltRegisterAll();
}
