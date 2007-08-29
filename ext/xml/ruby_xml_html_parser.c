/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

#include "libxml.h"

VALUE cXMLHTMLParser;

//static int
//ctxtRead(FILE *f, char * buf, int len) {
//    return(fread(buf, 1, len, f));
//}


/*
 * call-seq:
 *    parser.filename => "filename"
 * 
 * Obtain the filename this parser will read from.
 */
 /*
VALUE
ruby_xml_html_parser_filename_get(VALUE self) {
  ruby_xml_html_parser *rxp;
  rx_file_data *data;

  Data_Get_Struct(self, ruby_xml_html_parser, rxp);
  if (rxp->data == NULL)
    return(Qnil);

  if (rxp->data_type != RUBY_LIBXML_SRC_TYPE_FILE)
    return(Qnil);

  data = (rx_file_data *)rxp->data;
  return(data->filename);
}
*/

/*
 * call-seq:
 *    parser.filename = "filename"
 * 
 * Set the filename this parser will read from.
 */
/*
VALUE
ruby_xml_html_parser_filename_set(VALUE self, VALUE filename) {
  ruby_xml_html_parser *rxp;
  ruby_xml_parser_context *rxpc;
  rx_file_data *data;

  Check_Type(filename, T_STRING);
  Data_Get_Struct(self, ruby_xml_html_parser, rxp);

  if (rxp->data_type == RUBY_LIBXML_SRC_TYPE_NULL) {
    if (rxp->data != NULL)
      rb_fatal("crap, this should be null");

    rxp->data_type = RUBY_LIBXML_SRC_TYPE_FILE;
    data = ALLOC(rx_file_data);
    rxp->data = data;
  } else if (rxp->data_type != RUBY_LIBXML_SRC_TYPE_FILE) {
    return(Qnil);
  }

  rxp->ctxt = ruby_xml_parser_context_new3();
  data = (rx_file_data *)rxp->data;
  data->filename = filename;

  Data_Get_Struct(rxp->ctxt, ruby_xml_parser_context, rxpc);
  rxpc->ctxt = htmlCreateFileParserCtxt(StringValuePtr(filename));
  if (rxpc->ctxt == NULL)
    rb_sys_fail(StringValuePtr(filename));

  return(data->filename);
}
*/

void
ruby_xml_html_parser_free(ruby_xml_html_parser *rxp) {
  void *data;

  ruby_xml_parser_count--;
  if (ruby_xml_parser_count == 0)
    xmlCleanupParser();  

  switch(rxp->data_type) {
  case RUBY_LIBXML_SRC_TYPE_NULL:
    break;
  case RUBY_LIBXML_SRC_TYPE_FILE:
    data = (void *)(rx_file_data *)rxp->data;
    free((rx_file_data *)data);
    break;
  case RUBY_LIBXML_SRC_TYPE_STRING:
    data = (void *)(rx_string_data *)rxp->data;
    free((rx_string_data *)data);
    break;
  case RUBY_LIBXML_SRC_TYPE_IO:
    data = (void *)(rx_io_data *)rxp->data;
    free((rx_io_data *)data);
    break;
  default:
    rb_fatal("Unknown data type, %d", rxp->data_type);
  }

  free(rxp);
}


/*
 * call-seq:
 *    parser.io => IO
 * 
 * Obtain the IO instance this parser works with.
 */
 /*
VALUE
ruby_xml_html_parser_io_get(VALUE self, VALUE io) {
  ruby_xml_html_parser *rxp;
  rx_io_data *data;

  Data_Get_Struct(self, ruby_xml_html_parser, rxp);

  if (rxp->data_type == RUBY_LIBXML_SRC_TYPE_NULL ||
      rxp->data_type != RUBY_LIBXML_SRC_TYPE_IO ||
      rxp->data == NULL)
    return(Qnil);

  data = (rx_io_data *)rxp->data;

  return(data->io);
}
*/

/*
 * call-seq:
 *    parser.io = IO
 * 
 * Set the IO instance this parser works with.
 */
 /*
VALUE
ruby_xml_html_parser_io_set(VALUE self, VALUE io) {
  ruby_xml_html_parser *rxp;
  ruby_xml_parser_context *rxpc;
  rx_io_data *data;
  OpenFile *fptr;
  FILE *f;

  if (!rb_obj_is_kind_of(io, rb_cIO))
    rb_raise(rb_eTypeError, "need an IO object");

  Data_Get_Struct(self, ruby_xml_html_parser, rxp);

  if (rxp->data_type == RUBY_LIBXML_SRC_TYPE_NULL) {
    if (rxp->data != NULL)
      rb_fatal("crap, this should be null");

    rxp->data_type = RUBY_LIBXML_SRC_TYPE_IO;
    data = ALLOC(rx_io_data);
    rxp->data = data;
  } else if (rxp->data_type != RUBY_LIBXML_SRC_TYPE_IO) {
    return(Qnil);
  }

  rxp->ctxt = ruby_xml_parser_context_new3();
  data = (rx_io_data *)rxp->data;
  data->io = io;

  GetOpenFile(io, fptr);
  rb_io_check_readable(fptr);
  f = GetWriteFile(fptr);

  Data_Get_Struct(rxp->ctxt, ruby_xml_parser_context, rxpc);
  rxpc->ctxt = htmlCreateIOParserCtxt(NULL, NULL,
				     (xmlInputReadCallback) ctxtRead,
				     NULL, f, XML_CHAR_ENCODING_NONE);
  if (NIL_P(rxpc->ctxt))
    rb_sys_fail(0);

  return(data->io);
}
*/

