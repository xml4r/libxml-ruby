/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

#include "libxml.h"
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
  /* Apparently this isn't needed: time will tell */
  /* if (rxsp->xsh != NULL) */
  /* xmlFreeSax_Parser(rxsp->sax_parser); */
}

void
ruby_xml_sax_parser_mark(ruby_xml_sax_parser *rxsp) {
  if (rxsp->callbackHandler && (rxsp->callbackHandler != Qnil)) {
    rb_gc_mark(rxsp->callbackHandler);
  }
  
  if (rxsp->filename && (rxsp->filename != Qnil)) {
    rb_gc_mark(rxsp->filename);
  }

  if (rxsp->str && (rxsp->str != Qnil)) {
    rb_gc_mark(rxsp->str);
  }
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
  rxsp->xsh = &rubySAXHandlerStruct;

  rxsp->callbackHandler = Qnil;  
  rxsp->xpc = NULL;
  rxsp->filename = Qnil;
  rxsp->str = Qnil;

  return(Data_Wrap_Struct(class, ruby_xml_sax_parser_mark, 
                              ruby_xml_sax_parser_free, rxsp));
}


/*
 * call-seq:
 *    sax_parser.callbacks => #<XML::SaxParser::Callbacks subclass>
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
 * this attribute will usually be an object that extends the the 
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

 
/*
 * call-seq:
 *    parser.parse => (true|false)
 * 
 * Parse the input XML, generating callbacks to the object
 * registered via the +callbacks+ attribute.
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
    str = StringValuePtr(rxsp->str);
    status = //ruby_xml_document_new(cXMLDocument,
				   xmlSAXUserParseMemory(rxsp->xsh, rxsp,
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


/****** SaxCallbacks ******/
/* These are all the same. Bloody Rdoc... */

/*
 * call-seq:
 *    callbacks.on_internal_subset(name, external_id, system_id)
 *
 * Called for an internal subset event.
 */
VALUE
ruby_xml_sax_callbacks_on_internal_subset(int argc, VALUE *argv, VALUE self) {
  return Qnil;
}
 
 
/*
 * call-seq:
 *    callbacks.on_is_standalone()
 *
 * Called for 'is standalone' event.
 */
VALUE
ruby_xml_sax_callbacks_on_is_standalone(int argc, VALUE *argv, VALUE self) {
  return Qnil;
}
 
 
/*
 * call-seq:
 *    callbacks.on_has_internal_subset()
 *
 * Called for an internal subset notification event.
 */
VALUE
ruby_xml_sax_callbacks_on_has_internal_subset(int argc, VALUE *argv, VALUE self) {
  return Qnil;
}
 
 
/*
 * call-seq:
 *    callbacks.on_has_external_subset()
 *
 * Called for an external subset notification event.
 */
VALUE
ruby_xml_sax_callbacks_on_has_external_subset(int argc, VALUE *argv, VALUE self) {
  return Qnil;
}
 
 
/*
 * call-seq:
 *    callbacks.on_start_document()
 *
 * Called for a start document event.
 */
VALUE
ruby_xml_sax_callbacks_on_start_document(int argc, VALUE *argv, VALUE self) {
  return Qnil;
}
 
 
/*
 * call-seq:
 *    callbacks.on_end_document()
 *
 * Called for an end document event.
 */
VALUE
ruby_xml_sax_callbacks_on_end_document(int argc, VALUE *argv, VALUE self) {
  return Qnil;
}
 
 
/*
 * call-seq:
 *    callbacks.on_start_element(name, attr_hash)
 * 
 * Called for an element start event.
 */
VALUE
ruby_xml_sax_callbacks_on_start_element(int argc, VALUE *argv, VALUE self) {
  return Qnil;
}
 
 
/*
 * call-seq:
 *    callbacks.on_end_element(name)
 *
 * Called for an element end event.
 */
VALUE
ruby_xml_sax_callbacks_on_end_element(int argc, VALUE *argv, VALUE self) {
  return Qnil;
}
 
 
/*
 * call-seq:
 *    callbacks.on_reference(name)
 *
 * Called for a reference event.
 */
VALUE
ruby_xml_sax_callbacks_on_reference(int argc, VALUE *argv, VALUE self) {
  return Qnil;
}
 
 
/*
 * call-seq:
 *    callbacks.on_characters(chars)
 *
 * Called for a characters event.
 */
VALUE
ruby_xml_sax_callbacks_on_characters(int argc, VALUE *argv, VALUE self) {
  return Qnil;
}
 
 
/*
 * call-seq:
 *    callbacks.on_processing_instruction(target, data)
 *
 * Called for an processing instruction event.
 */
VALUE
ruby_xml_sax_callbacks_on_processing_instruction(int argc, VALUE *argv, VALUE self) {
  return Qnil;
}
 
 
/*
 * call-seq:
 *    callbacks.on_comment(msg)
 *
 * Called for a comment event.
 */
VALUE
ruby_xml_sax_callbacks_on_comment(int argc, VALUE *argv, VALUE self) {
  return Qnil;
}


