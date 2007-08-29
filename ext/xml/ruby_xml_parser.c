/* $Id$ */
/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

#include <stdarg.h>
#include "libxml.h"

static VALUE libxml_xmlRubyErrorProc = Qnil;
static int id_call;

int ruby_xml_parser_count = 0;
VALUE cXMLParser;
VALUE eXMLParserParseError;

static int
ctxtRead(FILE *f, char * buf, int len) {
    return(fread(buf, 1, len, f));
}

/*
 * call-seq:
 * 		XML::Parser.catalog_dump => true
 * 
 * Dump the parser resource catalogs to stdout.
 */
VALUE
ruby_xml_parser_catalog_dump(VALUE self) {
  xmlCatalogDump(stdout);
  return(Qtrue);
}


/*
 * call-seq:
 * 		XML::Parser.catalog_remove(catalog) => true
 * 
 * Remove the specified resource catalog.
 */
VALUE
ruby_xml_parser_catalog_remove(VALUE self, VALUE cat) {
  Check_Type(cat, T_STRING);
  xmlCatalogRemove((xmlChar *)StringValuePtr(cat));
  return(Qtrue);
}


/*
 * call-seq:
 * 		XML::Parser.check_lib_versions => true							
 * 
 * Check LIBXML version matches version the bindings
 * were compiled to. Throws an exception if not.
 */
VALUE
ruby_xml_parser_check_lib_versions(VALUE class) {
  xmlCheckVersion(LIBXML_VERSION);
  return(Qtrue);
}


/*
 * call-seq:
 * 		XML::Parser.enabled_automata? => (true|false)
 * 
 * Determine whether libxml regexp automata support is enabled.
 */
VALUE
ruby_xml_parser_enabled_automata_q(VALUE class) {
#ifdef LIBXML_AUTOMATA_ENABLED
  return(Qtrue);
#else
 return(Qfalse);
#endif
}


/*
 * call-seq:
 * 		XML::Parser.enabled_c14n? => (true|false)
 * 
 * Determine whether libxml 'canonical XML' support is enabled.
 * See "Canonical XML" (http://www.w3.org/TR/xml-c14n)
 */
VALUE
ruby_xml_parser_enabled_c14n_q(VALUE class) {
#ifdef LIBXML_C14N_ENABLED
  return(Qtrue);
#else
 return(Qfalse);
#endif
}


/*
 * call-seq:
 * 		XML::Parser.enabled_catalog? => (true|false)                                         
 * 
 * Determine whether libxml resource catalog support is enabled.
 */
VALUE
ruby_xml_parser_enabled_catalog_q(VALUE class) {
#ifdef LIBXML_CATALOG_ENABLED
  return(Qtrue);
#else
 return(Qfalse);
#endif
}


/*
 * call-seq:
 * 		XML::Parser.enabled_debug? => (true|false)  
 * 
 * Determine whether libxml debugging support is enabled.
 */
VALUE
ruby_xml_parser_enabled_debug_q(VALUE class) {
#ifdef LIBXML_DEBUG_ENABLED
  return(Qtrue);
#else
 return(Qfalse);
#endif
}


/*
 * call-seq:
 * 		XML::Parser.enabled_docbook? => (true|false)  
 * 
 * Determine whether libxml docbook support is enabled.
 */
VALUE
ruby_xml_parser_enabled_docbook_q(VALUE class) {
#ifdef LIBXML_DOCB_ENABLED
  return(Qtrue);
#else
 return(Qfalse);
#endif
}


/*
 * call-seq:
 * 		XML::Parser.enabled_ftp? => (true|false)  
 * 
 * Determine whether libxml ftp client support is enabled.
 */
VALUE
ruby_xml_parser_enabled_ftp_q(VALUE class) {
#ifdef LIBXML_FTP_ENABLED
  return(Qtrue);
#else
 return(Qfalse);
#endif
}


/*
 * call-seq:
 * 		XML::Parser.enabled_http? => (true|false)  
 * 
 * Determine whether libxml http client support is enabled.
 */
VALUE
ruby_xml_parser_enabled_http_q(VALUE class) {
#ifdef LIBXML_HTTP_ENABLED
  return(Qtrue);
#else
 return(Qfalse);
#endif
}


/*
 * call-seq:
 * 		XML::Parser.enabled_html? => (true|false)  
 * 
 * Determine whether libxml html support is enabled.
 */
VALUE
ruby_xml_parser_enabled_html_q(VALUE class) {
#ifdef LIBXML_HTML_ENABLED
  return(Qtrue);
#else
 return(Qfalse);
#endif
}


/*
 * call-seq:
 * 		XML::Parser.enabled_iconv? => (true|false)  
 * 
 * Determine whether libxml iconv support is enabled.
 */
VALUE
ruby_xml_parser_enabled_iconv_q(VALUE class) {
#ifdef LIBXML_ICONV_ENABLED
  return(Qtrue);
#else
 return(Qfalse);
#endif
}


