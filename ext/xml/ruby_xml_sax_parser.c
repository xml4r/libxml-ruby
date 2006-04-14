/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

#include "libxml.h"
#include "ruby_xml_sax_parser.h"

VALUE cXMLSaxParser;
VALUE callsym;

#include "sax_parser_callbacks.inc"

void
ruby_xml_sax_parser_free(ruby_xml_sax_parser *rxsp) {
  /* Apparently this isn't needed: time will tell */
  /* if (rxsp->xsh != NULL) */
  /* xmlFreeSax_Parser(rxsp->sax_parser); */
}

#define mark_handler(rxsp, handler) \
    if (rxsp->cbp->handler && (rxsp->cbp->handler != Qnil)) \
      rb_gc_mark(rxsp->cbp->handler)
  
void
ruby_xml_sax_parser_mark(ruby_xml_sax_parser *rxsp) {
  mark_handler(rxsp, internalSubset);
  mark_handler(rxsp, isStandalone);
  mark_handler(rxsp, hasInternalSubset);
  mark_handler(rxsp, hasExternalSubset);
  mark_handler(rxsp, startDocument);
  mark_handler(rxsp, endDocument);
  mark_handler(rxsp, startElement);
  mark_handler(rxsp, endElement);
  mark_handler(rxsp, reference);
  mark_handler(rxsp, characters);
  mark_handler(rxsp, processingInstruction);
  mark_handler(rxsp, comment);
  mark_handler(rxsp, xmlParserWarning);
  mark_handler(rxsp, xmlParserError);
  mark_handler(rxsp, xmlParserFatalError);
  mark_handler(rxsp, cdataBlock);
}


/*
 * call-seq:
 *    XML::SaxParser.new => sax_parser
 * 
 * Create a new XML::SaxParser instance.
 */
VALUE
ruby_xml_sax_parser_new(VALUE class) {
  ruby_xml_sax_parser *rxsp;
  
  rxsp = ALLOC(ruby_xml_sax_parser);
  rxsp->cbp = ALLOC(ruby_xml_sax_parser_callbacks);
  memset(rxsp->cbp, 0, sizeof(ruby_xml_sax_parser_callbacks));
  rxsp->xsh = &rubySAXHandlerStruct;
  
  rxsp->xpc = NULL;
  rxsp->filename = Qnil;
  rxsp->str = Qnil;

  return(Data_Wrap_Struct(class, ruby_xml_sax_parser_mark, 
                              ruby_xml_sax_parser_free, rxsp));
}


/*
 * call-seq:
 *    sax_parser.filename => "filename"
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

#define set_handler(self, argc, argv, handler)      \
  VALUE proc;                                       \
  rb_scan_args(argc, argv, "0&", &proc);            \
  ruby_xml_sax_parser *rxsp;                        \
  Data_Get_Struct(self, ruby_xml_sax_parser, rxsp); \
  rxsp->cbp->handler = proc;                        \
  return(Qnil);


/*
 * call-seq:
 *    parser.on_internal_subset { |name, external_id, system_id| ... } => nil
 *
 * Set the callback block for an internal subset event.
 */
VALUE
ruby_xml_sax_parser_on_internal_subset(int argc, VALUE *argv, VALUE self) {
  set_handler(self, argc, argv, internalSubset);
}
 
 
/*
 * call-seq:
 *    parser.on_is_standalone { || ... } => nil
 *
 * Set the callback proc for 'is standalone' event.
 */
VALUE
ruby_xml_sax_parser_on_is_standalone(int argc, VALUE *argv, VALUE self) {
  set_handler(self, argc, argv, isStandalone);
}
 
 
/*
 * call-seq:
 *    parser.on_has_internal_subset { || ... } => nil
 *
 * Set the callback proc for an internal subset notification event.
 */
VALUE
ruby_xml_sax_parser_on_has_internal_subset(int argc, VALUE *argv, VALUE self) {
  set_handler(self, argc, argv, hasInternalSubset);
}
 
 
/*
 * call-seq:
 *    parser.on_has_external_subset { || ... } => nil
 *
 * Set the callback proc for an external subset notification event.
 */
VALUE
ruby_xml_sax_parser_on_has_external_subset(int argc, VALUE *argv, VALUE self) {
  set_handler(self, argc, argv, hasExternalSubset);
}
 
 
/*
 * call-seq:
 *    parser.on_start_document { || ... } => nil
 *
 * Set the callback proc for a start document event.
 */
