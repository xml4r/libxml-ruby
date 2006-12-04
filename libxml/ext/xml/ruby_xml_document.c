/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

#include <stdarg.h>
#include "libxml.h"
#include "ruby_xml_document.h"

VALUE cXMLDocument;

/*
 * call-seq:
 *    document.compression => num
 * 
 * Obtain this document's compression mode identifier.
 */
VALUE
ruby_xml_document_compression_get(VALUE self) {
#ifdef HAVE_ZLIB_H
  ruby_xml_document *rxd;
  int compmode;
  Data_Get_Struct(self, ruby_xml_document, rxd);

  compmode = xmlGetDocCompressMode(rxd->doc);
  if (compmode == -1)
    return(Qnil);
  else
    return(INT2NUM(compmode));
#else
  rb_warn("libxml not compiled with zlib support");
  return(Qfalse);
#endif
}


/*
 * call-seq:
 *    document.compression = num
 * 
 * Set this document's compression mode.
 */
VALUE
ruby_xml_document_compression_set(VALUE self, VALUE num) {
#ifdef HAVE_ZLIB_H
  ruby_xml_document *rxd;
  int compmode;
  Check_Type(num, T_FIXNUM);
  Data_Get_Struct(self, ruby_xml_document, rxd);

  if (rxd->doc == NULL) {
    return(Qnil);
  } else {
    xmlSetDocCompressMode(rxd->doc, NUM2INT(num));

    compmode = xmlGetDocCompressMode(rxd->doc);
    if (compmode == -1)
      return(Qnil);
    else
      return(INT2NUM(compmode));
  }
#else
  rb_warn("libxml compiled without zlib support");
  return(Qfalse);
#endif
}


/*
 * call-seq:
 *    document.compression? => (true|false)
 * 
 * Determine whether this document is compressed.
 */
VALUE
ruby_xml_document_compression_q(VALUE self) {
#ifdef HAVE_ZLIB_H
  ruby_xml_document *rxd;
  Data_Get_Struct(self, ruby_xml_document, rxd);

  if (rxd->doc->compression != -1)
    return(Qtrue);
  else
    return(Qfalse);
#else
  rb_warn("libxml compiled without zlib support");
  return(Qfalse);
#endif
}


/*
 * call-seq:
 *    document.child => node
 * 
 * Get this document's child node.
 */
VALUE
ruby_xml_document_child_get(VALUE self) {
  ruby_xml_document *rxd;
  ruby_xml_node *rxn;
  VALUE node;

  Data_Get_Struct(self, ruby_xml_document, rxd);

  if (rxd->doc->children == NULL)
    return(Qnil);

  node = ruby_xml_node_new_ptr(cXMLNode, self, rxd->doc->children);
  Data_Get_Struct(node, ruby_xml_node, rxn);
  rxn->xd = self;
  return(node);
}


/*
 * call-seq:
 *    document.child? => (true|false)
 * 
 * Determine whether this document has a child node.
 */
VALUE
ruby_xml_document_child_q(VALUE self) {
  ruby_xml_document *rxd;
  Data_Get_Struct(self, ruby_xml_document, rxd);

  if (rxd->doc->children == NULL)
    return(Qfalse);
  else
    return(Qtrue);
}


/*
 * call-seq:
 *    document.dump([stream]) => true
 * 
 * Dump this document's XML to the specified IO stream.
 * If no stream is specified, stdout is used.
 */
VALUE
ruby_xml_document_dump(int argc, VALUE *argv, VALUE self) {
  OpenFile *fptr;
  VALUE io;
  FILE *out;
  ruby_xml_document *rxd;

  Data_Get_Struct(self, ruby_xml_document, rxd);
  if (rxd->doc == NULL)
    return(Qnil);

  switch (argc) {
  case 0:
    io = rb_stdout;
    break;
  case 1:
    io = argv[0];
    if (!rb_obj_is_kind_of(io, rb_cIO))
      rb_raise(rb_eTypeError, "need an IO object");
    break;
  default:
    rb_raise(rb_eArgError, "wrong number of arguments (0 or 1)");
  }

  GetOpenFile(io, fptr);
  rb_io_check_writable(fptr);
  out = GetWriteFile(fptr);
  xmlDocDump(out, rxd->doc);
  return(Qtrue);
}


/*
 * call-seq:
 *    document.debug_dump([stream]) => true
 * 
 * Debug version of dump.
 */
