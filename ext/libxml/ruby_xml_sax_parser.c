/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

#include "ruby_libxml.h"
#include "ruby_xml_sax_parser.h"

VALUE cXMLSaxParser;
VALUE mXMLSaxParserCallbacks;

VALUE cbidOnInternalSubset;
VALUE cbidOnIsStandalone;
VALUE cbidOnHasInternalSubset;
VALUE cbidOnHasExternalSubset;
VALUE cbidOnStartDocument;
VALUE cbidOnEndDocument;
VALUE cbidOnStartElement;
VALUE cbidOnEndElement;
VALUE cbidOnReference;
VALUE cbidOnCharacters;
VALUE cbidOnProcessingInstruction;
VALUE cbidOnComment;
VALUE cbidOnXmlParserWarning;
VALUE cbidOnXmlParserError;
VALUE cbidOnXmlParserFatalError;
VALUE cbidOnCdataBlock;
VALUE cbidOnExternalSubset;

#include "sax_parser_callbacks.inc"

void
ruby_xml_sax_parser_free(ruby_xml_sax_parser *rxsp) {
  xfree(rxsp);
}

void
ruby_xml_sax_parser_mark(ruby_xml_sax_parser *rxsp) {
  if (rxsp->callbackHandler != Qnil) {
    rb_gc_mark(rxsp->callbackHandler);
  }
  
  if (rxsp->filename != Qnil) {
    rb_gc_mark(rxsp->filename);
  }

  if (rxsp->str != Qnil) {
    rb_gc_mark(rxsp->str);
  }
}

VALUE
ruby_xml_sax_parser_alloc(VALUE klass) {
  ruby_xml_sax_parser *rxsp = ALLOC(ruby_xml_sax_parser);
  rxsp->xsh = &rubySAXHandlerStruct;
  rxsp->callbackHandler = Qnil;  
  rxsp->xpc = NULL;
  rxsp->filename = Qnil;
  rxsp->str = Qnil;

  return Data_Wrap_Struct(cXMLSaxParser, 
                          ruby_xml_sax_parser_mark, ruby_xml_sax_parser_free,
                          rxsp);
}


/*
 * call-seq:
 *    sax_parser.callbacks -> #<XML::SaxParser::Callbacks subclass>
 * 
 * Obtain the callbacks used by this parser.
 */
VALUE
ruby_xml_sax_parser_callbacks_get(VALUE self) {
  ruby_xml_sax_parser *rxsp;
  Data_Get_Struct(self, ruby_xml_sax_parser, rxsp);
  return(rxsp->callbackHandler);
}


/*
 * call-seq:
 *    sax_parser.callbacks = #<XML::SaxParser::Callbacks subclass>
 * 
 * Set the callbacks used by this parser. The value assigned to
 * this attributesibute will usually be an object that extends the the 
 * XML::SaxParser::Callbacks module, overriding the callbacks it
 * wishes to process.
 */
VALUE
ruby_xml_sax_parser_callbacks_set(VALUE self, VALUE callbacks) {
  ruby_xml_sax_parser *rxsp;
  Data_Get_Struct(self, ruby_xml_sax_parser, rxsp);
  rxsp->callbackHandler = callbacks;
  return(rxsp->callbackHandler);
}


/*
 * call-seq:
 *    sax_parser.filename -> "filename"
 * 
 * Obtain the filename this parser reads from.
 */
VALUE
ruby_xml_sax_parser_filename_get(VALUE self) {
  ruby_xml_sax_parser *rxsp;
  Data_Get_Struct(self, ruby_xml_sax_parser, rxsp);
  return(rxsp->filename);
}


/*
 * call-seq:
 *    sax_parser.filename = "filename"
 * 
 * Set the filename this parser reads from.
 */
VALUE
ruby_xml_sax_parser_filename_set(VALUE self, VALUE filename) {
  ruby_xml_sax_parser *rxsp;
  Check_Type(filename, T_STRING);
  Data_Get_Struct(self, ruby_xml_sax_parser, rxsp);
  rxsp->filename = filename;
  return(rxsp->filename);
}

 
/*
 * call-seq:
 *    parser.parse -> (true|false)
 * 
 * Parse the input XML, generating callbacks to the object
 * registered via the +callbacks+ attributesibute.
 */
