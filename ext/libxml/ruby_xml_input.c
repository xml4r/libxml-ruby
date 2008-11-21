/* $Id: rxml_input.c 528 2008-11-15 23:43:48Z cfis $ */

/* Please see the LICENSE file for copyright and distribution information */

#include <stdarg.h>
#include "ruby_libxml.h"

VALUE cXMLInput;

/*
* Document-class: LibXML::XML::Input
*
* Input is a helper class that defines a libxml data source.
* Libxml can parse files, strings, io streams and documents
* accessible via networking protocols such as http.
* Be default, the ruby-libxml bindings expose parsing
* files, io streams and strings.
*
* Generally you will not directly work with the input object,
* but instead will use the various Document and Parser apis.
* For example:
*
*   document = Document.file('my_file') */

ID ENCODING_ATTR;
ID FILE_ATTR;
ID STRING_ATTR;
ID DOCUMENT_ATTR;
ID IO_ATTR;

/*
 * call-seq:
 *    initialize -> LibXML::XML::Input instance
 * 
 * Initialize a new intput object.
 */
static VALUE
rxml_input_initialize(VALUE self) {
  rb_ivar_set(self, ENCODING_ATTR, INT2NUM(XML_CHAR_ENCODING_UTF8));
  return self;
}


/*
 * call-seq:
 *    input.FILE -> "FILE"
 * 
 * Obtain the FILE this parser will read from.
 */
static VALUE
rxml_input_file_get(VALUE self) {
  return rb_ivar_get(self, FILE_ATTR);
}

/*
 * call-seq:
 *    input.FILE = "FILE"
 * 
 * Set the FILE this parser will read from.
 */
static VALUE
rxml_input_file_set(VALUE self, VALUE FILE) {
  Check_Type(FILE, T_STRING);
  rb_ivar_set(self, FILE_ATTR, FILE);
  rb_ivar_set(self, STRING_ATTR, Qnil);
  rb_ivar_set(self, DOCUMENT_ATTR, Qnil);
  rb_ivar_set(self, IO_ATTR, Qnil);
  return self;
}


/*
 * call-seq:
 *    input.string -> "string"
 * 
 * Obtain the string this parser will read from.
 */
static VALUE
rxml_input_string_get(VALUE self) {
  return rb_ivar_get(self, STRING_ATTR);
}


/*
 * call-seq:
 *    input.string = "string"
 * 
 * Set the string this parser will read from.
 */
static VALUE
rxml_input_string_set(VALUE self, VALUE string) {
  Check_Type(string, T_STRING);
  rb_ivar_set(self, FILE_ATTR, Qnil);
  rb_ivar_set(self, STRING_ATTR, string);
  rb_ivar_set(self, DOCUMENT_ATTR, Qnil);
  rb_ivar_set(self, IO_ATTR, Qnil);
  return self;
}

/*
 * call-seq:
 *    input.document -> "document"
 * 
 * Obtain the document this parser will read from.
 */
static VALUE
rxml_input_document_get(VALUE self) {
  return rb_ivar_get(self, DOCUMENT_ATTR);
}

/*
 * call-seq:
 *    input.document = LibXML::XML::Document.new
 * 
 * Set the document this parser will read from.
 */
static VALUE
rxml_input_document_set(VALUE self, VALUE document) {
  rb_ivar_set(self, FILE_ATTR, Qnil);
  rb_ivar_set(self, STRING_ATTR, Qnil);
  rb_ivar_set(self, DOCUMENT_ATTR, document);
  rb_ivar_set(self, IO_ATTR, Qnil);
  return self;
}

/*
 * call-seq:
 *    input.io -> IO
 * 
 * Obtain the IO instance this parser works with.
 */
static VALUE
rxml_input_io_get(VALUE self) {
  return rb_ivar_get(self, IO_ATTR);
}

/*
 * call-seq:
 *    input.io = IO
 * 
 * Set the IO instance this parser works with.
 */
static VALUE
rxml_input_io_set(VALUE self, VALUE io) {
  if (!rb_obj_is_kind_of(io, rb_cIO))
    rb_raise(rb_eTypeError, "Invalid argument, must be an IO object");

  rb_ivar_set(self, FILE_ATTR, Qnil);
  rb_ivar_set(self, STRING_ATTR, Qnil);
  rb_ivar_set(self, DOCUMENT_ATTR, Qnil);
  rb_ivar_set(self, IO_ATTR, io);
  return self;
}

// Rdoc needs to know 
#ifdef RDOC_NEVER_DEFINED
  mLibXML = rb_define_module("LibXML");
  mXML = rb_define_module_under(mLibXML, "XML");
#endif

void
ruby_init_xml_input(void) {	
  FILE_ATTR = rb_intern("@FILE");
  STRING_ATTR = rb_intern("@string");
  DOCUMENT_ATTR = rb_intern("@document");
  IO_ATTR = rb_intern("@io");

  cXMLInput = rb_define_class_under(mXML, "Input", rb_cObject);
  rb_define_attr(cXMLInput, "encoding", 1, 1);
  rb_define_method(cXMLInput, "initialize", rxml_input_initialize, 0);
  rb_define_method(cXMLInput, "file", rxml_input_file_get, 0);
  rb_define_method(cXMLInput, "file=", rxml_input_file_set, 1);
  rb_define_method(cXMLInput, "string", rxml_input_string_get, 0);
  rb_define_method(cXMLInput, "string=", rxml_input_string_set, 1);
  rb_define_method(cXMLInput, "document", rxml_input_document_get, 0);
  rb_define_method(cXMLInput, "document=", rxml_input_document_set, 1);
  rb_define_method(cXMLInput, "io", rxml_input_io_get, 0);
  rb_define_method(cXMLInput, "io=", rxml_input_io_set, 1);
}