VALUE
ruby_xml_document_debug_dump(int argc, VALUE *argv, VALUE self) {
#ifdef LIBXML_DEBUG_ENABLED
  OpenFile *fptr;
  VALUE io;
  FILE *out;
  ruby_xml_document *rxd;

  Data_Get_Struct(self, ruby_xml_document, rxd);
  if (rxd->doc == NULL)
    return(Qnil);

  switch (argc) {
  case 0:
    io = rb_stderr;
    break;
  case 1:
    io = argv[0];
    if (!rb_obj_is_kind_of(io, rb_cIO))
      rb_raise(rb_eTypeError, "need an IO object");
    break;
  default:
    rb_raise(rb_eArgError, "wrong number of arguments (0 or 1)");
  }

  GetOpenFile(io, fptr);
  rb_io_check_writable(fptr);
  out = GetWriteFile(fptr);
  xmlDebugDumpDocument(out, rxd->doc);
  return(Qtrue);
#else
  rb_warn("libxml was compiled without debugging support.  Please recompile libxml and ruby-libxml");
  return(Qfalse);
#endif
}


/*
 * call-seq:
 *    document.debug_dump_head([stream]) => true
 * 
 * Debug-dump this document's header to the specified IO stream.
 * If no stream is specified, stdout is used.
 */
VALUE
ruby_xml_document_debug_dump_head(int argc, VALUE *argv, VALUE self) {
#ifdef LIBXML_DEBUG_ENABLED
  OpenFile *fptr;
  VALUE io;
  FILE *out;
  ruby_xml_document *rxd;

  Data_Get_Struct(self, ruby_xml_document, rxd);
  if (rxd->doc == NULL)
    return(Qnil);

  switch (argc) {
  case 0:
    io = rb_stdout;
    break;
  case 1:
    io = argv[0];
    if (!rb_obj_is_kind_of(io, rb_cIO))
      rb_raise(rb_eTypeError, "need an IO object");
    break;
  default:
    rb_raise(rb_eArgError, "wrong number of arguments (0 or 1)");
  }

  GetOpenFile(io, fptr);
  rb_io_check_writable(fptr);
  out = GetWriteFile(fptr);
  xmlDebugDumpDocumentHead(out, rxd->doc);
  return(Qtrue);
#else
  rb_warn("libxml was compiled without debugging support.  Please recompile libxml and ruby-libxml");
  return(Qfalse);
#endif
}


/*
 * call-seq:
 *    document.format_dump([stream], [spacing]) => true
 * 
 * Dump this document's formatted XML to the specified IO stream.
 * If no stream is specified, stdout is used. If spacing is 
 * specified, it must be a boolean that determines whether 
 * spacing is used.
 */
VALUE
ruby_xml_document_format_dump(int argc, VALUE *argv, VALUE self) {
  OpenFile *fptr;
  VALUE bool, io;
  FILE *out;
  ruby_xml_document *rxd;
  int size, spacing;

  Data_Get_Struct(self, ruby_xml_document, rxd);
  if (rxd->doc == NULL)
    return(Qnil);

  switch (argc) {
  case 0:
    io = rb_stdout;
    spacing = 1;
    break;
  case 1:
    io = argv[0];
    if (!rb_obj_is_kind_of(io, rb_cIO))
      rb_raise(rb_eTypeError, "need an IO object");
    spacing = 1;
    break;
  case 2:
    io = argv[0];
    if (!rb_obj_is_kind_of(io, rb_cIO))
      rb_raise(rb_eTypeError, "need an IO object");
    bool = argv[1];
    if (TYPE(bool) == T_TRUE)
      spacing = 1;
    else if (TYPE(bool) == T_FALSE)
      spacing = 0;
    else
      rb_raise(rb_eTypeError, "incorect argument type, second argument must be bool");

    break;
  default:
    rb_raise(rb_eArgError, "wrong number of arguments (0 or 1)");
  }

  GetOpenFile(io, fptr);
  rb_io_check_writable(fptr);
  out = GetWriteFile(fptr);
  size = xmlDocFormatDump(out, rxd->doc, spacing);
  return(INT2NUM(size));
}


/*
 * call-seq:
 *    document.debug_format_dump([stream]) => true
 * 
 * *Deprecated* in favour of format_dump.
 */
VALUE
ruby_xml_document_debug_format_dump(int argc, VALUE *argv, VALUE self) {
  rb_warn("debug_format_dump has been deprecaited, use format_dump instead");
  return(ruby_xml_document_format_dump(argc, argv, self));
}


