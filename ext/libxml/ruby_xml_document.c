/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

/*
* Document-class: LibXML::XML::Document
*
* Reads or writes an document:
*
* Reading:
*  require 'xml'
*
*  doc = XML::Document.new()
*  doc.root = XML::Node.new('root_node')
*  doc.root << XML::Node.new('elem1')
*  doc.save('output.xml', format)
* 
* Writing:
*  require 'libxml'
*  doc = XML::Document.file('output.xml')
*  root = doc.root
*/

#include <stdarg.h>
#include <st.h>
#include "ruby_libxml.h"
#include "ruby_xml_document.h"


VALUE cXMLDocument;


/* We have to make sure that documents are freed after
   XPathObjects.  The Ruby gc does not guarantee this
   at shutdown or in normal operation when a document,
   xpath context and xpath object form a cycle.  So
   we implement this simple reference counting scheme.
   It works as you'd expect, except with a simple twist.
   If a document is freed, but still has outstanding
   references, its reference count is made negative.  Thus
   if it has ref_count of 1, then its becomes -1.  At that
   point, decrement will increase the account to 0, and 
   once it reaches 0, the document is freed. */

static st_table *ref_count_table = 0;

void
ruby_xml_document_internal_free(xmlDocPtr xdoc) {
  st_delete(ref_count_table, &xdoc, 0);
  xdoc->_private = NULL;
  xmlFreeDoc(xdoc);
}

int
ruby_xml_document_incr(xmlDocPtr xdoc) {
  int ref_count;

  if (st_lookup(ref_count_table, xdoc, &ref_count))
  {
    ref_count++;
    st_insert(ref_count_table, xdoc, ref_count);
  }
  else
  {
    ref_count = 1;
    st_add_direct(ref_count_table, xdoc, ref_count);
  }
  
  return ref_count;
}

int
ruby_xml_document_decr(xmlDocPtr xdoc) {
  int ref_count = 0;

  if (!st_lookup(ref_count_table, xdoc, &ref_count)) 
    rb_raise(rb_eRuntimeError, "Document does not have a reference count.");

  if (ref_count == 0)
  {
    rb_raise(rb_eRuntimeError, "Document already has no references.");
  }
  else if (ref_count > 0)
  {
    ref_count--;
  }
  else
  {
    ref_count++;
    if (ref_count == 0)
    {
      ruby_xml_document_internal_free(xdoc);
      return ref_count;
    }
  }

  st_insert(ref_count_table, xdoc, ref_count);
  return ref_count;
}

void
ruby_xml_document_free(xmlDocPtr xdoc) {
  int ref_count;

  if (!st_lookup(ref_count_table, xdoc, &ref_count))
  {
    ruby_xml_document_internal_free(xdoc);
  }
  else if (ref_count == 0)
  {
    ruby_xml_document_internal_free(xdoc);
  }
  else if (ref_count > 0)
  {
    ref_count *= -1;
    st_insert(ref_count_table, xdoc, ref_count);
  }
  else 
  {
    rb_raise(rb_eRuntimeError, "Ruby is attempting to free document twice.");
  }
}   
 
void
ruby_xml_document_mark(xmlDocPtr xdoc) {
  rb_gc_mark(LIBXML_STATE);
}

VALUE
ruby_xml_document_wrap(xmlDocPtr xdoc) {
  VALUE result;

  // This node is already wrapped
  if (xdoc->_private != NULL)
  {
    result = (VALUE)xdoc->_private;
  }
  else  
  {
    result = Data_Wrap_Struct(cXMLDocument, ruby_xml_document_mark, ruby_xml_document_free, xdoc);
    xdoc->_private = (void*)result;
  }
     
  return result;
}


/*
 * call-seq:
 *    XML::Document.alloc(xml_version = 1.0) -> document
 * 
 * Alocates a new XML::Document, optionally specifying the
 * XML version.
 */
VALUE
ruby_xml_document_alloc(VALUE klass) {
  return Data_Wrap_Struct(klass, ruby_xml_document_mark, ruby_xml_document_free, NULL);
}

/*
 * call-seq:
 *    XML::Document.initialize(xml_version = 1.0) -> document
 * 
 * Initializes a new XML::Document, optionally specifying the
 * XML version.
 */
VALUE
ruby_xml_document_initialize(int argc, VALUE *argv, VALUE self) {
  xmlDocPtr xdoc;
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

  Check_Type(xmlver, T_STRING);
  xdoc = xmlNewDoc((xmlChar*)StringValuePtr(xmlver));
  xdoc->_private = (void*)self;
  DATA_PTR(self) = xdoc; 
  
  return self;
}


