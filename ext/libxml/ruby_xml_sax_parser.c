/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

#include "ruby_libxml.h"
#include "ruby_xml_sax_parser.h"

/*
 * Document-class: LibXML::XML::SaxParser
 *
 * XML::SaxParser provides a callback based API for parsing documents,
 * in contrast to XML::Parser's tree based API and XML::Reader's stream
 * based API.  
 *
 * Note that the XML::SaxParser API is fairly complex, not well standardized, 
 * and does not directly support validation making entity, namespace and
 * base processing relatively hard.
 *
 * To use the XML::SaxParser, register a callback class via the
 * XML::SaxParser#callbacks=.  It is easiest to include the 
 * XML::SaxParser::Callbacks module in your class and override 
 * the methods as needed.
 *
 * Basic example:
 *
 *   class MyCallbacks
 *     include XML::SaxParser::Callbacks
 *     def on_start_element(element, attributes)
 *       puts #Element started: #{element}"
 *     end
 *   end
 *
 *   parser = XML::SaxParser.new
 *   parser.callbacks = MyCallbacks.new
 *   parser.parse
 */

VALUE cXMLSaxParser;
VALUE mXMLSaxParserCallbacks;

static ID INPUT_ATTR;

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
 *    sax_parser.initialize -> sax_parser
 * 
 * Initiliazes instance of parser.
 */
VALUE
ruby_xml_sax_parser_initialize(VALUE self) {
  VALUE input = rb_class_new_instance(0, NULL, cXMLInput);
  rb_iv_set(self, "@input", input);
  return self;
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
  VALUE source;
  VALUE input = rb_ivar_get(self, INPUT_ATTR);

  Data_Get_Struct(self, ruby_xml_sax_parser, rxsp);

  if (rb_ivar_get(input, FILE_ATTR) != Qnil)
  {
    source = rb_ivar_get(input, FILE_ATTR);
    status = xmlSAXUserParseFile(rxsp->xsh, rxsp, StringValuePtr(source));
  }
  else if (rb_ivar_get(input, STRING_ATTR) != Qnil)
  {
    source = rb_ivar_get(input, STRING_ATTR);
    str = StringValueCStr(source);
    status = xmlSAXUserParseMemory(rxsp->xsh, rxsp, str, strlen(str));
  }
  else
  {
    rb_raise(rb_eArgError, "You must specify a parser data source");
  }
  
  if (status)
  {
    ruby_xml_raise(&xmlLastError);
    return Qfalse;
  }
  else
  {
    return(Qtrue);
  }
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
  rb_define_method(cXMLSaxParser, "callbacks", ruby_xml_sax_parser_callbacks_get, 0);
  rb_define_method(cXMLSaxParser, "callbacks=", ruby_xml_sax_parser_callbacks_set, 1);
  rb_define_method(cXMLSaxParser, "parse", ruby_xml_sax_parser_parse, 0);

  /* Atributes */
  rb_define_attr(cXMLSaxParser, "input", 1, 0);

  /* Instance Methods */
  rb_define_method(cXMLSaxParser, "initialize", ruby_xml_sax_parser_initialize, 0);


  INPUT_ATTR = rb_intern("@input");

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