/*
 * call-seq:
 *    document.encoding => "encoding"
 * 
 * Obtain the encoding specified by this document.
 */
VALUE
ruby_xml_document_encoding_get(VALUE self) {
  ruby_xml_document *rxd;
  Data_Get_Struct(self, ruby_xml_document, rxd);
  if (rxd->doc->encoding == NULL)
    return(Qnil);
  else
    return(rb_str_new2((const char*)rxd->doc->encoding));
}


/*
 * call-seq:
 *    document.encoding = "encoding"
 * 
 * Set the encoding for this document.
 */
VALUE
ruby_xml_document_encoding_set(VALUE self, VALUE encoding) {
  ruby_xml_document *rxd;

  Check_Type(encoding, T_STRING);
  Data_Get_Struct(self, ruby_xml_document, rxd);
  rxd->doc->encoding = (xmlChar*)ruby_strdup(StringValuePtr(encoding));
  return(ruby_xml_document_encoding_get(self));
}


/*
 * call-seq:
 *    document.filename => "filename"
 * 
 * Obtain the filename this document was read from.
 */
VALUE
ruby_xml_document_filename_get(VALUE self) {
  ruby_xml_document *rxd;
  rx_file_data *data;

  Data_Get_Struct(self, ruby_xml_document, rxd);
  if (rxd->data == NULL)
    return(Qnil);

  switch (rxd->data_type) {
  case RUBY_LIBXML_SRC_TYPE_NULL:
    return(Qnil);
  case RUBY_LIBXML_SRC_TYPE_FILE:
    data = (rx_file_data *)rxd->data;
    return(data->filename);
  default:
    rb_fatal("Unknown document type in libxml");
  }

  return(Qnil);
}


/*
 * call-seq:
 *    document.find(xpath_expr, [namespace]) => nodeset
 * 
 * Find nodes matching the specified xpath expression, optionally
 * using the specified namespace. Returns an XML::Node::Set.
 */
VALUE
ruby_xml_document_find(int argc, VALUE *argv, VALUE self) {
  int i, vargc;
  VALUE *vargv;

  if (argc > 2 || argc < 1)
    rb_raise(rb_eArgError, "wrong number of arguments (need 1 or 2)");

  vargc = argc + 1;
  vargv = ALLOC_N(VALUE, vargc + 1);
  vargv[0] = ruby_xml_document_root_get(self);
  for (i = 0; i<argc; i++)
    vargv[i + 1] = argv[i];

  return(ruby_xml_xpath_find2(vargc, vargv));
}


void
ruby_xml_document_free(ruby_xml_document *rxd) {
  void *data;

  if (rxd->doc != NULL && !rxd->is_ptr) {
    xmlFreeDoc(rxd->doc);
    ruby_xml_parser_count--;
    rxd->doc = NULL;
  }

  if (ruby_xml_parser_count == 0)
    xmlCleanupParser();

  switch(rxd->data_type) {
  case RUBY_LIBXML_SRC_TYPE_NULL:
    break;
  case RUBY_LIBXML_SRC_TYPE_FILE:
    data = (void*)(rx_file_data *)rxd->data;
    free((rx_file_data *)data);
    break;
  case RUBY_LIBXML_SRC_TYPE_STRING:
    data = (void*)(rx_string_data *)rxd->data;
    free((rx_string_data *)data);
    break;
  case RUBY_LIBXML_SRC_TYPE_IO:
    data = (void*)(rx_io_data *)rxd->data;
    free((rx_io_data *)data);
    break;
  default:
    rb_fatal("Unknown data type, %d", rxd->data_type);
  }

  free(rxd);
}


/*
 * call-seq:
 *    XML::Document.new(xml_version = 1.0) => document
 * 
 * Create a new XML::Document, optionally specifying the
 * XML version.
 */
VALUE
ruby_xml_document_initialize(int argc, VALUE *argv, VALUE class) {
  VALUE docobj, xmlver;

  switch (argc) {
  case 0:
    xmlver = rb_str_new2("1.0");
    break;
  case 1:
    rb_scan_args(argc, argv, "01", &xmlver);
    break;
  default:
    rb_raise(rb_eArgError, "wrong number of arguments (need 0 or 1)");
  }

  docobj = ruby_xml_document_new2(cXMLDocument, xmlver);
  return(docobj);
}


/*
 * call-seq:
 *    document.last => node
 * 
 * Obtain the last node.
 */