/*
 * call-seq:
 *    document.compression -> num
 * 
 * Obtain this document's compression mode identifier.
 */
VALUE
ruby_xml_document_compression_get(VALUE self) {
#ifdef HAVE_ZLIB_H
  xmlDocPtr xdoc;

  int compmode;
  Data_Get_Struct(self, xmlDoc, xdoc);

  compmode = xmlGetDocCompressMode(xdoc);
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
  xmlDocPtr xdoc;

  int compmode;
  Check_Type(num, T_FIXNUM);
  Data_Get_Struct(self, xmlDoc, xdoc);

  if (xdoc == NULL) {
    return(Qnil);
  } else {
    xmlSetDocCompressMode(xdoc, NUM2INT(num));

    compmode = xmlGetDocCompressMode(xdoc);
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
 *    document.compression? -> (true|false)
 * 
 * Determine whether this document is compressed.
 */
VALUE
ruby_xml_document_compression_q(VALUE self) {
#ifdef HAVE_ZLIB_H
  xmlDocPtr xdoc;

  Data_Get_Struct(self, xmlDoc, xdoc);

  if (xdoc->compression != -1)
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
 *    document.child -> node
 * 
 * Get this document's child node.
 */
VALUE
ruby_xml_document_child_get(VALUE self) {
  xmlDocPtr xdoc;
  Data_Get_Struct(self, xmlDoc, xdoc);

  if (xdoc->children == NULL)
    return(Qnil);

  return ruby_xml_node2_wrap(cXMLNode, xdoc->children);
}


/*
 * call-seq:
 *    document.child? -> (true|false)
 * 
 * Determine whether this document has a child node.
 */
VALUE
ruby_xml_document_child_q(VALUE self) {
  xmlDocPtr xdoc;
  Data_Get_Struct(self, xmlDoc, xdoc);

  if (xdoc->children == NULL)
    return(Qfalse);
  else
    return(Qtrue);
}


/*
 * call-seq:
 *    document.dump([stream]) -> true
 * 
 * Dump this document's XML to the specified IO stream.
 * If no stream is specified, stdout is used.
 */
VALUE
ruby_xml_document_dump(int argc, VALUE *argv, VALUE self) {
  OpenFile *fptr;
  VALUE io;
  FILE *out;
  xmlDocPtr xdoc;

  Data_Get_Struct(self, xmlDoc, xdoc);
  if (xdoc == NULL)
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
  xmlDocDump(out, xdoc);
  return(Qtrue);
}


/*
 * call-seq:
 *    document.debug_dump([stream]) -> true
 * 
 * Debug version of dump.
 */
VALUE
ruby_xml_document_debug_dump(int argc, VALUE *argv, VALUE self) {
#ifdef LIBXML_DEBUG_ENABLED
  OpenFile *fptr;
  VALUE io;
  FILE *out;
  xmlDocPtr xdoc;

  Data_Get_Struct(self, xmlDoc, xdoc);
  if (xdoc == NULL)
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
  xmlDebugDumpDocument(out, xdoc);
  return(Qtrue);
#else
  rb_warn("libxml was compiled without debugging support.  Please recompile libxml and ruby-libxml");
  return(Qfalse);
#endif
}


/*
 * call-seq:
 *    document.debug_dump_head([stream]) -> true
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
  xmlDocPtr xdoc;


  Data_Get_Struct(self, xmlDoc, xdoc);
  if (xdoc == NULL)
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
  xmlDebugDumpDocumentHead(out, xdoc);
  return(Qtrue);
#else
  rb_warn("libxml was compiled without debugging support.  Please recompile libxml and ruby-libxml");
  return(Qfalse);
#endif
}


/*
 * call-seq:
 *    document.format_dump([stream], [spacing]) -> true
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
  xmlDocPtr xdoc;

  int size, spacing;

  Data_Get_Struct(self, xmlDoc, xdoc);
  if (xdoc == NULL)
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
  size = xmlDocFormatDump(out, xdoc, spacing);
  return(INT2NUM(size));
}


/*
 * call-seq:
 *    document.debug_format_dump([stream]) -> true
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
 *    document.encoding -> "encoding"
 * 
 * Obtain the encoding specified by this document.
 */
VALUE
ruby_xml_document_encoding_get(VALUE self) {
  xmlDocPtr xdoc;

  Data_Get_Struct(self, xmlDoc, xdoc);
  if (xdoc->encoding == NULL)
    return(Qnil);
  else
    return(rb_str_new2((const char*)xdoc->encoding));
}


/*
 * call-seq:
 *    document.encoding = "encoding"
 * 
 * Set the encoding for this document.
 */
VALUE
ruby_xml_document_encoding_set(VALUE self, VALUE encoding) {
  xmlDocPtr xdoc;


  Check_Type(encoding, T_STRING);
  Data_Get_Struct(self, xmlDoc, xdoc);
  xdoc->encoding = xmlStrdup(StringValuePtr(encoding));
  return(ruby_xml_document_encoding_get(self));
}

/*
 * call-seq:
 *    document.last -> node
 * 
 * Obtain the last node.
 */
VALUE
ruby_xml_document_last_get(VALUE self) {
  xmlDocPtr xdoc;


  Data_Get_Struct(self, xmlDoc, xdoc);

  if (xdoc->last == NULL)
    return(Qnil);

  return ruby_xml_node2_wrap(cXMLNode, xdoc->last);
}


/*
 * call-seq:
 *    document.last? -> (true|false)
 * 
 * Determine whether there is a last node.
 */
VALUE
ruby_xml_document_last_q(VALUE self) {
  xmlDocPtr xdoc;

  Data_Get_Struct(self, xmlDoc, xdoc);

  if (xdoc->last == NULL)
    return(Qfalse);
  else
    return(Qtrue);
}


/*
 * call-seq:
 *    document.next -> node
 * 
 * Obtain the next node.
 */
VALUE
ruby_xml_document_next_get(VALUE self) {
  xmlDocPtr xdoc;


  Data_Get_Struct(self, xmlDoc, xdoc);

  if (xdoc->next == NULL)
    return(Qnil);

  return ruby_xml_node2_wrap(cXMLNode, xdoc->next);
}


/*
 * call-seq:
 *    document.next? -> (true|false)
 * 
 * Determine whether there is a next node.
 */
VALUE
ruby_xml_document_next_q(VALUE self) {
  xmlDocPtr xdoc;

  Data_Get_Struct(self, xmlDoc, xdoc);

  if (xdoc->next == NULL)
    return(Qfalse);
  else
    return(Qtrue);
}


/*
 * call-seq:
 *    document.parent -> node
 * 
 * Obtain the parent node.
 */
VALUE
ruby_xml_document_parent_get(VALUE self) {
  xmlDocPtr xdoc;


  Data_Get_Struct(self, xmlDoc, xdoc);

  if (xdoc->parent == NULL)
    return(Qnil);

  return ruby_xml_node2_wrap(cXMLNode, xdoc->parent);
}


/*
 * call-seq:
 *    document.parent? -> (true|false)
 * 
 * Determine whether there is a parent node.
 */
VALUE
ruby_xml_document_parent_q(VALUE self) {
  xmlDocPtr xdoc;

  Data_Get_Struct(self, xmlDoc, xdoc);

  if (xdoc->parent == NULL)
    return(Qfalse);
  else
    return(Qtrue);
}


/*
 * call-seq:
 *    document.prev -> node
 * 
 * Obtain the previous node.
 */
VALUE
ruby_xml_document_prev_get(VALUE self) {
  xmlDocPtr xdoc;


  Data_Get_Struct(self, xmlDoc, xdoc);

  if (xdoc->prev == NULL)
    return(Qnil);

  return ruby_xml_node2_wrap(cXMLNode, xdoc->prev);
}


/*
 * call-seq:
 *    document.prev? -> (true|false)
 * 
 * Determine whether there is a previous node.
 */
VALUE
ruby_xml_document_prev_q(VALUE self) {
  xmlDocPtr xdoc;

  Data_Get_Struct(self, xmlDoc, xdoc);

  if (xdoc->prev == NULL)
    return(Qfalse);
  else
    return(Qtrue);
}


/*
 * call-seq:
 *    document.root -> node
 * 
 * Obtain the root node.
 */
VALUE
ruby_xml_document_root_get(VALUE self) {
  xmlDocPtr xdoc;

  xmlNodePtr root;

  Data_Get_Struct(self, xmlDoc, xdoc);
  root = xmlDocGetRootElement(xdoc);

  if (root == NULL)
    return(Qnil);

  return ruby_xml_node2_wrap(cXMLNode, root);
}


/*
 * call-seq:
 *    document.root = node
 * 
 * Set the root node.
 */
VALUE
ruby_xml_document_root_set(VALUE self, VALUE node) {
  xmlDocPtr xdoc;
  xmlNodePtr xroot, xnode;

  if (rb_obj_is_kind_of(node, cXMLNode) == Qfalse)
    rb_raise(rb_eTypeError, "must pass an XML::Node type object");

  Data_Get_Struct(self, xmlDoc, xdoc);
  Data_Get_Struct(node, xmlNode, xnode);
  xroot = xmlDocSetRootElement(xdoc, xnode);
  if (xroot == NULL)
    return(Qnil);

  return ruby_xml_node2_wrap(cXMLNode, xroot);
}


/*
 * call-seq:
 *    document.save(filename, format = false) -> int
 * 
 * Save this document to the file given by filename, 
 * optionally formatting the output.
 
 * Parameters:
 *  filename: The filename or URL of the new document
 *  format: Specifies whether formatting spaces should be added.
 *  returns: The number of bytes written or -1 in case of error. 
 */
VALUE
ruby_xml_document_save(int argc, VALUE *argv, VALUE self) {
  xmlDocPtr xdoc;

  const char *filename;
  int format = 0;
  int len;

  if (argc <1 || argc > 2)
    rb_raise(rb_eArgError, "wrong number of arguments (need 1 or 2)");

  Check_Type(argv[0], T_STRING);
  filename = StringValuePtr(argv[0]);
  
  if (argc == 2)
  {
    switch (TYPE(argv[1])) {
      case T_TRUE:
        format = 1;
        break;
      case T_FALSE:
        format = 0;
        break;
      default:
        rb_raise(rb_eArgError, "The second parameter (format) must be true or false");
    }
  }

  Data_Get_Struct(self, xmlDoc, xdoc);
  len = xmlSaveFormatFileEnc(filename, xdoc, (const char*)xdoc->encoding, format);
  
  if (len == -1)
    rb_raise(rb_eIOError, "Could not write document");
  else
    return(INT2NUM(len));
}


/*
 * call-seq:
 *    document.standalone? -> (true|false)
 * 
 * Determine whether this is a standalone document.
 */
VALUE
ruby_xml_document_standalone_q(VALUE self) {
  xmlDocPtr xdoc;

  Data_Get_Struct(self, xmlDoc, xdoc);
  if (xdoc->standalone)
    return(Qtrue);
  else
    return(Qfalse);
}


/*
 * call-seq:
 *    document.to_s({format=true,encoding) -> "xml"
 * 
 * Coerce this document to a string representation
 * of it's XML. The default is to pretty format, but this
 * depends Parser#indent_tree_output==true or
 * Parser#default_keep_blanks==false.
 *
 * The encoding is not applied to the document, but is
 * encoding target of the resulting string.
 */
VALUE
ruby_xml_document_to_s(int argc, VALUE *argv, VALUE self) {
  xmlDocPtr xdoc;

  xmlChar *result, *encoding=NULL;
  int format, len;
  VALUE rresult;

  switch (argc) {
  case 0:
    format = 1;
    break;
  case 2:
    if (TYPE(argv[1]) == T_STRING)
      encoding=(xmlChar *)StringValuePtr(argv[1]);
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

  Data_Get_Struct(self, xmlDoc, xdoc);
  if (xdoc == NULL) {
    return(Qnil);
  } else if (encoding != NULL) {
    if (format) {
      xmlDocDumpFormatMemoryEnc(xdoc, &result, &len,
				(const char*)encoding, format);
    } else {
      xmlDocDumpMemoryEnc(xdoc, &result, &len,
			  (const char *)encoding);
    }
  } else {
    if (format)
      xmlDocDumpFormatMemory(xdoc, &result, &len, format);
    else
      xmlDocDumpMemory(xdoc, &result, &len);
  }
  rresult=rb_str_new((const char*)result,len);
  xmlFree(result);
  return rresult;
}


/*
 * call-seq:
 *    document.url -> "url"
 * 
 * Obtain this document's source URL, if any.
 */
VALUE
ruby_xml_document_url_get(VALUE self) {
  xmlDocPtr xdoc;

  Data_Get_Struct(self, xmlDoc, xdoc);
  if (xdoc->URL == NULL)
    return(Qnil);
  else
    return(rb_str_new2((const char*)xdoc->URL));
}


/*
 * call-seq:
 *    document.version -> "version"
 * 
 * Obtain the XML version specified by this document.
 */
VALUE
ruby_xml_document_version_get(VALUE self) {
  xmlDocPtr xdoc;

  Data_Get_Struct(self, xmlDoc, xdoc);
  if (xdoc->version == NULL)
    return(Qnil);
  else
    return(rb_str_new2((const char*)xdoc->version));
}


/*
 * call-seq:
 *    document.xinclude -> num
 * 
 * Process xinclude directives in this document. 
 */
VALUE
ruby_xml_document_xinclude(VALUE self) {
#ifdef LIBXML_XINCLUDE_ENABLED
  xmlDocPtr xdoc;

  int ret;

  Data_Get_Struct(self, xmlDoc, xdoc);
  ret = xmlXIncludeProcess(xdoc);
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
 *    document.validate_schema(schema) -> (true|false)
 * 
 * Validate this document against the specified XML::Schema.
 * 
 * If a block is provided it is used as an error handler for validaten errors.
 * The block is called with two argument, the message and a flag indication
 * if the message is an error (true) or a warning (false).
 */
VALUE
ruby_xml_document_validate_schema(VALUE self, VALUE schema) {
  xmlSchemaValidCtxtPtr vptr;
  xmlDocPtr xdoc;
  ruby_xml_schema *c_schema;
  int is_invalid;

  Data_Get_Struct(self, xmlDoc, xdoc);
  Data_Get_Struct(schema, ruby_xml_schema, c_schema);

  vptr = xmlSchemaNewValidCtxt(c_schema->schema);

  xmlSchemaSetValidErrors(vptr, (xmlSchemaValidityErrorFunc)LibXML_validity_error,
                                (xmlSchemaValidityWarningFunc)LibXML_validity_warning, NULL);
  
  is_invalid = xmlSchemaValidateDoc(vptr, xdoc);
  xmlSchemaFreeValidCtxt(vptr);
  if (is_invalid) 
  {
    ruby_xml_raise(&xmlLastError);
    return Qfalse;
  } else
  {
	  return Qtrue;
  }
}

/*
 * call-seq:
 *    document.validate_schema(relaxng) -> (true|false)
 * 
 * Validate this document against the specified XML::RelaxNG.
 *
 * If a block is provided it is used as an error handler for validaten errors.
 * The block is called with two argument, the message and a flag indication
 * if the message is an error (true) or a warning (false).
 */
VALUE
ruby_xml_document_validate_relaxng(VALUE self, VALUE relaxng) {
  xmlRelaxNGValidCtxtPtr vptr;
  xmlDocPtr xdoc;
  ruby_xml_relaxng *c_relaxng;
  int is_invalid;

  Data_Get_Struct(self, xmlDoc, xdoc);
  Data_Get_Struct(relaxng, ruby_xml_relaxng, c_relaxng);

  vptr = xmlRelaxNGNewValidCtxt(c_relaxng->relaxng);

  xmlRelaxNGSetValidErrors(vptr, (xmlRelaxNGValidityErrorFunc)LibXML_validity_error,
                                (xmlRelaxNGValidityWarningFunc)LibXML_validity_warning, NULL);
  
  is_invalid = xmlRelaxNGValidateDoc(vptr, xdoc);
  xmlRelaxNGFreeValidCtxt(vptr);
  if (is_invalid) 
  {
    ruby_xml_raise(&xmlLastError);
    return Qfalse;
  } else 
  {
	  return Qtrue;
  }
}


/*
 * call-seq:
 *    document.validate(dtd) -> (true|false)
 * 
 * Validate this document against the specified XML::DTD.
 */
VALUE
ruby_xml_document_validate_dtd(VALUE self, VALUE dtd) {
  VALUE error = Qnil;
  xmlValidCtxt ctxt;
  xmlDocPtr xdoc;
  ruby_xml_dtd *c_dtd;

  Data_Get_Struct(self, xmlDoc, xdoc);
  Data_Get_Struct(dtd, ruby_xml_dtd, c_dtd);

  ctxt.userData = &error;
  ctxt.error = (xmlValidityErrorFunc)LibXML_validity_error;
  ctxt.warning = (xmlValidityWarningFunc)LibXML_validity_warning;

  ctxt.nodeNr = 0;
  ctxt.nodeTab = NULL;
  ctxt.vstateNr = 0;
  ctxt.vstateTab = NULL;

  if (xmlValidateDtd(&ctxt, xdoc, c_dtd->dtd))
  {
    return(Qtrue);
  }
  else
  {
    ruby_xml_raise(&xmlLastError);
    return Qfalse;
  }
}


/*
 * call-seq:
 *    document.reader -> reader
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
  mLibXML = rb_define_module("LibXML");
  mXML = rb_define_module_under(mLibXML, "XML");
#endif

void
ruby_init_xml_document(void) {
  ref_count_table = st_init_numtable();

  cXMLDocument = rb_define_class_under(mXML, "Document", rb_cObject);
  rb_define_alloc_func(cXMLDocument, ruby_xml_document_alloc);

  rb_define_method(cXMLDocument, "initialize", ruby_xml_document_initialize, -1);
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
  rb_define_method(cXMLDocument, "validate_relaxng", ruby_xml_document_validate_relaxng, 1);
  rb_define_method(cXMLDocument, "reader", ruby_xml_document_reader, 0);
}