void
ruby_xml_html_parser_mark(ruby_xml_html_parser *rxp) {
  if (rxp == NULL) return;
  if (!NIL_P(rxp->ctxt)) rb_gc_mark(rxp->ctxt);

  switch(rxp->data_type) {
  case RUBY_LIBXML_SRC_TYPE_NULL:
    break;
  case RUBY_LIBXML_SRC_TYPE_FILE:
    if (!NIL_P(((rx_file_data *)rxp->data)->filename))
      rb_gc_mark(((rx_file_data *)rxp->data)->filename);
    break;
  case RUBY_LIBXML_SRC_TYPE_STRING:
    if (!NIL_P(((rx_string_data *)rxp->data)->str))
      rb_gc_mark(((rx_string_data *)rxp->data)->str);
    break;
  case RUBY_LIBXML_SRC_TYPE_IO:
    if (!NIL_P(((rx_io_data *)rxp->data)->io))
      rb_gc_mark(((rx_io_data *)rxp->data)->io);
    break;
  default:
    rb_fatal("unknown datatype: %d", rxp->data_type);
  }
}


/*
 * call-seq:
 *    XML::HTMLParser.new => parser
 * 
 * Create a new parser instance with no pre-determined source.
 */
VALUE
ruby_xml_html_parser_new(VALUE class) {
  ruby_xml_html_parser *rxp;

  ruby_xml_parser_count++;
  rxp = ALLOC(ruby_xml_html_parser);
  rxp->ctxt = Qnil;
  rxp->data_type = RUBY_LIBXML_SRC_TYPE_NULL;
  rxp->data = NULL;
  rxp->parsed = 0;

  return(Data_Wrap_Struct(class, ruby_xml_html_parser_mark,
			  ruby_xml_html_parser_free, rxp));
}


/*
 * call-seq:
 *    XML::HTMLParser.file => parser
 * 
 * Create a new parser instance that will read the specified file.
 */
 /*
VALUE
ruby_xml_html_parser_new_file(VALUE class, VALUE filename) {
  VALUE obj;
  ruby_xml_html_parser *rxp;
  rx_file_data *data;

  obj = ruby_xml_html_parser_new(class);
  Data_Get_Struct(obj, ruby_xml_html_parser, rxp);

  data = ALLOC(rx_file_data);
  rxp->data_type = RUBY_LIBXML_SRC_TYPE_FILE;
  rxp->data = data;

  ruby_xml_html_parser_filename_set(obj, filename);

  return(obj);
}
*/

/*
 * call-seq:
 *    XML::HTMLParser.io => parser
 * 
 * Create a new parser instance that will read from the
 * specified IO object.
 */
 /*
VALUE
ruby_xml_html_parser_new_io(VALUE class, VALUE io) {
  VALUE obj;
  ruby_xml_html_parser *rxp;
  rx_io_data *data;

  obj = ruby_xml_html_parser_new(class);
  Data_Get_Struct(obj, ruby_xml_html_parser, rxp);

  data = ALLOC(rx_io_data);
  rxp->data_type = RUBY_LIBXML_SRC_TYPE_IO;
  rxp->data = data;

  ruby_xml_html_parser_io_set(obj, io);

  return(obj);
}
*/

/*
 * call-seq:
 *    XML::HTMLParser.string => parser
 * 
 * Create a new parser instance that will parse the given
 * string.
 */
VALUE
ruby_xml_html_parser_new_string(VALUE class, VALUE str) {
  VALUE obj;
  ruby_xml_html_parser *rxp;
  rx_string_data *data;

  obj = ruby_xml_html_parser_new(class);
  Data_Get_Struct(obj, ruby_xml_html_parser, rxp);

  data = ALLOC(rx_string_data);
  rxp->data_type = RUBY_LIBXML_SRC_TYPE_STRING;
  rxp->data = data;

  ruby_xml_html_parser_str_set(obj, str);

  return(obj);
}


/*
 * call-seq:
 *    parser.parse => document
 * 
 * Parse the input XML and create an XML::Document with
 * it's content. If an error occurs, XML::Parser::ParseError
 * is thrown.
 */