VALUE
ruby_xml_document_last_get(VALUE self) {
  ruby_xml_document *rxd;
  ruby_xml_node *rxn;
  VALUE node;

  Data_Get_Struct(self, ruby_xml_document, rxd);

  if (rxd->doc->last == NULL)
    return(Qnil);

  node = ruby_xml_node_new_ptr(cXMLNode, self, rxd->doc->last);
  Data_Get_Struct(node, ruby_xml_node, rxn);
  rxn->xd = self;
  return(node);
}


/*
 * call-seq:
 *    document.last? => (true|false)
 * 
 * Determine whether there is a last node.
 */
VALUE
ruby_xml_document_last_q(VALUE self) {
  ruby_xml_document *rxd;
  Data_Get_Struct(self, ruby_xml_document, rxd);

  if (rxd->doc->last == NULL)
    return(Qfalse);
  else
    return(Qtrue);
}


static void
ruby_xml_document_mark(ruby_xml_document *rxd) {
  if (rxd == NULL) return;
  if (!NIL_P(rxd->xmlver)) rb_gc_mark(rxd->xmlver);
}


VALUE
ruby_xml_document_new(VALUE class, xmlDocPtr doc) {
  ruby_xml_document *rxd;

  rxd = ALLOC(ruby_xml_document);
  ruby_xml_parser_count++;

  rxd->data = NULL;
  rxd->data_type = RUBY_LIBXML_SRC_TYPE_NULL;
  rxd->doc = doc;
  rxd->is_ptr = 0;
  rxd->xmlver = Qnil;

  return(Data_Wrap_Struct(cXMLDocument, ruby_xml_document_mark,
			  ruby_xml_document_free, rxd));
}


VALUE
ruby_xml_document_new2(VALUE class, VALUE xmlver) {
  ruby_xml_document *rxd;

  Check_Type(xmlver, T_STRING);
  rxd = ALLOC(ruby_xml_document);
  ruby_xml_parser_count++;

  rxd->data = NULL;
  rxd->data_type = RUBY_LIBXML_SRC_TYPE_NULL;
  rxd->doc = xmlNewDoc((xmlChar*)StringValuePtr(xmlver));
  rxd->is_ptr = 0;
  rxd->xmlver = xmlver;

  if (rxd->doc == NULL)
    rb_fatal("bad");

  return(Data_Wrap_Struct(cXMLDocument, ruby_xml_document_mark,
			  ruby_xml_document_free, rxd));
}


VALUE
ruby_xml_document_new3(VALUE class) {
  return(ruby_xml_document_new2(class, rb_str_new2("1.0")));
}


VALUE
ruby_xml_document_new4(VALUE class, xmlDocPtr doc) {
  ruby_xml_document *rxd;

  rxd = ALLOC(ruby_xml_document);

  rxd->data = NULL;
  rxd->data_type = RUBY_LIBXML_SRC_TYPE_NULL;
  rxd->doc = doc;
  rxd->is_ptr = 1;
  rxd->xmlver = Qnil;

  return(Data_Wrap_Struct(cXMLDocument, ruby_xml_document_mark,
			  ruby_xml_document_free, rxd));
}


/*
 * call-seq:
 *    XML::Document.file(filename) => document
 * 
 * Create a new XML::Document by parsing the specified
 * file.
 */
VALUE
ruby_xml_document_new_file(VALUE class, VALUE filename) {
  VALUE parser;

  parser = ruby_xml_parser_new(cXMLParser);
  ruby_xml_parser_filename_set(parser, filename);
  return(ruby_xml_parser_parse(parser));
}


/*
 * call-seq:
 *    document.next => node
 * 
 * Obtain the next node.
 */
VALUE
ruby_xml_document_next_get(VALUE self) {
  ruby_xml_document *rxd;
  ruby_xml_node *rxn;
  VALUE node;

  Data_Get_Struct(self, ruby_xml_document, rxd);

  if (rxd->doc->next == NULL)
    return(Qnil);

  node = ruby_xml_node_new_ptr(cXMLNode, self, rxd->doc->next);
  Data_Get_Struct(node, ruby_xml_node, rxn);
  rxn->xd = self;
  return(node);
}


/*
 * call-seq:
 *    document.next? => (true|false)
 * 
 * Determine whether there is a next node.
 */