VALUE
ruby_xml_sax_parser_on_start_document(int argc, VALUE *argv, VALUE self) {
  set_handler(self, argc, argv, startDocument);
}
 
 
/*
 * call-seq:
 *    parser.on_end_document { || ... } => nil
 *
 * Set the callback proc for an end document event.
 */
VALUE
ruby_xml_sax_parser_on_end_document(int argc, VALUE *argv, VALUE self) {
  set_handler(self, argc, argv, endDocument);
}
 
 
/*
 * call-seq:
 *    parser.on_start_element { |name, attr_hash| ... } => nil
 * 
 * Set the callback proc for an element start event.
 */
VALUE
ruby_xml_sax_parser_on_start_element(int argc, VALUE *argv, VALUE self) {
  set_handler(self, argc, argv, startElement);
}
 
 
/*
 * call-seq:
 *    parser.on_end_element { |name| ... } => nil
 *
 * Set the callback proc for an element end event.
 */
VALUE
ruby_xml_sax_parser_on_end_element(int argc, VALUE *argv, VALUE self) {
  set_handler(self, argc, argv, endElement);
}
 
 
/*
 * call-seq:
 *    parser.on_reference { |name| ... } => nil
 *
 * Set the callback proc for a reference event.
 */
VALUE
ruby_xml_sax_parser_on_reference(int argc, VALUE *argv, VALUE self) {
  set_handler(self, argc, argv, reference);
}
 
 
/*
 * call-seq:
 *    parser.on_characters { |chars| ... } => nil
 *
 * Set the callback proc for a characters event.
 */
VALUE
ruby_xml_sax_parser_on_characters(int argc, VALUE *argv, VALUE self) {
  set_handler(self, argc, argv, characters);
}
 
 
/*
 * call-seq:
 *    parser.on_processing_instruction { |target, data| ... } => nil
 *
 * Set the callback proc for an processing instruction event.
 */
VALUE
ruby_xml_sax_parser_on_processing_instruction(int argc, VALUE *argv, VALUE self) {
  set_handler(self, argc, argv, processingInstruction);
}
 
 
/*
 * call-seq:
 *    parser.on_comment { |msg| ... } => nil
 *
 * Set the callback proc for a comment event.
 */
VALUE
ruby_xml_sax_parser_on_comment(int argc, VALUE *argv, VALUE self) {
  set_handler(self, argc, argv, comment);
}


/*
 * call-seq:
 *    parser.on_parser_warning { |msg| ... } => nil
 *
 * Set the callback proc that receives parser warnings.
 */
VALUE
ruby_xml_sax_parser_on_parser_warning(int argc, VALUE *argv, VALUE self) {
  set_handler(self, argc, argv, xmlParserWarning);
}
 
 
/*
 * call-seq:
 *    parser.on_parser_error { |msg| ... } => nil
 *
 * Set the callback proc that receives parser errors.
 */
VALUE
ruby_xml_sax_parser_on_parser_error(int argc, VALUE *argv, VALUE self) {
  set_handler(self, argc, argv, xmlParserError);
}
 
  
/*
 * call-seq:
 *    parser.on_parser_fatal_error { |msg| ... } => nil
 *
 * Set the callback proc that receives fatal parser errors.
 */
VALUE
ruby_xml_sax_parser_on_parser_fatal_error(int argc, VALUE *argv, VALUE self) {
  set_handler(self, argc, argv, xmlParserFatalError);
}
 
/*
 * call-seq:
 *    parser.on_cdata_block { |cdata| ... } => nil
 *
 * Set the callback proc for a CDATA block event.
 */
VALUE
ruby_xml_sax_parser_on_cdata_block(int argc, VALUE *argv, VALUE self) {
  set_handler(self, argc, argv, cdataBlock);
}
 
/*
 * call-seq:
 *    parser.on_external_subset { |name, external_id, system_id| ... } => nil
 *
 * Set the callback proc for an external subset event.
 */
VALUE
ruby_xml_sax_parser_on_external_subset(int argc, VALUE *argv, VALUE self) {
  set_handler(self, argc, argv, externalSubset);
}
 
  
/*
 * call-seq:
 *    parser.parse => (true|false)
 * 
 * Parse the input XML, generating callbacks to the procs
 * registered with the parser (via the on_xxxx attributes).
 */
