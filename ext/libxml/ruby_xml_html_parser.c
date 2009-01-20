/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

#include "ruby_libxml.h"

VALUE cXMLHTMLParser;
static ID CONTEXT_ATTR;
static ID INPUT_ATTR;

/*
 * Document-class: LibXML::XML::HTMLParser
 *
 * The HTML parser implements an HTML 4.0 non-verifying parser with an API
 * compatible with the XML::Parser.  In contrast with the XML::Parser,
 * it can parse "real world" HTML, even if it severely broken from a
 * specification point of view. */

/*
 * call-seq:
 *    XML::HTMLParser.initialize -> parser
 *
 * Initializes a new parser instance with no pre-determined source.
 */
static VALUE rxml_html_parser_initialize(VALUE self)
{
  VALUE input = rb_class_new_instance(0, NULL, cXMLInput);
  htmlParserCtxtPtr ctxt = htmlNewParserCtxt();
  VALUE context = rxml_parser_context_wrap(ctxt);

  rb_ivar_set(self, INPUT_ATTR, input);
  rb_ivar_set(self, CONTEXT_ATTR, context);

  return self;
}

static xmlDocPtr rxml_html_parser_document(xmlParserCtxtPtr ctxt, VALUE input)
{
  VALUE document = rb_ivar_get(input, DOCUMENT_ATTR);
  VALUE base_url = rb_ivar_get(input, BASE_URL_ATTR);
  VALUE encoding = rb_ivar_get(input, ENCODING_ATTR);
  VALUE options = rb_ivar_get(input, OPTIONS_ATTR);
  const char* xurl = NIL_P(base_url) ? NULL : StringValueCStr(base_url);

  xmlDocPtr xdoc;
  int indent = 0;
  xmlChar *buffer; 
  int length;

  Data_Get_Struct(document, xmlDoc, xdoc);

  xmlDocDumpFormatMemoryEnc(xdoc,
                            &buffer, &length,
                            xmlGetCharEncodingName((xmlCharEncoding)NUM2INT(encoding)),
                            indent);

  return htmlCtxtReadDoc(ctxt,
                         buffer, 
                         xurl,
                         xmlGetCharEncodingName((xmlCharEncoding)NUM2INT(encoding)),
                         NUM2INT(options));
}

static htmlDocPtr rxml_html_parser_file(xmlParserCtxtPtr ctxt, VALUE input)
{
  VALUE file = rb_ivar_get(input, FILE_ATTR);
  VALUE encoding = rb_ivar_get(input, ENCODING_ATTR);
  VALUE options = rb_ivar_get(input, OPTIONS_ATTR);

  return htmlCtxtReadFile(ctxt, StringValuePtr(file),
                          xmlGetCharEncodingName((xmlCharEncoding)NUM2INT(encoding)),
                          NUM2INT(options));
}

static htmlDocPtr rxml_html_parser_io(xmlParserCtxtPtr ctxt, VALUE input)
{
  VALUE io = rb_ivar_get(input, IO_ATTR);
  VALUE base_url = rb_ivar_get(input, BASE_URL_ATTR);
  VALUE encoding = rb_ivar_get(input, ENCODING_ATTR);
  VALUE options = rb_ivar_get(input, OPTIONS_ATTR);
  const char* xurl = NIL_P(base_url) ? NULL : StringValueCStr(base_url);

  return htmlCtxtReadIO(ctxt,
                        (xmlInputReadCallback) rxml_read_callback, NULL,
                        (void*)io,
                        xurl,
                        xmlGetCharEncodingName((xmlCharEncoding)NUM2INT(encoding)),
                        NUM2INT(options));
}

static htmlDocPtr rxml_html_parser_string(xmlParserCtxtPtr ctxt, VALUE input)
{
  VALUE string = rb_ivar_get(input, STRING_ATTR);
  VALUE base_url = rb_ivar_get(input, BASE_URL_ATTR);
  VALUE encoding = rb_ivar_get(input, ENCODING_ATTR);
  VALUE options = rb_ivar_get(input, OPTIONS_ATTR);
  const char* xurl = NIL_P(base_url) ? NULL : StringValueCStr(base_url);

  return htmlCtxtReadMemory(ctxt,
                            StringValuePtr(string), RSTRING_LEN(string),
                            xurl,
                            xmlGetCharEncodingName((xmlCharEncoding)NUM2INT(encoding)),
                            NUM2INT(options));
}


/*
 * call-seq:
 *    parser.parse -> XML::Document
 *
 * Parse the input XML and create an XML::Document with
 * it's content. If an error occurs, XML::Parser::ParseError
 * is thrown.
 */
static VALUE rxml_html_parser_parse(VALUE self)
{
  VALUE input = rb_ivar_get(self, INPUT_ATTR);
  VALUE context = rb_ivar_get(self, CONTEXT_ATTR);
  htmlParserCtxtPtr ctxt;
  htmlDocPtr xdoc;
  
  Data_Get_Struct(context, xmlParserCtxt, ctxt);
    
  if (rb_ivar_get(input, FILE_ATTR) != Qnil)
    xdoc = rxml_html_parser_file(ctxt, input);
  else if (rb_ivar_get(input, STRING_ATTR) != Qnil)
    xdoc = rxml_html_parser_string(ctxt, input);
  else if (rb_ivar_get(input, DOCUMENT_ATTR) != Qnil)
    xdoc = rxml_html_parser_document(ctxt, input);
  else if (rb_ivar_get(input, IO_ATTR) != Qnil)
    xdoc = rxml_html_parser_io(ctxt, input);
  else
    rb_raise(rb_eArgError, "You must specify a parser data source");

  if (!xdoc)
    rxml_raise(&xmlLastError);

/*  if (!ctxt->wellFormed)
  {
    xmlFreeDoc(ctxt->myDoc);
    rxml_raise(&ctxt->lastError);
  }*/

  return rxml_document_wrap(xdoc);
}

// Rdoc needs to know
#ifdef RDOC_NEVER_DEFINED
mLibXML = rb_define_module("LibXML");
mXML = rb_define_module_under(mLibXML, "XML");
#endif

void ruby_init_html_parser(void)
{
  INPUT_ATTR = rb_intern("@input");
  CONTEXT_ATTR = rb_intern("@context");

  cXMLHTMLParser = rb_define_class_under(mXML, "HTMLParser", rb_cObject);

  /* Atributes */
  rb_define_attr(cXMLHTMLParser, "input", 1, 0);

  /* Instance methods */
  rb_define_method(cXMLHTMLParser, "initialize", rxml_html_parser_initialize, 0);
  rb_define_method(cXMLHTMLParser, "parse", rxml_html_parser_parse, 0);
}