VALUE
ruby_xml_document_next_q(VALUE self) {
  ruby_xml_document *rxd;
  Data_Get_Struct(self, ruby_xml_document, rxd);

  if (rxd->doc->next == NULL)
    return(Qfalse);
  else
    return(Qtrue);
}


/*
 * call-seq:
 *    document.parent => node
 * 
 * Obtain the parent node.
 */
VALUE
ruby_xml_document_parent_get(VALUE self) {
  ruby_xml_document *rxd;
  ruby_xml_node *rxn;
  VALUE node;

  Data_Get_Struct(self, ruby_xml_document, rxd);

  if (rxd->doc->parent == NULL)
    return(Qnil);

  node = ruby_xml_node_new_ptr(cXMLNode, self, rxd->doc->parent);
  Data_Get_Struct(node, ruby_xml_node, rxn);
  rxn->xd = self;
  return(node);
}


/*
 * call-seq:
 *    document.parent? => (true|false)
 * 
 * Determine whether there is a parent node.
 */
VALUE
ruby_xml_document_parent_q(VALUE self) {
  ruby_xml_document *rxd;
  Data_Get_Struct(self, ruby_xml_document, rxd);

  if (rxd->doc->parent == NULL)
    return(Qfalse);
  else
    return(Qtrue);
}


/*
 * call-seq:
 *    document.prev => node
 * 
 * Obtain the previous node.
 */
VALUE
ruby_xml_document_prev_get(VALUE self) {
  ruby_xml_document *rxd;
  ruby_xml_node *rxn;
  VALUE node;

  Data_Get_Struct(self, ruby_xml_document, rxd);

  if (rxd->doc->prev == NULL)
    return(Qnil);

  node = ruby_xml_node_new_ptr(cXMLNode, self, rxd->doc->prev);
  Data_Get_Struct(node, ruby_xml_node, rxn);
  rxn->xd = self;
  return(node);
}


/*
 * call-seq:
 *    document.prev? => (true|false)
 * 
 * Determine whether there is a previous node.
 */
VALUE
ruby_xml_document_prev_q(VALUE self) {
  ruby_xml_document *rxd;
  Data_Get_Struct(self, ruby_xml_document, rxd);

  if (rxd->doc->prev == NULL)
    return(Qfalse);
  else
    return(Qtrue);
}


/*
 * call-seq:
 *    document["key"] => "value"
 * 
 * Obtain the named property.
 */
VALUE
ruby_xml_document_property_get(VALUE self, VALUE key) {
  return(ruby_xml_node_property_get(ruby_xml_document_root_get(self), key));
}


/*
 * call-seq:
 *    document["key"] = "value"
 * 
 * Set the named property.
 */
VALUE
ruby_xml_document_property_set(VALUE self, VALUE key, VALUE val) {
  return(ruby_xml_node_property_set(ruby_xml_document_root_get(self), key, val));
}


/*
 * call-seq:
 *    document.root => node
 * 
 * Obtain the root node.
 */
VALUE
ruby_xml_document_root_get(VALUE self) {
  ruby_xml_document *rxd;
  VALUE node;
  xmlNodePtr root;

  Data_Get_Struct(self, ruby_xml_document, rxd);
  root = xmlDocGetRootElement(rxd->doc);

  if (root == NULL)
    return(Qnil);

  node = ruby_xml_node_new_ptr(cXMLNode, self, root);
  return(node);
}


/*
 * call-seq:
 *    document.root = node
 * 
 * Set the root node.
 */
VALUE
ruby_xml_document_root_set(VALUE self, VALUE node) {
  ruby_xml_document *rxd;
  ruby_xml_node *rxn;
  VALUE retnode;
  xmlNodePtr root;

  if (rb_obj_is_kind_of(node, cXMLNode) == Qfalse)
    rb_raise(rb_eTypeError, "must pass an XML::Node type object");

  Data_Get_Struct(self, ruby_xml_document, rxd);
  Data_Get_Struct(node, ruby_xml_node, rxn);
  ruby_xml_node_set_ptr(node, 1);
  root = xmlDocSetRootElement(rxd->doc, rxn->node);
  if (root == NULL)
    return(Qnil);

  retnode = ruby_xml_node_new_ptr(cXMLNode, self, root);
  return(retnode);
}


/*
 * call-seq:
 *    document.save(format = false)
 * 
 * Save this document to the file given by filename, 
 * optionally formatting the output.
 */