/*
 * call-seq:
 * 		XML::Parser.enabled_memory_debug? => (true|false)  
 * 
 * Determine whether libxml memory location debugging support
 * is enabled.
 */
VALUE
ruby_xml_parser_enabled_memory_debug_location_q(VALUE class) {
#ifdef DEBUG_MEMORY_LOCATION
  return(Qtrue);
#else
 return(Qfalse);
#endif
}


/*
 * call-seq:
 * 		XML::Parser.enabled_regexp? => (true|false)  
 * 
 * Determine whether libxml regular expression support is enabled.
 */
VALUE
ruby_xml_parser_enabled_regexp_q(VALUE class) {
#ifdef LIBXML_REGEXP_ENABLED
  return(Qtrue);
#else
 return(Qfalse);
#endif
}


/*
 * call-seq:
 * 		XML::Parser.enabled_schemas? => (true|false)  
 * 
 * Determine whether libxml schema support is enabled.
 */
VALUE
ruby_xml_parser_enabled_schemas_q(VALUE class) {
#ifdef LIBXML_SCHEMAS_ENABLED
  return(Qtrue);
#else
 return(Qfalse);
#endif
}


/*
 * call-seq:
 * 		XML::Parser.enabled_thread? => (true|false)  
 * 
 * Determine whether libxml thread-safe semantics support 
 * is enabled (I think?).
 */
VALUE
ruby_xml_parser_enabled_thread_q(VALUE class) {
#ifdef LIBXML_THREAD_ENABLED
  return(Qtrue);
#else
 return(Qfalse);
#endif
}


/*
 * call-seq:
 * 		XML::Parser.enabled_unicode? => (true|false)  
 * 
 * Determine whether libxml unicode support is enabled.
 */
VALUE
ruby_xml_parser_enabled_unicode_q(VALUE class) {
#ifdef LIBXML_UNICODE_ENABLED
  return(Qtrue);
#else
 return(Qfalse);
#endif
}


/*
 * call-seq:
 * 		XML::Parser.enabled_xinclude? => (true|false)  
 * 
 * Determine whether libxml xinclude support is enabled.
 */
VALUE
ruby_xml_parser_enabled_xinclude_q(VALUE class) {
#ifdef LIBXML_XINCLUDE_ENABLED
  return(Qtrue);
#else
 return(Qfalse);
#endif
}


/*
 * call-seq:
 * 		XML::Parser.enabled_xpath? => (true|false)  
 * 
 * Determine whether libxml xpath support is enabled.
 */
VALUE
ruby_xml_parser_enabled_xpath_q(VALUE class) {
#ifdef LIBXML_XPATH_ENABLED
  return(Qtrue);
#else
 return(Qfalse);
#endif
}


/*
 * call-seq:
 * 		XML::Parser.enabled_xpointer? => (true|false)  
 * 
 * Determine whether libxml xpointer support is enabled.
 */
VALUE
ruby_xml_parser_enabled_xpointer_q(VALUE class) {
#ifdef LIBXML_XPTR_ENABLED
  return(Qtrue);
#else
 return(Qfalse);
#endif
}


/*
 * call-seq:
 * 		XML::Parser.enabled_zlib? => (true|false)  
 * 
 * Determine whether libxml zlib support is enabled.
 */
VALUE
ruby_xml_parser_enabled_zlib_q(VALUE class) {
#ifdef HAVE_ZLIB_H
  return(Qtrue);
#else
 return(Qfalse);
#endif
}


/*
 * call-seq:
 *    XML::Parser.debug_entities => (true|false)
 * 
 * Determine whether included-entity debugging is enabled.
 * (Requires Libxml to be compiled with debugging support)
 */
VALUE
ruby_xml_parser_debug_entities_get(VALUE class) {
#ifdef LIBXML_DEBUG_ENABLED
  if (xmlParserDebugEntities)
    return(Qtrue);
  else
    return(Qfalse);
#else
  rb_warn("libxml was compiled with debugging turned off");
  return(Qfalse);
#endif
}


/*
 * call-seq:
 *    XML::Parser.debug_entities = true|false
 * 
 * Enable or disable included-entity debugging.
 * (Requires Libxml to be compiled with debugging support)
 */
VALUE
ruby_xml_parser_debug_entities_set(VALUE class, VALUE bool) {
#ifdef LIBXML_DEBUG_ENABLED
  if (TYPE(bool) == T_FALSE) {
    xmlParserDebugEntities = 0;
    return(Qfalse);
  } else {
    xmlParserDebugEntities = 1;
    return(Qtrue);
  }
#else
  rb_warn("libxml was compiled with debugging turned off");
#endif
}


/*
 * call-seq:
 *    XML::Parser.default_keep_blanks => (true|false)
 * 
 * Determine whether parsers retain whitespace by default.
 */
VALUE
ruby_xml_parser_default_keep_blanks_get(VALUE class) {
  if (xmlKeepBlanksDefaultValue)
    return(Qtrue);
  else
    return(Qfalse);
}