/*
 * call-seq:
 *    callbacks.on_parser_warning(msg)
 *
 * Called for parser warnings.
 */
VALUE
ruby_xml_sax_callbacks_on_parser_warning(int argc, VALUE *argv, VALUE self) {
  return Qnil;
}
 
 
/*
 * call-seq:
 *    callbacks.on_parser_error(msg)
 *
 * Called for parser errors.
 */
VALUE
ruby_xml_sax_callbacks_on_parser_error(int argc, VALUE *argv, VALUE self) {
  return Qnil;
}
 
  
/*
 * call-seq:
 *    callbacks.on_parser_fatal_error(msg)
 *
 * Called for fatal parser errors.
 */
VALUE
ruby_xml_sax_callbacks_on_parser_fatal_error(int argc, VALUE *argv, VALUE self) {
  return Qnil;
}
 
/*
 * call-seq:
 *    callbacks.on_cdata_block(cdata)
 *
 * Called for a CDATA block event.
 */
VALUE
ruby_xml_sax_callbacks_on_cdata_block(int argc, VALUE *argv, VALUE self) {
  return Qnil;
}
 
/*
 * call-seq:
 *    callbacks.on_external_subset(name, external_id, system_id)
 *
 * Called for an external subset event.
 */
VALUE
ruby_xml_sax_callbacks_on_external_subset(int argc, VALUE *argv, VALUE self) {
  return Qnil;
}


// Rdoc needs to know 
#ifdef RDOC_NEVER_DEFINED
  mXML = rb_define_module("XML");
#endif

void
ruby_init_xml_sax_parser(void) {
  cXMLSaxParser = rb_define_class_under(mXML, "SaxParser", rb_cObject);
  mXMLSaxParserCallbacks = rb_define_module_under(cXMLSaxParser, "Callbacks");

  /* SaxParser */
  rb_define_singleton_method(cXMLSaxParser, "new", ruby_xml_sax_parser_new, 0);

  rb_define_method(cXMLSaxParser, "filename",
       ruby_xml_sax_parser_filename_get, 0);
  rb_define_method(cXMLSaxParser, "filename=",
       ruby_xml_sax_parser_filename_set, 1);
  rb_define_method(cXMLSaxParser, "callbacks",
       ruby_xml_sax_parser_callbacks_get, 0);
  rb_define_method(cXMLSaxParser, "callbacks=",
       ruby_xml_sax_parser_callbacks_set, 1);
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

  rb_define_method(mXMLSaxParserCallbacks, "on_internal_subset", 
      ruby_xml_sax_callbacks_on_internal_subset, -1);        
  rb_define_method(mXMLSaxParserCallbacks, "on_is_standalone", 
      ruby_xml_sax_callbacks_on_is_standalone, -1);        
  rb_define_method(mXMLSaxParserCallbacks, "on_has_internal_subset", 
      ruby_xml_sax_callbacks_on_has_internal_subset, -1);        
  rb_define_method(mXMLSaxParserCallbacks, "on_has_external_subset", 
      ruby_xml_sax_callbacks_on_has_external_subset, -1);        
  rb_define_method(mXMLSaxParserCallbacks, "on_start_document", 
      ruby_xml_sax_callbacks_on_start_document, -1);  
  rb_define_method(mXMLSaxParserCallbacks, "on_end_document", 
      ruby_xml_sax_callbacks_on_end_document, -1);  
  rb_define_method(mXMLSaxParserCallbacks, "on_start_element", 
      ruby_xml_sax_callbacks_on_start_element, -1);  
  rb_define_method(mXMLSaxParserCallbacks, "on_end_element", 
      ruby_xml_sax_callbacks_on_end_element, -1);        
  rb_define_method(mXMLSaxParserCallbacks, "on_reference", 
      ruby_xml_sax_callbacks_on_reference, -1);        
  rb_define_method(mXMLSaxParserCallbacks, "on_characters", 
      ruby_xml_sax_callbacks_on_characters, -1);        
  rb_define_method(mXMLSaxParserCallbacks, "on_processing_instruction", 
      ruby_xml_sax_callbacks_on_processing_instruction, -1);        
  rb_define_method(mXMLSaxParserCallbacks, "on_comment", 
      ruby_xml_sax_callbacks_on_comment, -1);        
  rb_define_method(mXMLSaxParserCallbacks, "on_parser_warning", 
      ruby_xml_sax_callbacks_on_parser_warning, -1);        
  rb_define_method(mXMLSaxParserCallbacks, "on_parser_error", 
      ruby_xml_sax_callbacks_on_parser_error, -1);        
  rb_define_method(mXMLSaxParserCallbacks, "on_parser_fatal_error", 
      ruby_xml_sax_callbacks_on_parser_fatal_error, -1);        
  rb_define_method(mXMLSaxParserCallbacks, "on_cdata_block", 
      ruby_xml_sax_callbacks_on_cdata_block, -1);        
  rb_define_method(mXMLSaxParserCallbacks, "on_external_subset", 
      ruby_xml_sax_callbacks_on_external_subset, -1);              
}