VALUE
ruby_xml_document_save(int argc, VALUE *argv, VALUE self) {
  ruby_xml_document *rxd;
  const char *filename;
  int format, len;

  format = 0;
  switch (argc) {
  case 1:
    break;
  case 2:
    if (TYPE(argv[1]) == T_TRUE)
      format = 1;
    else if (TYPE(argv[1]) == T_FALSE)
      format = 0;
    else
      rb_raise(rb_eTypeError, "wrong type of argument, must be bool");
    break;
  default:
    rb_raise(rb_eArgError, "wrong number of arguments (0 or 1)");
  }

  Check_Type(argv[0], T_STRING);
  filename = StringValuePtr(argv[0]);

  Data_Get_Struct(self, ruby_xml_document, rxd);
  len = xmlSaveFormatFileEnc(filename, rxd->doc, (const char*)rxd->doc->encoding, format);
  if (len == -1)
    rb_fatal("Unable to write out file");
  else
    return(INT2NUM(len));
}


/*
 * call-seq:
 *    document.standalone? => (true|false)
 * 
 * Determine whether this is a standalone document.
 */
VALUE
ruby_xml_document_standalone_q(VALUE self) {
  ruby_xml_document *rxd;
  Data_Get_Struct(self, ruby_xml_document, rxd);
  if (rxd->doc->standalone)
    return(Qtrue);
  else
    return(Qfalse);
}


/*
 * call-seq:
 *    document.to_s => "xml"
 * 
 * Coerce this document to a string representation
 * of it's XML.
 */
VALUE
ruby_xml_document_to_s(int argc, VALUE *argv, VALUE self) {
  ruby_xml_document *rxd;
  xmlChar *result;
  int format, len;

  switch (argc) {
  case 0:
    format = 1;
    break;
  case 1:
    if (TYPE(argv[0]) == T_TRUE)
      format = 1;
    else if (TYPE(argv[0]) == T_FALSE)
      format = 0;
    else
      rb_raise(rb_eTypeError, "wrong type of argument, must be bool");
    break;
  default:
    rb_raise(rb_eArgError, "wrong number of arguments (0 or 1)");
  }

  Data_Get_Struct(self, ruby_xml_document, rxd);
  if (rxd->doc == NULL) {
    return(Qnil);
  } else if (rxd->doc->encoding != NULL) {
    if (format) {
      xmlDocDumpFormatMemoryEnc(rxd->doc, &result, &len,
				(const char*)rxd->doc->encoding, format);
    } else {
      xmlDocDumpMemoryEnc(rxd->doc, &result, &len,
			  (const char*)rxd->doc->encoding);
    }
  } else {
    if (format)
      xmlDocDumpFormatMemory(rxd->doc, &result, &len, format);
    else
      xmlDocDumpMemory(rxd->doc, &result, &len);
  }

  return(rb_str_new2((const char*)result)); 
}


/*
 * call-seq:
 *    document.url => "url"
 * 
 * Obtain this document's source URL, if any.
 */
VALUE
ruby_xml_document_url_get(VALUE self) {
  ruby_xml_document *rxd;
  Data_Get_Struct(self, ruby_xml_document, rxd);
  if (rxd->doc->URL == NULL)
    return(Qnil);
  else
    return(rb_str_new2((const char*)rxd->doc->URL));
}


/*
 * call-seq:
 *    document.version => "version"
 * 
 * Obtain the XML version specified by this document.
 */
VALUE
ruby_xml_document_version_get(VALUE self) {
  ruby_xml_document *rxd;
  Data_Get_Struct(self, ruby_xml_document, rxd);
  if (rxd->doc->version == NULL)
    return(Qnil);
  else
    return(rb_str_new2((const char*)rxd->doc->version));
}


/*
 * call-seq:
 *    document.xinclude => num
 * 
 * Process xinclude directives in this document. 
 */
VALUE
ruby_xml_document_xinclude(VALUE self) {
#ifdef LIBXML_XINCLUDE_ENABLED
  ruby_xml_document *rxd;
  int ret;

  Data_Get_Struct(self, ruby_xml_document, rxd);
  ret = xmlXIncludeProcess(rxd->doc);
  if (ret >= 0)
    return(INT2NUM(ret));
  else
    rb_raise(eXMLXIncludeError, "error processing xinclude directives in document");
#else
  rb_warn("libxml was compiled without XInclude support.  Please recompile libxml and ruby-libxml");
  return(Qfalse);
#endif
}