/*
 * call-seq:
 *    XML::Parser.default_keep_blanks = true|false
 * 
 * Controls whether parsers retain whitespace by default.
 */
VALUE
ruby_xml_parser_default_keep_blanks_set(VALUE class, VALUE bool) {
  if (TYPE(bool) == T_FALSE) {
    xmlKeepBlanksDefaultValue = 0;
    return(Qfalse);
  } else if (TYPE(bool) == T_TRUE) {
    xmlKeepBlanksDefaultValue = 1;
    return(Qtrue);
  } else {
    rb_raise(rb_eArgError, "invalid argument, must be a boolean");
  }
}


/*
 * call-seq:
 *    XML::Parser.default_load_external_dtd => (true|false)
 * 
 * Determine whether parsers load external DTDs by default.
 */
VALUE
ruby_xml_parser_default_load_external_dtd_get(VALUE class) {
  if (xmlSubstituteEntitiesDefaultValue)
    return(Qtrue);
  else
    return(Qfalse);
}


/*
 * call-seq:
 *    XML::Parser.default_load_external_dtd = true|false
 * 
 * Controls whether parsers load external DTDs by default.
 */
VALUE
ruby_xml_parser_default_load_external_dtd_set(VALUE class, VALUE bool) {
  if (TYPE(bool) == T_FALSE) {
    xmlLoadExtDtdDefaultValue = 0;
    return(Qfalse);
  } else {
    xmlLoadExtDtdDefaultValue = 1;
    return(Qtrue);
  }
}


/*
 * call-seq:
 *    XML::Parser.default_line_numbers => (true|false)
 * 
 * Determine whether parsers retain line-numbers by default.
 */
VALUE
ruby_xml_parser_default_line_numbers_get(VALUE class) {
  if (xmlLineNumbersDefaultValue)
    return(Qtrue);
  else
    return(Qfalse);
}


/*
 * call-seq:
 *    XML::Parser.default_line_numbers = true|false
 * 
 * Controls whether parsers retain line-numbers by default.
 */
VALUE
ruby_xml_parser_default_line_numbers_set(VALUE class, VALUE bool) {
  if (TYPE(bool) == T_FALSE) {
    xmlLineNumbersDefault(0);
    return(Qfalse);
  } else {
    xmlLineNumbersDefault(1);
    return(Qtrue);
  }
}


/*
 * call-seq:
 *    XML::Parser.default_pedantic_parser => (true|false)
 * 
 * Determine whether parsers are pedantic by default.
 */
VALUE
ruby_xml_parser_default_pedantic_parser_get(VALUE class) {
  if (xmlPedanticParserDefaultValue)
    return(Qtrue);
  else
    return(Qfalse);
}


/*
 * call-seq:
 *    XML::Parser.default_pedantic_parser = true|false
 * 
 * Controls whether parsers are pedantic by default.
 */
VALUE
ruby_xml_parser_default_pedantic_parser_set(VALUE class, VALUE bool) {
  if (TYPE(bool) == T_FALSE) {
    xmlPedanticParserDefault(0);
    return(Qfalse);
  } else {
    xmlPedanticParserDefault(1);
    return(Qtrue);
  }
}


/*
 * call-seq:
 *    XML::Parser.default_substitute_entities => (true|false)
 * 
 * Determine whether parsers perform inline entity substitution
 * (for external entities) by default.
 */
VALUE
ruby_xml_parser_default_substitute_entities_get(VALUE class) {
  if (xmlSubstituteEntitiesDefaultValue)
    return(Qtrue);
  else
    return(Qfalse);
}


/*
 * call-seq:
 *    XML::Parser.default_substitute_entities = true|false
 * 
 * Controls whether parsers perform inline entity substitution
 * (for external entities) by default.
 */
VALUE
ruby_xml_parser_default_substitute_entities_set(VALUE class, VALUE bool) {
  if (TYPE(bool) == T_FALSE) {
    xmlSubstituteEntitiesDefault(0);
    return(Qfalse);
  } else {
    xmlSubstituteEntitiesDefault(1);
    return(Qtrue);
  }
}


/*
 * call-seq:
 *    XML::Parser.default_tree_indent_string => "string"
 * 
 * Obtain the default string used by parsers to indent the XML tree
 * for output.
 */
VALUE
ruby_xml_parser_default_tree_indent_string_get(VALUE class) {
  if (xmlTreeIndentString == NULL)
    return(Qnil);
  else
    return(rb_str_new2(xmlTreeIndentString));
}


/*
 * call-seq:
 *    XML::Parser.default_tree_indent_string = "string"
 * 
 * Set the default string used by parsers to indent the XML tree
 * for output.
 */
VALUE
ruby_xml_parser_default_tree_indent_string_set(VALUE class, VALUE string) {
  Check_Type(string, T_STRING);
  xmlTreeIndentString = ruby_strdup(StringValuePtr(string));
  return(string);
}