VALUE
ruby_xml_sax_parser_parse(VALUE self) {
  char *str;
  int status = 1;
  ruby_xml_sax_parser *rxsp;

  Data_Get_Struct(self, ruby_xml_sax_parser, rxsp);

  if (rxsp->filename != Qnil) {
    status = xmlSAXUserParseFile(rxsp->xsh, rxsp, StringValuePtr(rxsp->filename));
  } else if (rxsp->str != Qnil) {
    str = StringValueCStr(rxsp->str);
    status = xmlSAXUserParseMemory(rxsp->xsh, rxsp, str, strlen(str));
  }
  
  /* XXX This should return an exception for the various error codes
   * that can come back in status, but I'm too lazy to do that right
   * now. */
  if (status)
    ruby_xml_raise(&xmlLastError);
  else
    return(Qtrue);
}


/*
 * call-seq:
 *    parser.string -> "xml"
 * 
 * Obtain the parser's input string.
 */
VALUE
ruby_xml_sax_parser_str_get(VALUE self) {
  ruby_xml_sax_parser *rxsp;
  Data_Get_Struct(self, ruby_xml_sax_parser, rxsp);
  return(rxsp->str);
}


/*
 * call-seq:
 *    parser.string = "xml"
 * 
 * Set the parser's input string.
 */
VALUE
ruby_xml_sax_parser_str_set(VALUE self, VALUE str) {
  ruby_xml_sax_parser *rxsp;
  Check_Type(str, T_STRING);
  Data_Get_Struct(self, ruby_xml_sax_parser, rxsp);
  rxsp->str = str;
  return(rxsp->str);
}

// Rdoc needs to know 
#ifdef RDOC_NEVER_DEFINED
  mLibXML = rb_define_module("LibXML");
  mXML = rb_define_module_under(mLibXML, "XML");
#endif

void
ruby_init_xml_sax_parser(void) {
  /* SaxParser */
  cXMLSaxParser = rb_define_class_under(mXML, "SaxParser", rb_cObject);
  rb_define_alloc_func(cXMLSaxParser, ruby_xml_sax_parser_alloc);
  rb_define_method(cXMLSaxParser, "filename", ruby_xml_sax_parser_filename_get, 0);
  rb_define_method(cXMLSaxParser, "filename=", ruby_xml_sax_parser_filename_set, 1);
  rb_define_method(cXMLSaxParser, "callbacks", ruby_xml_sax_parser_callbacks_get, 0);
  rb_define_method(cXMLSaxParser, "callbacks=", ruby_xml_sax_parser_callbacks_set, 1);
  rb_define_method(cXMLSaxParser, "parse", ruby_xml_sax_parser_parse, 0);
  rb_define_method(cXMLSaxParser, "string", ruby_xml_sax_parser_str_get, 0);
  rb_define_method(cXMLSaxParser, "string=", ruby_xml_sax_parser_str_set, 1);

  /* SaxCallbacks */
  cbidOnInternalSubset = rb_intern("on_internal_subset");
  cbidOnIsStandalone = rb_intern("on_is_standalone");
  cbidOnHasInternalSubset = rb_intern("on_has_internal_subset");
  cbidOnHasExternalSubset = rb_intern("on_has_external_subset");
  cbidOnStartDocument = rb_intern("on_start_document");
  cbidOnEndDocument = rb_intern("on_end_document");
  cbidOnStartElement = rb_intern("on_start_element");
  cbidOnEndElement = rb_intern("on_end_element");
  cbidOnReference = rb_intern("on_reference");
  cbidOnCharacters = rb_intern("on_characters");
  cbidOnProcessingInstruction = rb_intern("on_processing_instruction");
  cbidOnComment = rb_intern("on_comment");
  cbidOnXmlParserWarning = rb_intern("on_parser_warning");
  cbidOnXmlParserError = rb_intern("on_parser_error");
  cbidOnXmlParserFatalError = rb_intern("on_parser_fatal_error");
  cbidOnCdataBlock = rb_intern("on_cdata_block");
  cbidOnExternalSubset = rb_intern("on_external_subset");
}