void
LibXML_validity_error(void * ctxt, const char * msg, va_list ap)
{
  if (rb_block_given_p()) {
    char buff[1024];
    snprintf(buff, 1024, msg, ap);
    rb_yield(rb_ary_new3(2, rb_str_new2(buff), Qtrue));
  } else {
    fprintf(stderr, "error -- found validity error: ");
    fprintf(stderr, msg, ap);
  }
}

void
LibXML_validity_warning(void * ctxt, const char * msg, va_list ap)
{
  if (rb_block_given_p()) {
    char buff[1024];
    snprintf(buff, 1024, msg, ap);
    rb_yield(rb_ary_new3(2, rb_str_new2(buff), Qfalse));
  } else {
    fprintf(stderr, "warning -- found validity error: ");
    fprintf(stderr, msg, ap);
  }
}

/*
 * call-seq:
 *    document.validate(schema) => (true|false)
 * 
 * Validate this document against the specified XML::Schema.
 */
VALUE
ruby_xml_document_validate_schema(VALUE self, VALUE schema) {
  xmlSchemaValidCtxtPtr vptr;
  ruby_xml_document     *c_doc;
  ruby_xml_schema       *c_schema;
  int is_invalid;

  Data_Get_Struct(self,   ruby_xml_document, c_doc);
  Data_Get_Struct(schema, ruby_xml_schema,   c_schema);

  vptr = xmlSchemaNewValidCtxt(c_schema->schema);

  xmlSchemaSetValidErrors(vptr, (xmlSchemaValidityErrorFunc)LibXML_validity_error,
                                (xmlSchemaValidityWarningFunc)LibXML_validity_warning, NULL);
  
  is_invalid = xmlSchemaValidateDoc(vptr, c_doc->doc);
  xmlSchemaFreeValidCtxt(vptr);
  if (is_invalid) {
	return Qfalse;
  } else {
	return Qtrue;
  }
}


/*
 * call-seq:
 *    document.validate(schema) => (true|false)
 * 
 * Validate this document against the specified XML::DTD.
 */
VALUE
ruby_xml_document_validate_dtd(VALUE self, VALUE dtd) {
  xmlValidCtxt cvp;
  ruby_xml_document *c_doc;
  ruby_xml_dtd *c_dtd;

  Data_Get_Struct(self, ruby_xml_document, c_doc);
  Data_Get_Struct(dtd,  ruby_xml_dtd,      c_dtd);

  cvp.userData = NULL;
  cvp.error = (xmlValidityErrorFunc)LibXML_validity_error;
  cvp.warning = (xmlValidityWarningFunc)LibXML_validity_warning;

  cvp.nodeNr = 0;
  cvp.nodeTab = NULL;
  cvp.vstateNr = 0;
  cvp.vstateTab = NULL;

  if ( xmlValidateDtd(&cvp, c_doc->doc, c_dtd->dtd) )
    return(Qtrue);
  else
    return(Qfalse);

//    int	xmlValidateDtd(xmlValidCtxtPtr ctxt, xmlDocPtr doc, xmlDtdPtr dtd)
/*
int
validate(self, ...)
        xmlDocPtr self
    PREINIT:
        xmlValidCtxt cvp;
        xmlDtdPtr dtd;
        SV * dtd_sv;
        STRLEN n_a, len;
    CODE:
        LibXML_init_error();
        cvp.userData = (void*)PerlIO_stderr();
        cvp.error = (xmlValidityErrorFunc)LibXML_validity_error;
        cvp.warning = (xmlValidityWarningFunc)LibXML_validity_warning;
        // we need to initialize the node stack, because perl might 
        // already messed it up.
        //
        cvp.nodeNr = 0;
        cvp.nodeTab = NULL;
        cvp.vstateNr = 0;
        cvp.vstateTab = NULL;

        if (items > 1) {
            dtd_sv = ST(1);
            if ( sv_isobject(dtd_sv) && (SvTYPE(SvRV(dtd_sv)) == SVt_PVMG) ) {
                dtd = (xmlDtdPtr)PmmSvNode(dtd_sv);
            }
            else {
                croak("is_valid: argument must be a DTD object");
            }
            RETVAL = xmlValidateDtd(&cvp, self , dtd);
        }
        else {
            RETVAL = xmlValidateDocument(&cvp, self);
        }
        sv_2mortal(LibXML_error);

        if (RETVAL == 0) {
            LibXML_croak_error();
        }
    OUTPUT:
        RETVAL
*/
}