/*
 * call-seq:
 *    XML::Parser.default_validity_checking => (true|false)
 * 
 * Determine whether parsers perform XML validation by default.
 */
VALUE
ruby_xml_parser_default_validity_checking_get(VALUE class) {
  if (xmlDoValidityCheckingDefaultValue)
    return(Qtrue);
  else
    return(Qfalse);
}


/*
 * call-seq:
 *    XML::Parser.default_validity_checking = true|false
 * 
 * Controls whether parsers perform XML validation by default.
 */
VALUE
ruby_xml_parser_default_validity_checking_set(VALUE class, VALUE bool) {
  if (TYPE(bool) == T_FALSE) {
    xmlDoValidityCheckingDefaultValue = 0;
    return(Qfalse);
  } else {
    xmlDoValidityCheckingDefaultValue = 1;
    return(Qtrue);
  }
}


/*
 * call-seq:
 *    XML::Parser.default_warnings => (true|false)
 * 
 * Determine whether parsers output warnings by default.
 */
VALUE
ruby_xml_parser_default_warnings_get(VALUE class) {
  if (xmlGetWarningsDefaultValue)
    return(Qtrue);
  else
    return(Qfalse);
}


/*
 * call-seq:
 *    XML::Parser.default_warnings = true|false
 * 
 * Controls whether parsers output warnings by default.
 */
VALUE
ruby_xml_parser_default_warnings_set(VALUE class, VALUE bool) {
  if (TYPE(bool) == T_FALSE) {
    xmlGetWarningsDefaultValue = 0;
    return(Qfalse);
  } else {
    xmlGetWarningsDefaultValue = 1;
    return(Qtrue);
  }
}


/*
 * call-seq:
 *    XML::Parser.default_compression => (true|false)
 * 
 * Determine whether parsers use Zlib compression by default
 * (requires libxml to be compiled with Zlib support).
 */
VALUE
ruby_xml_parser_default_compression_get(VALUE class) {
#ifdef HAVE_ZLIB_H
  return(INT2FIX(xmlGetCompressMode()));
#else
  rb_warn("libxml was compiled without zlib support");
  return(Qfalse);
#endif
}


/*
 * call-seq:
 *    XML::Parser.default_compression = true|false
 * 
 * Controls whether parsers use Zlib compression by default
 * (requires libxml to be compiled with Zlib support).
 */
VALUE
ruby_xml_parser_default_compression_set(VALUE class, VALUE num) {
#ifdef HAVE_ZLIB_H
  Check_Type(num, T_FIXNUM);
  xmlSetCompressMode(FIX2INT(num));
  return(num);
#else
  rb_warn("libxml was compiled without zlib support");
  return(Qfalse);
#endif
}


/*
 * call-seq:
 *    XML::Parser.features => ["feature", ..., "feature"]
 * 
 * Obtains an array of strings representing features supported 
 * (and enabled) by the installed libxml.
 */
VALUE
ruby_xml_parser_features(VALUE class) {
  VALUE arr, str;
  int i, len = MAX_LIBXML_FEATURES_LEN;
  char **list = NULL;

  list = ALLOC_N(char *,MAX_LIBXML_FEATURES_LEN);
  MEMZERO(list, char *, MAX_LIBXML_FEATURES_LEN);

  arr = rb_ary_new();
  if (xmlGetFeaturesList(&len, (const char **)list) == -1)
    return Qnil;

  for (i = 0; i < len; i++) {
    str = rb_str_new2((const char *)list[i]);
    rb_gc_unregister_address(&str);
    rb_ary_push(arr, str);
  }

  if (len == MAX_LIBXML_FEATURES_LEN)
    rb_warn("Please contact libxml-devel@rubyforge.org and ask to have the \"MAX_LIBXML_FEATURES_LEN increased\" because you could possibly be seeing an incomplete list");

  ruby_xfree(list);
  return(arr);
}


/*
 * call-seq:
 *    parser.filename => "filename"
 * 
 * Obtain the filename this parser will read from.
 */
VALUE
ruby_xml_parser_filename_get(VALUE self) {
  ruby_xml_parser *rxp;
  rx_file_data *data;

  Data_Get_Struct(self, ruby_xml_parser, rxp);
  if (rxp->data == NULL)
    return(Qnil);

  if (rxp->data_type != RUBY_LIBXML_SRC_TYPE_FILE)
    return(Qnil);

  data = (rx_file_data *)rxp->data;
  return(data->filename);
}


/*
 * call-seq:
 *    parser.filename = "filename"
 * 
 * Set the filename this parser will read from.
 */
VALUE
ruby_xml_parser_filename_set(VALUE self, VALUE filename) {
  ruby_xml_parser *rxp;
  ruby_xml_parser_context *rxpc;
  rx_file_data *data;

  Check_Type(filename, T_STRING);
  Data_Get_Struct(self, ruby_xml_parser, rxp);

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
  rxpc->ctxt = xmlCreateFileParserCtxt(StringValuePtr(filename));
  if (rxpc->ctxt == NULL)
    rb_sys_fail(StringValuePtr(filename));

  return(data->filename);
}