VALUE
ruby_xml_html_parser_parse(VALUE self) {
  ruby_xml_document_t *rxd;
  ruby_xml_html_parser *rxp;
  ruby_xml_parser_context *rxpc;
  htmlDocPtr xdp;
  VALUE doc;

  Data_Get_Struct(self, ruby_xml_html_parser, rxp);

  switch (rxp->data_type) {
  case RUBY_LIBXML_SRC_TYPE_NULL:
    return(Qnil);
  case RUBY_LIBXML_SRC_TYPE_STRING:
  //case RUBY_LIBXML_SRC_TYPE_FILE:
  //case RUBY_LIBXML_SRC_TYPE_IO:
    Data_Get_Struct(rxp->ctxt, ruby_xml_parser_context, rxpc);
    
    /* don't check return values here, the HTML parser returns errors
     * but still allows the resulting tree to be used.
     */
    htmlParseDocument(rxpc->ctxt);
    xdp = rxpc->ctxt->myDoc;
    rxp->parsed = 1;

    doc = ruby_xml_document_wrap(cXMLDocument, xdp);
    break;
  default:
    rb_fatal("Unknown data type, %d", rxp->data_type);
  }

  return(doc);
}


/*
 * call-seq:
 *    parser.context => context
 * 
 * Obtain the XML::Parser::Context associated with this
 * parser.
 */
VALUE
ruby_xml_html_parser_context_get(VALUE self) {
  ruby_xml_html_parser *rxp;

  Data_Get_Struct(self, ruby_xml_html_parser, rxp);
  if (rxp->ctxt == Qnil)
    return(Qnil);
  else
    return(rxp->ctxt);
}


/*
 * call-seq:
 *    parser.string => "string"
 * 
 * Obtain the string this parser works with.
 */
VALUE
ruby_xml_html_parser_str_get(VALUE self) {
  ruby_xml_html_parser *rxp;
  rx_string_data *data;

  Data_Get_Struct(self, ruby_xml_html_parser, rxp);
  if (rxp->data == NULL || rxp->data_type != RUBY_LIBXML_SRC_TYPE_STRING)
    return(Qnil);

  data = (rx_string_data *)rxp->data;
  return(data->str);
}


/*
 * call-seq:
 *    parser.string = "string"
 * 
 * Set the string this parser works with.
 */
VALUE
ruby_xml_html_parser_str_set(VALUE self, VALUE str) {
  ruby_xml_html_parser *rxp;
  ruby_xml_parser_context *rxpc;
  rx_string_data *data;

  Check_Type(str, T_STRING);
  Data_Get_Struct(self, ruby_xml_html_parser, rxp);

  if (rxp->data_type == RUBY_LIBXML_SRC_TYPE_NULL) {
    rxp->data_type = RUBY_LIBXML_SRC_TYPE_STRING;
    data = ALLOC(rx_string_data);
    rxp->data = data;
  } else if (rxp->data_type != RUBY_LIBXML_SRC_TYPE_STRING) {
    return(Qnil);
  }

  rxp->ctxt = ruby_xml_parser_context_new3();
  data = (rx_string_data *)rxp->data;
  data->str = str;

  Data_Get_Struct(rxp->ctxt, ruby_xml_parser_context, rxpc);
  rxpc->ctxt = htmlCreateMemoryParserCtxt(StringValuePtr(data->str), RSTRING_LEN(data->str));

  return(data->str);
}


// Rdoc needs to know 
#ifdef RDOC_NEVER_DEFINED
  mXML = rb_define_module("XML");
#endif

void
ruby_init_html_parser(void) {	
  cXMLHTMLParser = rb_define_class_under(mXML, "HTMLParser", rb_cObject);

  /*                
  rb_define_singleton_method(cXMLHTMLParser, "file", ruby_xml_html_parser_new_file, 1);
  rb_define_singleton_method(cXMLHTMLParser, "io", ruby_xml_html_parser_new_io, 1);
  */
  rb_define_singleton_method(cXMLHTMLParser, "new", ruby_xml_html_parser_new, 0);
  rb_define_singleton_method(cXMLHTMLParser, "string", ruby_xml_html_parser_new_string, 1);
  /*
  rb_define_method(cXMLHTMLParser, "filename", ruby_xml_html_parser_filename_get, 0);
  rb_define_method(cXMLHTMLParser, "filename=", ruby_xml_html_parser_filename_set, 1);
  rb_define_method(cXMLHTMLParser, "io", ruby_xml_html_parser_io_get, 0);
  rb_define_method(cXMLHTMLParser, "io=", ruby_xml_html_parser_io_set, 1);
  */
  rb_define_method(cXMLHTMLParser, "parse", ruby_xml_html_parser_parse, 0);
  rb_define_method(cXMLHTMLParser, "parser_context", ruby_xml_html_parser_context_get, 0);
  rb_define_method(cXMLHTMLParser, "string", ruby_xml_html_parser_str_get, 0);
  rb_define_method(cXMLHTMLParser, "string=", ruby_xml_html_parser_str_set, 1);  
}