/*
 * call-seq:
 *    document.reader => reader
 * 
 * Create a XML::Reader from the document. This is a shortcut to
 * XML::Reader.walker().
 */
static VALUE
ruby_xml_document_reader(VALUE self)
{
  return ruby_xml_reader_new_walker(cXMLReader, self);
}

// Rdoc needs to know 
#ifdef RDOC_NEVER_DEFINED
  mXML = rb_define_module("XML");
#endif

void
ruby_init_xml_document(void) {
  cXMLDocument = rb_define_class_under(mXML, "Document", rb_cObject);
  rb_define_singleton_method(cXMLDocument, "file", ruby_xml_document_new_file, 1);
  rb_define_singleton_method(cXMLDocument, "new", ruby_xml_document_initialize, -1);

  //rb_raise(eXMLNodeFailedModify, "unable to add a child to the document");
  //eDTDValidityWarning = rb_define_class_under(cXMLNode, "ValidityWarning", eXMLError);
  //eDTDValidityError   = rb_define_class_under(cXMLNode, "ValidityWarning", eXMLError);
  rb_define_method(cXMLDocument, "[]", ruby_xml_document_property_get, 1);
  rb_define_method(cXMLDocument, "[]=", ruby_xml_document_property_set, 2);
  rb_define_method(cXMLDocument, "child", ruby_xml_document_child_get, 0);
  rb_define_method(cXMLDocument, "child?", ruby_xml_document_child_q, 0);
  rb_define_method(cXMLDocument, "compression", ruby_xml_document_compression_get, 0);
  rb_define_method(cXMLDocument, "compression=", ruby_xml_document_compression_set, 1);
  rb_define_method(cXMLDocument, "compression?", ruby_xml_document_compression_q, 0);
  rb_define_method(cXMLDocument, "dump", ruby_xml_document_dump, -1);
  rb_define_method(cXMLDocument, "debug_dump", ruby_xml_document_debug_dump, -1);
  rb_define_method(cXMLDocument, "debug_dump_head", ruby_xml_document_debug_dump_head, -1);
  rb_define_method(cXMLDocument, "debug_format_dump", ruby_xml_document_debug_format_dump, -1);
  rb_define_method(cXMLDocument, "encoding", ruby_xml_document_encoding_get, 0);
  rb_define_method(cXMLDocument, "encoding=", ruby_xml_document_encoding_set, 1);
  rb_define_method(cXMLDocument, "filename", ruby_xml_document_filename_get, 0);
  rb_define_method(cXMLDocument, "find", ruby_xml_document_find, -1);
  rb_define_method(cXMLDocument, "format_dump", ruby_xml_document_format_dump, -1);
  rb_define_method(cXMLDocument, "last", ruby_xml_document_last_get, 0);
  rb_define_method(cXMLDocument, "last?", ruby_xml_document_last_q, 0);
  rb_define_method(cXMLDocument, "next", ruby_xml_document_next_get, 0);
  rb_define_method(cXMLDocument, "next?", ruby_xml_document_next_q, 0);
  rb_define_method(cXMLDocument, "parent", ruby_xml_document_parent_get, 0);
  rb_define_method(cXMLDocument, "parent?", ruby_xml_document_parent_q, 0);
  rb_define_method(cXMLDocument, "prev", ruby_xml_document_prev_get, 0);
  rb_define_method(cXMLDocument, "prev?", ruby_xml_document_prev_q, 0);
  rb_define_method(cXMLDocument, "root", ruby_xml_document_root_get, 0);
  rb_define_method(cXMLDocument, "root=", ruby_xml_document_root_set, 1);
  rb_define_method(cXMLDocument, "save", ruby_xml_document_save, -1);
  rb_define_method(cXMLDocument, "standalone?", ruby_xml_document_standalone_q, 0);
  rb_define_method(cXMLDocument, "to_s", ruby_xml_document_to_s, -1);
  rb_define_method(cXMLDocument, "url", ruby_xml_document_url_get, 0);
  rb_define_method(cXMLDocument, "version", ruby_xml_document_version_get, 0);
  rb_define_method(cXMLDocument, "xinclude", ruby_xml_document_xinclude, 0);
  rb_define_method(cXMLDocument, "validate", ruby_xml_document_validate_dtd, 1);
  rb_define_method(cXMLDocument, "validate_schema", ruby_xml_document_validate_schema, 1);
  rb_define_method(cXMLDocument, "reader", ruby_xml_document_reader, 0);
}
