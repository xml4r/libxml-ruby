/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

#include <stdarg.h>
#include "ruby_libxml.h"

VALUE cXMLParser;
ID INPUT_ATTR;
ID CONTEXT_ATTR;

static int
ctxtRead(FILE *f, char * buf, size_t len) {
    return(rb_io_fread(buf, len, f));
}

/*
 * call-seq:
 * 		XML::Parser.catalog_dump -> true
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
 * 		XML::Parser.catalog_remove(catalog) -> true
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
 * 		XML::Parser.check_lib_versions -> true							
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
 * 		XML::Parser.enabled_automata? -> (true|false)
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
 * 		XML::Parser.enabled_c14n? -> (true|false)
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
 * 		XML::Parser.enabled_catalog? -> (true|false)                                         
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
 * 		XML::Parser.enabled_debug? -> (true|false)  
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
 * 		XML::Parser.enabled_docbook? -> (true|false)  
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
 * 		XML::Parser.enabled_ftp? -> (true|false)  
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
 * 		XML::Parser.enabled_http? -> (true|false)  
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
 * 		XML::Parser.enabled_html? -> (true|false)  
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
 * 		XML::Parser.enabled_iconv? -> (true|false)  
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
 * 		XML::Parser.enabled_memory_debug? -> (true|false)  
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
 * 		XML::Parser.enabled_regexp? -> (true|false)  
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
 * 		XML::Parser.enabled_schemas? -> (true|false)  
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
 * 		XML::Parser.enabled_thread? -> (true|false)  
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
 * 		XML::Parser.enabled_unicode? -> (true|false)  
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
 * 		XML::Parser.enabled_xinclude? -> (true|false)  
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
 * 		XML::Parser.enabled_xpath? -> (true|false)  
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
 * 		XML::Parser.enabled_xpointer? -> (true|false)  
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
 * 		XML::Parser.enabled_zlib? -> (true|false)  
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
 *    XML::Parser.debug_entities -> (true|false)
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
 *    XML::Parser.default_keep_blanks -> (true|false)
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
    rb_raise(rb_eArgError, "Invalid argument, must be a boolean");
  }
}


/*
 * call-seq:
 *    XML::Parser.default_load_external_dtd -> (true|false)
 * 
 * Determine whether parsers load external DTDs by default.
 */
VALUE
ruby_xml_parser_default_load_external_dtd_get(VALUE class) {
  if (xmlLoadExtDtdDefaultValue)
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
 *    XML::Parser.default_line_numbers -> (true|false)
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
 *    XML::Parser.default_pedantic_parser -> (true|false)
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
 *    XML::Parser.default_substitute_entities -> (true|false)
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
 *    XML::Parser.default_tree_indent_string -> "string"
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
  xmlTreeIndentString = xmlStrdup(StringValuePtr(string));
  return(string);
}


/*
 * call-seq:
 *    XML::Parser.default_validity_checking -> (true|false)
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
 *    XML::Parser.default_warnings -> (true|false)
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
 *    XML::Parser.default_compression -> (true|false)
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
 *    XML::Parser.features -> ["feature", ..., "feature"]
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
 *    XML::Parser.indent_tree_output -> (true|false)
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
    rb_raise(rb_eArgError, "Invalid argument, must be boolean");
  }
}

/*
 * call-seq:
 *    XML::Parser.memory_dump -> (true|false)
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
 *    XML::Parser.memory_used -> num_bytes
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
 *    parser.initialize -> parser
 * 
 * Initiliazes instance of parser.
 */
VALUE
ruby_xml_parser_initialize(VALUE self) {
  VALUE input = rb_class_new_instance(0, Qnil, cXMLInput);
  rb_iv_set(self, "@input", input);
  rb_iv_set(self, "@context", Qnil);
  return self;
}


xmlParserCtxtPtr
ruby_xml_parser_filename_ctxt(VALUE input) {
  xmlParserCtxtPtr ctxt;
  VALUE context;
  int retry_count = 0;
  VALUE filename = rb_ivar_get(input, FILE_ATTR);
  
  retry:
  ctxt = xmlCreateFileParserCtxt(StringValuePtr(filename));
  if (ctxt == NULL) {
    if ((errno == EMFILE || errno == ENFILE) && retry_count == 0) {
      retry_count++;
      rb_gc();
      goto retry;
    } else {
      rb_raise(rb_eIOError, StringValuePtr(filename));
    }
  }

  return ctxt;
}