void
ruby_xml_parser_free(ruby_xml_parser *rxp) {
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
 *    XML::Parser.indent_tree_output => (true|false)
 * 
 * Determines whether XML output will be indented 
 * (using the string supplied to +default_indent_tree_string+)
 */
VALUE
ruby_xml_parser_indent_tree_output_get(VALUE class) {
  if (xmlIndentTreeOutput)
    return(Qtrue);
  else
    return(Qfalse);
}


/*
 * call-seq:
 *    XML::Parser.indent_tree_output = true|false
 * 
 * Controls whether XML output will be indented 
 * (using the string supplied to +default_indent_tree_string+)
 */
VALUE
ruby_xml_parser_indent_tree_output_set(VALUE class, VALUE bool) {
  if (TYPE(bool) == T_TRUE) {
    xmlIndentTreeOutput = 1;
    return(Qtrue);
  } else if (TYPE(bool) == T_FALSE) {
    xmlIndentTreeOutput = 0;
    return(Qfalse);
  } else {
    rb_raise(rb_eArgError, "invalid argument, must be boolean");
  }
}


/*
 * call-seq:
 *    parser.io => IO
 * 
 * Obtain the IO instance this parser works with.
 */
VALUE
ruby_xml_parser_io_get(VALUE self, VALUE io) {
  ruby_xml_parser *rxp;
  rx_io_data *data;

  Data_Get_Struct(self, ruby_xml_parser, rxp);

  if (rxp->data_type == RUBY_LIBXML_SRC_TYPE_NULL ||
      rxp->data_type != RUBY_LIBXML_SRC_TYPE_IO ||
      rxp->data == NULL)
    return(Qnil);

  data = (rx_io_data *)rxp->data;

  return(data->io);
}


/*
 * call-seq:
 *    parser.io = IO
 * 
 * Set the IO instance this parser works with.
 */
VALUE
ruby_xml_parser_io_set(VALUE self, VALUE io) {
  ruby_xml_parser *rxp;
  ruby_xml_parser_context *rxpc;
  rx_io_data *data;
  OpenFile *fptr;
  FILE *f;

  if (!rb_obj_is_kind_of(io, rb_cIO))
    rb_raise(rb_eTypeError, "need an IO object");

  Data_Get_Struct(self, ruby_xml_parser, rxp);

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
  rxpc->ctxt = xmlCreateIOParserCtxt(NULL, NULL,
				     (xmlInputReadCallback) ctxtRead,
				     NULL, f, XML_CHAR_ENCODING_NONE);
  if (NIL_P(rxpc->ctxt))
    rb_sys_fail(0);

  return(data->io);
}


void
ruby_xml_parser_mark(ruby_xml_parser *rxp) {
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
 *    XML::Parser.memory_dump => (true|false)
 * 
 * Perform a parser memory dump (requires memory debugging 
 * support in libxml).
 */
VALUE
ruby_xml_parser_memory_dump(VALUE self) {
#ifdef DEBUG_MEMORY_LOCATION
  xmlMemoryDump();
  return(Qtrue);
#else
  rb_warn("libxml was compiled without memory debugging support");
  return(Qfalse);
#endif
}


/*
 * call-seq:
 *    XML::Parser.memory_used => num_bytes
 * 
 * Perform a parser memory dump (requires memory debugging 
 * support in libxml).
 */
VALUE
ruby_xml_parser_memory_used(VALUE self) {
#ifdef DEBUG_MEMORY_LOCATION
  return(INT2NUM(xmlMemUsed()));
#else
  rb_warn("libxml was compiled without memory debugging support");
  return(Qfalse);
#endif
}


/*
 * call-seq:
 *    XML::Parser.new => parser
 * 
 * Create a new parser instance with no pre-determined source.
 */
VALUE
ruby_xml_parser_new(VALUE class) {
  ruby_xml_parser *rxp;

  ruby_xml_parser_count++;
  rxp = ALLOC(ruby_xml_parser);
  rxp->ctxt = Qnil;
  rxp->data_type = RUBY_LIBXML_SRC_TYPE_NULL;
  rxp->data = NULL;
  rxp->parsed = 0;

  return(Data_Wrap_Struct(class, ruby_xml_parser_mark,
			  ruby_xml_parser_free, rxp));
}


/*
 * call-seq:
 *    XML::Parser.file => parser
 * 
 * Create a new parser instance that will read the specified file.
 */
VALUE
ruby_xml_parser_new_file(VALUE class, VALUE filename) {
  VALUE obj;
  ruby_xml_parser *rxp;
  rx_file_data *data;

  obj = ruby_xml_parser_new(class);
  Data_Get_Struct(obj, ruby_xml_parser, rxp);

  data = ALLOC(rx_file_data);
  rxp->data_type = RUBY_LIBXML_SRC_TYPE_FILE;
  rxp->data = data;

  ruby_xml_parser_filename_set(obj, filename);

  return(obj);
}


/*
 * call-seq:
 *    XML::Parser.io => parser
 * 
 * Create a new parser instance that will read from the
 * specified IO object.
 */
VALUE
ruby_xml_parser_new_io(VALUE class, VALUE io) {
  VALUE obj;
  ruby_xml_parser *rxp;
  rx_io_data *data;

  obj = ruby_xml_parser_new(class);
  Data_Get_Struct(obj, ruby_xml_parser, rxp);

  data = ALLOC(rx_io_data);
  rxp->data_type = RUBY_LIBXML_SRC_TYPE_IO;
  rxp->data = data;

  ruby_xml_parser_io_set(obj, io);

  return(obj);
}


/*
 * call-seq:
 *    XML::Parser.string => parser
 * 
 * Create a new parser instance that will parse the given
 * string.
 */
VALUE
ruby_xml_parser_new_string(VALUE class, VALUE str) {
  VALUE obj;
  ruby_xml_parser *rxp;
  rx_string_data *data;

  obj = ruby_xml_parser_new(class);
  Data_Get_Struct(obj, ruby_xml_parser, rxp);

  data = ALLOC(rx_string_data);
  rxp->data_type = RUBY_LIBXML_SRC_TYPE_STRING;
  rxp->data = data;

  ruby_xml_parser_str_set(obj, str);

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
ruby_xml_parser_parse(VALUE self) {
  ruby_xml_document_t *rxd;
  ruby_xml_parser *rxp;
  ruby_xml_parser_context *rxpc;
  xmlDocPtr xdp;
  VALUE doc;

  Data_Get_Struct(self, ruby_xml_parser, rxp);

  switch (rxp->data_type) {
  case RUBY_LIBXML_SRC_TYPE_NULL:
    return(Qnil);
  case RUBY_LIBXML_SRC_TYPE_STRING:
  case RUBY_LIBXML_SRC_TYPE_FILE:
  case RUBY_LIBXML_SRC_TYPE_IO:
    Data_Get_Struct(rxp->ctxt, ruby_xml_parser_context, rxpc);
    if (xmlParseDocument(rxpc->ctxt) == -1) {
      xmlFreeDoc(rxpc->ctxt->myDoc);
      rb_raise(eXMLParserParseError, "Document didn't parse");
    }

    xdp = rxpc->ctxt->myDoc;
    if (!rxpc->ctxt->wellFormed) {
      xmlFreeDoc(xdp);
      xdp = NULL;
      rb_raise(eXMLParserParseError, "Document did not contain well-formed XML");
    } else {
      rxp->parsed = 1;
    }

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
ruby_xml_parser_parser_context_get(VALUE self) {
  ruby_xml_parser *rxp;

  Data_Get_Struct(self, ruby_xml_parser, rxp);
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
ruby_xml_parser_str_get(VALUE self) {
  ruby_xml_parser *rxp;
  rx_string_data *data;

  Data_Get_Struct(self, ruby_xml_parser, rxp);
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
ruby_xml_parser_str_set(VALUE self, VALUE str) {
  ruby_xml_parser *rxp;
  ruby_xml_parser_context *rxpc;
  rx_string_data *data;

  Check_Type(str, T_STRING);
  Data_Get_Struct(self, ruby_xml_parser, rxp);

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
  rxpc->ctxt = xmlCreateMemoryParserCtxt(StringValuePtr(data->str), RSTRING_LEN(data->str));

  return(data->str);
}

/*
 * call-seq:
 *    XML::Parser.register_error_handler(lambda { |msg| ... }) => old_handler
 *    XML::Parser.register_error_handler(nil) => old_handler
 * 
 * Register the attached block as the handler for parser errors.
 * A message describing parse errors is passed to the block.
 * Libxml passes error messages to the handler in parts, one per call.
 * A typical error results in six calls to this proc, with arguments:
 * 
 *   "Entity: line 1: ", 
 *   "parser ", 
 *   "error : ", 
 *   "Opening and ending tag mismatch: foo line 1 and foz\n",
 *   "<foo><bar/></foz>\n",
 *   "                 ^\n"
 * 
 * Note that the error handler is shared by all threads.
 */
VALUE
ruby_xml_parser_registerErrorHandler(VALUE self, VALUE proc) {
  VALUE old_block = libxml_xmlRubyErrorProc;
  libxml_xmlRubyErrorProc = proc;
  return(old_block);
}

static void
libxml_xmlErrorFuncHandler(ATTRIBUTE_UNUSED void *ctx, const char *msg, ...)
{
  va_list ap;
  char str[1000];
  VALUE rstr;

  if (libxml_xmlRubyErrorProc == Qnil) {
    va_start(ap, msg);
    vfprintf(stderr, msg, ap);
    va_end(ap);
  } else {
    va_start(ap, msg);
    if (vsnprintf(str, 999, msg, ap) >= 998) str[999] = 0;
    va_end(ap);

    rstr = rb_str_new2(str);
    rb_funcall2(libxml_xmlRubyErrorProc, id_call, 1, &rstr);
  }
}

/* #define RUBY_XML_PARSER_ENABLED_INIT(func, method) \
 rb_define_singleton_method(cXMLParser, method, \
			   ruby_xml_parser_enabled_##func##_q, 0); */

///#include "cbg.c"
///
///VALUE ruby_register_deb(VALUE self) {
///  deb_register_cbg();
///  return(Qtrue);
///}

// Rdoc needs to know 
#ifdef RDOC_NEVER_DEFINED
  mXML = rb_define_module("XML");
#endif

void
ruby_init_parser(void) {	
  cXMLParser = rb_define_class_under(mXML, "Parser", rb_cObject);
  eXMLParserParseError = rb_define_class_under(cXMLParser, "ParseError",
					       rb_eRuntimeError);
                 
  /* Constants */
  rb_define_const(cXMLParser, "LIBXML_VERSION",
		  rb_str_new2(LIBXML_DOTTED_VERSION));
  rb_define_const(cXMLParser, "VERSION", rb_str_new2(RUBY_LIBXML_VERSION));
  rb_define_const(cXMLParser, "VERNUM", INT2NUM(RUBY_LIBXML_VERNUM));

  /* Question-esqe Class Methods */
  /* RDoc won't have them defined by a macro... */
  rb_define_singleton_method(cXMLParser, "enabled_automata?",
			     ruby_xml_parser_enabled_automata_q, 0);
  rb_define_singleton_method(cXMLParser, "enabled_c14n?",
			     ruby_xml_parser_enabled_c14n_q, 0);
  rb_define_singleton_method(cXMLParser, "enabled_catalog?",
			     ruby_xml_parser_enabled_catalog_q, 0);
  rb_define_singleton_method(cXMLParser, "enabled_debug?",
			     ruby_xml_parser_enabled_debug_q, 0);
  rb_define_singleton_method(cXMLParser, "enabled_docbook?",
			     ruby_xml_parser_enabled_docbook_q, 0);
  rb_define_singleton_method(cXMLParser, "enabled_ftp?",
			     ruby_xml_parser_enabled_ftp_q, 0);
  rb_define_singleton_method(cXMLParser, "enabled_http?",
			     ruby_xml_parser_enabled_http_q, 0);
  rb_define_singleton_method(cXMLParser, "enabled_html?",
			     ruby_xml_parser_enabled_html_q, 0);
  rb_define_singleton_method(cXMLParser, "enabled_iconv?",
			     ruby_xml_parser_enabled_iconv_q, 0);
  rb_define_singleton_method(cXMLParser, "enabled_memory_debug?",
			     ruby_xml_parser_enabled_memory_debug_location_q, 0);
  rb_define_singleton_method(cXMLParser, "enabled_regexp?",
			     ruby_xml_parser_enabled_regexp_q, 0);
  rb_define_singleton_method(cXMLParser, "enabled_schemas?",
			     ruby_xml_parser_enabled_schemas_q, 0);
  rb_define_singleton_method(cXMLParser, "enabled_thread?",
			     ruby_xml_parser_enabled_thread_q, 0);
  rb_define_singleton_method(cXMLParser, "enabled_unicode?",
			     ruby_xml_parser_enabled_unicode_q, 0);
  rb_define_singleton_method(cXMLParser, "enabled_xinclude?",
			     ruby_xml_parser_enabled_xinclude_q, 0);
  rb_define_singleton_method(cXMLParser, "enabled_xpath?",
			     ruby_xml_parser_enabled_xpath_q, 0);
  rb_define_singleton_method(cXMLParser, "enabled_xpointer?",
			     ruby_xml_parser_enabled_xpointer_q, 0);
  rb_define_singleton_method(cXMLParser, "enabled_zlib?",
			     ruby_xml_parser_enabled_zlib_q, 0);

  /* Other Class Methods */
///  rb_define_singleton_method(cXMLParser, "register_deb",
///			     ruby_register_deb, 0);

	// TODO Maybe a set of 'xxxx_catalog' aliases might be more Ruby?
  rb_define_singleton_method(cXMLParser, "catalog_dump",
			     ruby_xml_parser_catalog_dump, 0);
  rb_define_singleton_method(cXMLParser, "catalog_remove",
			     ruby_xml_parser_catalog_remove, 1);
  rb_define_singleton_method(cXMLParser, "check_lib_versions",
			     ruby_xml_parser_check_lib_versions, 0);
           
  // TODO should this be debug_entities_q / debug_entities_set?
  //      should all these default attribute pairs work that way?         
  rb_define_singleton_method(cXMLParser, "debug_entities",
			     ruby_xml_parser_debug_entities_get, 0);
  rb_define_singleton_method(cXMLParser, "debug_entities=",
			     ruby_xml_parser_debug_entities_set, 1);
  rb_define_singleton_method(cXMLParser, "default_compression",
			     ruby_xml_parser_default_compression_get, 0);
  rb_define_singleton_method(cXMLParser, "default_compression=",
			     ruby_xml_parser_default_compression_set, 1);
  rb_define_singleton_method(cXMLParser, "default_keep_blanks",
			     ruby_xml_parser_default_keep_blanks_get, 0);
  rb_define_singleton_method(cXMLParser, "default_keep_blanks=",
			     ruby_xml_parser_default_keep_blanks_set, 1);
  rb_define_singleton_method(cXMLParser, "default_load_external_dtd",
			     ruby_xml_parser_default_load_external_dtd_set, 0);
  rb_define_singleton_method(cXMLParser, "default_load_external_dtd=",
			     ruby_xml_parser_default_load_external_dtd_get, 1);
  rb_define_singleton_method(cXMLParser, "default_line_numbers",
			     ruby_xml_parser_default_line_numbers_get, 0);
  rb_define_singleton_method(cXMLParser, "default_line_numbers=",
			     ruby_xml_parser_default_line_numbers_set, 1);
  rb_define_singleton_method(cXMLParser, "default_pedantic_parser",
			     ruby_xml_parser_default_pedantic_parser_get, 0);
  rb_define_singleton_method(cXMLParser, "default_pedantic_parser=",
			     ruby_xml_parser_default_pedantic_parser_set, 1);
  rb_define_singleton_method(cXMLParser, "default_substitute_entities",
			     ruby_xml_parser_default_substitute_entities_get, 0);
  rb_define_singleton_method(cXMLParser, "default_substitute_entities=",
			     ruby_xml_parser_default_substitute_entities_set, 1);
  rb_define_singleton_method(cXMLParser, "default_tree_indent_string",
			     ruby_xml_parser_default_tree_indent_string_get, 0);
  rb_define_singleton_method(cXMLParser, "default_tree_indent_string=",
			     ruby_xml_parser_default_tree_indent_string_set, 1);
  rb_define_singleton_method(cXMLParser, "default_validity_checking",
			     ruby_xml_parser_default_validity_checking_get, 0);
  rb_define_singleton_method(cXMLParser, "default_validity_checking=",
			     ruby_xml_parser_default_validity_checking_set, 1);
  rb_define_singleton_method(cXMLParser, "default_warnings",
			     ruby_xml_parser_default_warnings_get, 0);
  rb_define_singleton_method(cXMLParser, "default_warnings=",
			     ruby_xml_parser_default_warnings_set, 1);
			     
  rb_define_singleton_method(cXMLParser, "features", ruby_xml_parser_features, 0);
  rb_define_singleton_method(cXMLParser, "file", ruby_xml_parser_new_file, 1);
  rb_define_singleton_method(cXMLParser, "indent_tree_output", ruby_xml_parser_indent_tree_output_get, 0);
  rb_define_singleton_method(cXMLParser, "indent_tree_output=", ruby_xml_parser_indent_tree_output_set, 1);
  rb_define_singleton_method(cXMLParser, "io", ruby_xml_parser_new_io, 1);
  rb_define_singleton_method(cXMLParser, "memory_dump",
			     ruby_xml_parser_memory_dump, 0);
  rb_define_singleton_method(cXMLParser, "memory_used",
			     ruby_xml_parser_memory_used, 0);
  rb_define_singleton_method(cXMLParser, "new", ruby_xml_parser_new, 0);
  rb_define_singleton_method(cXMLParser, "string", ruby_xml_parser_new_string, 1);
  rb_define_singleton_method(cXMLParser, "register_error_handler", ruby_xml_parser_registerErrorHandler, 1);
  rb_define_method(cXMLParser, "filename", ruby_xml_parser_filename_get, 0);
  rb_define_method(cXMLParser, "filename=", ruby_xml_parser_filename_set, 1);
  rb_define_method(cXMLParser, "io", ruby_xml_parser_io_get, 0);
  rb_define_method(cXMLParser, "io=", ruby_xml_parser_io_set, 1);
  rb_define_method(cXMLParser, "parse", ruby_xml_parser_parse, 0);
  rb_define_method(cXMLParser, "parser_context", ruby_xml_parser_parser_context_get, 0);
  rb_define_method(cXMLParser, "string", ruby_xml_parser_str_get, 0);
  rb_define_method(cXMLParser, "string=", ruby_xml_parser_str_set, 1);
  
  // set up error handling
  xmlSetGenericErrorFunc(NULL, libxml_xmlErrorFuncHandler);
  xmlThrDefSetGenericErrorFunc(NULL, libxml_xmlErrorFuncHandler);

  // Ruby needs to know about this even though it's not exported, otherwise
  // our error proc might get garbage collected.
  rb_global_variable(&libxml_xmlRubyErrorProc);             

  id_call = rb_intern("call");
}