VALUE
ruby_xml_sax_parser_parse(VALUE self) {
  char *str;
  int status = 1;
  ruby_xml_sax_parser *rxsp;

  Data_Get_Struct(self, ruby_xml_sax_parser, rxsp);

  if (rxsp->filename != Qnil) {
    status = xmlSAXUserParseFile(rxsp->xsh, rxsp->cbp, StringValuePtr(rxsp->filename));
  } else if (rxsp->str != Qnil) {
    str = StringValuePtr(rxsp->str);
    status = //ruby_xml_document_new(cXMLDocument,
				   xmlSAXUserParseMemory(rxsp->xsh, rxsp->cbp,
                 str, strlen(str)); //);
  }
  
  /* XXX This should return an exception for the various error codes
   * that can come back in status, but I'm too lazy to do that right
   * now. */
  if (status)
    return(Qfalse);
  else
    return(Qtrue);
}


/*
 * call-seq:
 *    parser.string => "xml"
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
  mXML = rb_define_module("XML");
#endif

void
ruby_init_xml_sax_parser(void) {
  cXMLSaxParser = rb_define_class_under(mXML, "SaxParser", rb_cObject);
  callsym = rb_intern("call");

  rb_define_singleton_method(cXMLSaxParser, "new", ruby_xml_sax_parser_new, 0);

  rb_define_method(cXMLSaxParser, "filename",
		   ruby_xml_sax_parser_filename_get, 0);
  rb_define_method(cXMLSaxParser, "filename=",
		   ruby_xml_sax_parser_filename_set, 1);
  rb_define_method(cXMLSaxParser, "parse", ruby_xml_sax_parser_parse, 0);
  rb_define_method(cXMLSaxParser, "string", ruby_xml_sax_parser_str_get, 0);
  rb_define_method(cXMLSaxParser, "string=", ruby_xml_sax_parser_str_set, 1);

  rb_define_method(cXMLSaxParser, "on_internal_subset", 
      ruby_xml_sax_parser_on_internal_subset, -1);        
  rb_define_method(cXMLSaxParser, "on_is_standalone", 
      ruby_xml_sax_parser_on_is_standalone, -1);        
  rb_define_method(cXMLSaxParser, "on_has_internal_subset", 
      ruby_xml_sax_parser_on_has_internal_subset, -1);        
  rb_define_method(cXMLSaxParser, "on_has_external_subset", 
      ruby_xml_sax_parser_on_has_external_subset, -1);        
  rb_define_method(cXMLSaxParser, "on_start_document", 
      ruby_xml_sax_parser_on_start_document, -1);  
  rb_define_method(cXMLSaxParser, "on_end_document", 
      ruby_xml_sax_parser_on_end_document, -1);  
  rb_define_method(cXMLSaxParser, "on_start_element", 
      ruby_xml_sax_parser_on_start_element, -1);  
  rb_define_method(cXMLSaxParser, "on_end_element", 
      ruby_xml_sax_parser_on_end_element, -1);        
  rb_define_method(cXMLSaxParser, "on_reference", 
      ruby_xml_sax_parser_on_reference, -1);        
  rb_define_method(cXMLSaxParser, "on_characters", 
      ruby_xml_sax_parser_on_characters, -1);        
  rb_define_method(cXMLSaxParser, "on_processing_instruction", 
      ruby_xml_sax_parser_on_processing_instruction, -1);        
  rb_define_method(cXMLSaxParser, "on_comment", 
      ruby_xml_sax_parser_on_comment, -1);        
  rb_define_method(cXMLSaxParser, "on_parser_warning", 
      ruby_xml_sax_parser_on_parser_warning, -1);        
  rb_define_method(cXMLSaxParser, "on_parser_error", 
      ruby_xml_sax_parser_on_parser_error, -1);        
  rb_define_method(cXMLSaxParser, "on_parser_fatal_error", 
      ruby_xml_sax_parser_on_parser_fatal_error, -1);        
  rb_define_method(cXMLSaxParser, "on_cdata_block", 
      ruby_xml_sax_parser_on_cdata_block, -1);        
  rb_define_method(cXMLSaxParser, "on_external_subset", 
      ruby_xml_sax_parser_on_external_subset, -1);              
}