xmlParserCtxtPtr
ruby_xml_parser_str_ctxt(VALUE input) {
  VALUE str = rb_ivar_get(input, STRING_ATTR);
  return xmlCreateMemoryParserCtxt(StringValuePtr(str), RSTRING_LEN(str));
}

xmlParserCtxtPtr
ruby_xml_parser_io_ctxt(VALUE input) {
  xmlParserCtxtPtr ctxt;
  VALUE context;
  VALUE io = rb_ivar_get(input, IO_ATTR);
  VALUE encoding = rb_ivar_get(input, ENCODING_ATTR);
  xmlCharEncoding xmlEncoding = NUM2INT(encoding);
  OpenFile *fptr;
  FILE *f;

  GetOpenFile(io, fptr);
  rb_io_check_readable(fptr);
  f = GetReadFile(fptr);

  return xmlCreateIOParserCtxt(NULL, NULL,
	 			                      (xmlInputReadCallback) ctxtRead,
				                      NULL, f, xmlEncoding);
}


/*
 * call-seq:
 *    parser.parse -> document
 * 
 * Parse the input XML and create an XML::Document with
 * it's content. If an error occurs, XML::Parser::ParseError
 * is thrown.
 */
VALUE
ruby_xml_parser_parse(VALUE self) {
  xmlParserCtxtPtr ctxt;
  VALUE context;
  VALUE source;
  VALUE doc;
  VALUE input = rb_ivar_get(self, INPUT_ATTR);

  context = rb_ivar_get(self, CONTEXT_ATTR);
  if (context != Qnil)
    rb_raise(rb_eRuntimeError, "You cannot parse a data source twice");

  if (rb_ivar_get(input, FILE_ATTR) != Qnil)
    ctxt = ruby_xml_parser_filename_ctxt(input);
  else if (rb_ivar_get(input, STRING_ATTR) != Qnil)
    ctxt = ruby_xml_parser_str_ctxt(input);
  /*else if (rb_ivar_get(input, DOCUMENT_ATTR) != Qnil)
    ctxt = ruby_xml_parser_parse_document(input);*/
  else if (rb_ivar_get(input, IO_ATTR) != Qnil)
    ctxt = ruby_xml_parser_io_ctxt(input);
  else
    rb_raise(rb_eArgError, "You must specify a parser data source");
  
  if (!ctxt)
    ruby_xml_raise(&xmlLastError);

  context = ruby_xml_parser_context_wrap(ctxt);
  rb_ivar_set(self, CONTEXT_ATTR, context);
 
  if (xmlParseDocument(ctxt) == -1 || !ctxt->wellFormed) {
    xmlFreeDoc(ctxt->myDoc);
    ruby_xml_raise(&ctxt->lastError);
  }

  return ruby_xml_document_wrap(ctxt->myDoc);
}


// Rdoc needs to know 
#ifdef RDOC_NEVER_DEFINED
  mLibXML = rb_define_module("LibXML");
  mXML = rb_define_module_under(mLibXML, "XML");
#endif

void
ruby_init_parser(void) {	
  INPUT_ATTR = rb_intern("@input");
  CONTEXT_ATTR = rb_intern("@context");

  cXMLParser = rb_define_class_under(mXML, "Parser", rb_cObject);
                 
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
			     ruby_xml_parser_default_load_external_dtd_get, 0);
  rb_define_singleton_method(cXMLParser, "default_load_external_dtd=",
			     ruby_xml_parser_default_load_external_dtd_set, 1);
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
  
  rb_define_singleton_method(cXMLParser, "default_warnings", ruby_xml_parser_default_warnings_get, 0);
  rb_define_singleton_method(cXMLParser, "default_warnings=", ruby_xml_parser_default_warnings_set, 1);
	rb_define_singleton_method(cXMLParser, "features", ruby_xml_parser_features, 0);
  rb_define_singleton_method(cXMLParser, "indent_tree_output", ruby_xml_parser_indent_tree_output_get, 0);
  rb_define_singleton_method(cXMLParser, "indent_tree_output=", ruby_xml_parser_indent_tree_output_set, 1);
  rb_define_singleton_method(cXMLParser, "memory_dump", ruby_xml_parser_memory_dump, 0);
  rb_define_singleton_method(cXMLParser, "memory_used", ruby_xml_parser_memory_used, 0);

  /* Atributes */
  rb_define_attr(cXMLParser, "input", 1, 0);
  rb_define_attr(cXMLParser, "context", 1, 0);

  /* Instance Methods */
  rb_define_method(cXMLParser, "initialize", ruby_xml_parser_initialize, 0);
  rb_define_method(cXMLParser, "parse", ruby_xml_parser_parse, 0);
}
