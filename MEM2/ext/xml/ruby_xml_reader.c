/* Copyright (c) 2006-2007 Apple Inc.
 * Please see the LICENSE file for copyright and distribution information. */

#include "libxml.h"
#include "ruby_xml_reader.h"

#define CSTR2RVAL(x)  (x == NULL ? Qnil : rb_str_new2((const char *)x))
#define RVAL2CSTR(x)  (StringValueCStr(x))

static inline VALUE 
__rb_str_new_and_free(xmlChar *x)
{
  if (x != NULL) {
    VALUE v = rb_str_new2((const char *)x);
    xmlFree(x);
    return v;
  }
  return Qnil;
}

#define CSTR2RVAL2(x) (__rb_str_new_and_free(x))

VALUE cXMLReader;
static ID error_handler_block_ivar_id;

static VALUE
ruby_xml_reader_new(VALUE class, xmlTextReaderPtr reader)
{
  return Data_Wrap_Struct(class, NULL, xmlFreeTextReader, reader);
}

static xmlTextReaderPtr
__ruby_xml_parser_get(VALUE obj)
{
  xmlTextReaderPtr ptr;
  Data_Get_Struct(obj, xmlTextReader, ptr);
  return ptr;
}

#if defined(_SELF)
# undef _SELF
#endif
#define _SELF(x) (__ruby_xml_parser_get(x))

/*
 * call-seq:
 *    XML::Reader.file(path, encoding=nil, options=0) -> reader
 *
 * Parse an XML file from the filesystem or the network. The parsing flags 
 * options are a combination of xmlParserOption. 
 */
static VALUE
ruby_xml_reader_new_file(int argc, VALUE *argv, VALUE self)
{
  xmlTextReaderPtr reader;
  VALUE path, encoding, options;

  rb_scan_args(argc, argv, "12", &path, &encoding, &options);

  reader = xmlReaderForFile(RVAL2CSTR(path), 
                            NIL_P(encoding) ? NULL : RVAL2CSTR(encoding), 
                            NIL_P(options) ? 0 : FIX2INT(options));
  if (reader == NULL)
    rb_raise(rb_eRuntimeError, 
             "cannot create text reader for given XML file at path '%s'", 
             RVAL2CSTR(path));

  return ruby_xml_reader_new(self, reader); 
}

/*
 * call-seq:
 *    XML::Reader.walker(doc) -> reader
 *    XML::Reader.document(doc) -> reader
 *
 * Create an XML text reader for a preparsed document.
 */
VALUE
ruby_xml_reader_new_walker(VALUE self, VALUE doc)
{
  ruby_xml_document_t *rxd;
  xmlTextReaderPtr reader;
  
  Data_Get_Struct(doc, ruby_xml_document_t, rxd);
 
  reader = xmlReaderWalker(rxd->doc);  
  if (reader == NULL)
    rb_raise(rb_eRuntimeError, "cannot create text reader for given document");

  return ruby_xml_reader_new(self, reader); 
}

/*
 * call-seq:
 *    XML::Reader.new(data, url=nil, encoding=nil, options=0) -> reader
 *    XML::Reader.string(data, url=nil, encoding=nil, options=0) -> reader
 *
 * Create an XML text reader for an XML in-memory document. The parsing flags
 * options are a combination of xmlParserOption.
 */
static VALUE
ruby_xml_reader_new_data(int argc, VALUE *argv, VALUE self)
{
  xmlTextReaderPtr reader;
  VALUE data, url, encoding, options;
  char *c_data;

  rb_scan_args(argc, argv, "13", &data, &url, &encoding, &options);

  c_data = RVAL2CSTR(data);
  reader = xmlReaderForMemory(c_data,
                              strlen(c_data), 
                              NIL_P(url) ? NULL : RVAL2CSTR(url),
                              NIL_P(encoding) ? NULL : RVAL2CSTR(encoding), 
                              NIL_P(options) ? 0 : FIX2INT(options));
  if (reader == NULL)
    rb_raise(rb_eRuntimeError, "cannot create text reader for given data");

  return ruby_xml_reader_new(self, reader); 
}

/*
 * call-seq:
 *    parser.close -> code
 *
 * This method releases any resources allocated by the current instance 
 * changes the state to Closed and close any underlying input.
 */
static VALUE
ruby_xml_reader_close(VALUE self)
{
  return INT2FIX(xmlTextReaderClose(_SELF(self)));
}

/*
 * call-seq:
 *   parser.move_to_attribute(val) -> code
 *
 * Move the position of the current instance to the attribute with the 
 * specified index (if +val+ is an integer) or name (if +val+ is a string) 
 * relative to the containing element.
 */
static VALUE
ruby_xml_reader_move_to_attr(VALUE self, VALUE val)
{
  xmlTextReaderPtr reader;
  int ret;

  reader = _SELF(self);

  if (TYPE(val) == T_FIXNUM) {
    ret = xmlTextReaderMoveToAttributeNo(reader, FIX2INT(val));
  }
  else {
    ret = xmlTextReaderMoveToAttribute(reader, (const xmlChar *)RVAL2CSTR(val));
  }

  return INT2FIX(ret);
}

/*
 * call-seq:
 *    reader.move_to_first_attribute -> code
 *
 * Move the position of the current instance to the first attribute associated
 * with the current node.
 */
static VALUE
ruby_xml_reader_move_to_first_attr(VALUE self)
{
  return INT2FIX(xmlTextReaderMoveToFirstAttribute(_SELF(self)));
}

/*
 * call-seq:
 *    reader.move_to_next_attribute -> code
 *
 * Move the position of the current instance to the next attribute associated 
 * with the current node.
 */
static VALUE
ruby_xml_reader_move_to_next_attr(VALUE self)
{
  return INT2FIX(xmlTextReaderMoveToNextAttribute(_SELF(self)));
}

/*
 * call-seq:
 *    reader.move_to_element -> code
 *
 * Move the position of the current instance to the node that contains the 
 * current attribute node.
 */
static VALUE
ruby_xml_reader_move_to_element(VALUE self)
{
  return INT2FIX(xmlTextReaderMoveToElement(_SELF(self)));
}

/*
 * call-seq:
 *    reader.next -> code
 *
 * Skip to the node following the current one in document order while avoiding 
 * the subtree if any.
 */
static VALUE
ruby_xml_reader_next(VALUE self)
{
  return INT2FIX(xmlTextReaderNext(_SELF(self)));
}

/*
 * call-seq:
 *    reader.next_sibling -> code
 *
 * Skip to the node following the current one in document order while avoiding 
 * the subtree if any. Currently implemented only for Readers built on a 
 * document.
 */
static VALUE
ruby_xml_reader_next_sibling(VALUE self)
{
  return INT2FIX(xmlTextReaderNextSibling(_SELF(self)));
}

/*
 * call-seq:
 *    reader.node_type -> type
 *
 * Get the node type of the current node. Reference: 
 * http://dotgnu.org/pnetlib-doc/System/Xml/XmlNodeType.html
 */
static VALUE
ruby_xml_reader_node_type(VALUE self)
{
  return INT2FIX(xmlTextReaderNodeType(_SELF(self)));
}

/*
 * call-seq:
 *    reader.normalization -> value
 *
 * The value indicating whether to normalize white space and attribute values. 
 * Since attribute value and end of line normalizations are a MUST in the XML 
 * specification only the value true is accepted. The broken bahaviour of 
 * accepting out of range character entities like &#0; is of course not 
 * supported either.
 *
 * Return 1 or -1 in case of error.
 */
static VALUE
ruby_xml_reader_normalization(VALUE self)
{
  return INT2FIX(xmlTextReaderNormalization(_SELF(self)));
}

/*
 * call-seq:
 *    reader.read -> code
 *
 * Move the position of the current instance to the next node in the stream, 
 * exposing its properties.
 *
 * Return 1 if the node was read successfully, 0 if there is no more nodes to 
 * read, or -1 in case of error.
 */
static VALUE
ruby_xml_reader_read(VALUE self)
{
  return INT2FIX(xmlTextReaderRead(_SELF(self)));
}

/*
 * call-seq:
 *    reader.read_attribute_value -> code
 *
 * Parse an attribute value into one or more Text and EntityReference nodes.
 *
 * Return 1 in case of success, 0 if the reader was not positionned on an 
 * attribute node or all the attribute values have been read, or -1 in case of 
 * error.
 */
static VALUE
ruby_xml_reader_read_attr_value(VALUE self)
{
  return INT2FIX(xmlTextReaderReadAttributeValue(_SELF(self)));
}

/*
 * call-seq:
 *    reader.read_inner_xml -> data
 *
 * Read the contents of the current node, including child nodes and markup.
 *
 * Return a string containing the XML content, or nil if the current node is 
 * neither an element nor attribute, or has no child nodes. 
 */
static VALUE
ruby_xml_reader_read_inner_xml(VALUE self)
{
  return CSTR2RVAL2(xmlTextReaderReadInnerXml(_SELF(self)));
}

/*
 * call-seq:
 *    reader.read_outer_xml -> data
 *
 * Read the contents of the current node, including child nodes and markup.
 *
 * Return a string containing the XML content, or nil if the current node is 
 * neither an element nor attribute, or has no child nodes. 
 */
static VALUE
ruby_xml_reader_read_outer_xml(VALUE self)
{
  return CSTR2RVAL2(xmlTextReaderReadOuterXml(_SELF(self)));
}

/*
 * call-seq:
 *    reader.read_state -> state
 *
 * Get the read state of the reader.
 */
static VALUE
ruby_xml_reader_read_state(VALUE self)
{
  return INT2FIX(xmlTextReaderReadState(_SELF(self)));
}

/*
 * call-seq:
 *    reader.read_string -> string
 *
 * Read the contents of an element or a text node as a string.
 *
 * Return a string containing the contents of the Element or Text node, or nil 
 * if the reader is positioned on any other type of node.
 */
static VALUE
ruby_xml_reader_read_string(VALUE self)
{
  return CSTR2RVAL2(xmlTextReaderReadString(_SELF(self)));
}

static void
__xml_reader_error_cb(void *arg, 
                      const char *msg, xmlParserSeverities severity, 
                      xmlTextReaderLocatorPtr locator)
{
  VALUE reader;
  VALUE block;

  reader = (VALUE)arg;
  block = rb_ivar_get(reader, error_handler_block_ivar_id); 
  if (NIL_P(block))
    rb_bug("no ivar block");
  
  rb_funcall(block, 
             rb_intern("call"), 
             5, 
             reader, 
             CSTR2RVAL(msg), 
             INT2FIX(severity), 
             CSTR2RVAL2(xmlTextReaderLocatorBaseURI(locator)), 
             INT2FIX(xmlTextReaderLocatorLineNumber(locator))); 
}

/*
 * call-seq:
 *    reader.set_error_handler { ... }
 *
 * Register a callback block that will be called on error and warnings. The
 * block will be called with 5 parameters: the reader, the error message, the 
 * severity, the base URI and the line number.
 */
static VALUE
ruby_xml_reader_set_error_handler(VALUE self)
{
  VALUE block;
  xmlTextReaderPtr reader;

  if (rb_block_given_p() == Qfalse)
    rb_raise(rb_eRuntimeError, "No block given");

  block = rb_block_proc();
 
  /* Embed the block within the parser object to avoid it to be collected.
   * Previous handler if exits is overwritten. 
   */
  rb_ivar_set(self, error_handler_block_ivar_id, block); 
  reader = _SELF(self);
  xmlTextReaderSetErrorHandler(reader, __xml_reader_error_cb, (void *)self); 
  
  return self;
}

/*
 * call-seq:
 *    reader.reset_error_handler
 *
 * Restore the default error and warning handlers.
 */
static VALUE
ruby_xml_reader_reset_error_handler(VALUE self)
{
  xmlTextReaderSetErrorHandler(_SELF(self), NULL, NULL);
  return self;
}

/*
 * call-seq:
 *    reader.relax_ng_validate(rng) -> code
 *
 * Use RelaxNG to validate the document as it is processed. Activation is only 
 * possible before the first read. If +rng+ is nil, the RelaxNG validation is
 * desactivated.
 *
 * Return 0 in case the RelaxNG validation could be (des)activated and -1 in 
 * case of error.
 */ 
static VALUE
ruby_xml_reader_relax_ng_validate(VALUE self, VALUE rng)
{
  return INT2FIX(xmlTextReaderRelaxNGValidate(_SELF(self), NIL_P(rng) ? NULL : RVAL2CSTR(rng)));
}

#if LIBXML_VERSION >= 20620
/*
 * call-seq:
 *    reader.schema_validate(schema) -> code
 * 
 * Use W3C XSD schema to validate the document as it is processed. Activation 
 * is only possible before the first read. If +schema+ is nil, then XML Schema
 * validation is desactivated.
 *
 * Return 0 in case the schemas validation could be (de)activated and -1 in 
 * case of error.
 */
static VALUE
ruby_xml_reader_schema_validate(VALUE self, VALUE xsd)
{
  return INT2FIX(xmlTextReaderSchemaValidate(_SELF(self), NIL_P(xsd) ? NULL : RVAL2CSTR(xsd)));
}
#endif

/* 
 * call-seq:
 *    reader.name -> name
 *
 * Return the qualified name of the node.
 */
static VALUE
ruby_xml_reader_name(VALUE self)
{
  return CSTR2RVAL(xmlTextReaderConstName(_SELF(self)));
}

/* 
 * call-seq:
 *    reader.local_name -> name
 *
 * Return the local name of the node.
 */
static VALUE
ruby_xml_reader_local_name(VALUE self)
{
  return CSTR2RVAL(xmlTextReaderConstLocalName(_SELF(self)));
}

/*
 * call-seq:
 *    reader.attribute_count -> count
 *
 * Provide the number of attributes of the current node.
 */
static VALUE
ruby_xml_reader_attr_count(VALUE self)
{
  return INT2FIX(xmlTextReaderAttributeCount(_SELF(self)));
}

/*
 * call-seq:
 *    reader.encoding -> encoding
 *
 * Determine the encoding of the document being read.
 */
static VALUE
ruby_xml_reader_encoding(VALUE self)
{
  return CSTR2RVAL(xmlTextReaderConstEncoding(_SELF(self)));
}

/*
 * call-seq:
 *    reader.base_uri -> URI
 *
 * Determine the base URI of the node.
 */
static VALUE
ruby_xml_reader_base_uri(VALUE self)
{
  return CSTR2RVAL(xmlTextReaderConstBaseUri(_SELF(self)));
}

/*
 * call-seq:
 *    reader.namespace_uri -> URI
 *
 * Determine the namespace URI of the node.
 */
static VALUE
ruby_xml_reader_namespace_uri(VALUE self)
{
  return CSTR2RVAL(xmlTextReaderConstNamespaceUri(_SELF(self)));
}

/*
 * call-seq:
 *    reader.value -> text
 *
 * Provide the text value of the node if present.
 */
static VALUE
ruby_xml_reader_value(VALUE self)
{
  return CSTR2RVAL(xmlTextReaderConstValue(_SELF(self)));
}

/* 
 * call-seq:
 *    reader.prefix -> prefix
 *
 * Get a shorthand reference to the namespace associated with the node.
 */
static VALUE
ruby_xml_reader_prefix(VALUE self)
{
  return CSTR2RVAL(xmlTextReaderConstPrefix(_SELF(self)));
}

/*
 * call-seq:
 *    reader.depth -> depth
 *
 * Get the depth of the node in the tree.
 */
static VALUE
ruby_xml_reader_depth(VALUE self)
{
  return INT2FIX(xmlTextReaderDepth(_SELF(self)));
}

/*
 * call-seq:
 *    reader.quote_char -> char
 *
 * Get the quotation mark character used to enclose the value of an attribute,
 * as an integer value (and -1 in case of error).
 */
static VALUE
ruby_xml_reader_quote_char(VALUE self)
{
  return INT2FIX(xmlTextReaderQuoteChar(_SELF(self)));
}

/*
 * call-seq:
 *    reader.standalone -> code
 *
 * Determine the standalone status of the document being read.
 *
 * Return 1 if the document was declared to be standalone, 0 if it was 
 * declared to be not standalone, or -1 if the document did not specify its 
 * standalone status or in case of error.
 */
static VALUE
ruby_xml_reader_standalone(VALUE self)
{
  return INT2FIX(xmlTextReaderStandalone(_SELF(self)));
}

/*
 * call-seq:
 *    reader.xml_lang -> value
 *
 * Get the xml:lang scope within which the node resides.
 */
static VALUE
ruby_xml_reader_xml_lang(VALUE self)
{
  return CSTR2RVAL(xmlTextReaderConstXmlLang(_SELF(self)));
}

/*
 * call-seq:
 *    reader.xml_version -> version
 *
 * Determine the XML version of the document being read.
 */
static VALUE
ruby_xml_reader_xml_version(VALUE self)
{
  return CSTR2RVAL(xmlTextReaderConstXmlVersion(_SELF(self)));
}

/*
 * call-seq:
 *    reader.has_attributes? -> bool
 *
 * Get whether the node has attributes.
 */
static VALUE
ruby_xml_reader_has_attributes(VALUE self)
{
  return xmlTextReaderHasAttributes(_SELF(self)) ? Qtrue : Qfalse;
}

/*
 * call-seq:
 *    reader.has_value? -> bool
 *
 * Get whether the node can have a text value.
 */
static VALUE
ruby_xml_reader_has_value(VALUE self)
{
  return xmlTextReaderHasValue(_SELF(self)) ? Qtrue : Qfalse;
}

/*
 * call-seq:
 *    reader[key] -> value
 *
 * Provide the value of the attribute with the specified index (if +key+ is an 
 * integer) or with the specified name (if +key+ is a string) relative to the 
 * containing element, as a string.
 */
static VALUE
ruby_xml_reader_attribute(VALUE self, VALUE key)
{
  xmlTextReaderPtr reader;
  xmlChar *attr;

  reader = _SELF(self);

  if (TYPE(key) == T_FIXNUM) {
    attr = xmlTextReaderGetAttributeNo(reader, FIX2INT(key));
  }
  else {
    attr = xmlTextReaderGetAttribute(reader, (const xmlChar *)RVAL2CSTR(key));
  }
  return CSTR2RVAL2(attr);
}

/*
 * call-seq:
 *    reader.lookup_namespace(prefix) -> value
 *
 * Resolve a namespace prefix in the scope of the current element.
 * To return the default namespace, specify nil as +prefix+.
 */
static VALUE
ruby_xml_reader_lookup_namespace(VALUE self, VALUE prefix)
{
  return CSTR2RVAL2(xmlTextReaderLookupNamespace(_SELF(self), (const xmlChar *)RVAL2CSTR(prefix)));
}

/*
 * call-seq:
 *    reader.expand -> node
 *
 * Read the contents of the current node and the full subtree. It then makes 
 * the subtree available until the next read call.
 *
 * Return an XML::Node object, or nil in case of error.
 */
static VALUE
ruby_xml_reader_expand(VALUE self)
{
  xmlNodePtr node;

  node = xmlTextReaderExpand(_SELF(self));
  if (NIL_P(node))
    return Qnil;

  return ruby_xml_node2_wrap(cXMLNode, node);
}

#if LIBXML_VERSION >= 20618
/*
 * call-seq:
 *    reader.byte_consumed -> value
 *
 * This method provides the current index of the parser used by the reader, 
 * relative to the start of the current entity. 
 */
static VALUE
ruby_xml_reader_byte_consumed(VALUE self)
{
  return INT2NUM(xmlTextReaderByteConsumed(_SELF(self)));
}
#endif

#if LIBXML_VERSION >= 20617
/*
 * call-seq:
 *    reader.column_number -> number
 *
 * Provide the column number of the current parsing point.
 */
static VALUE
ruby_xml_reader_column_number(VALUE self)
{
  return INT2NUM(xmlTextReaderGetParserColumnNumber(_SELF(self)));
}

/*
 * call-seq:
 *    reader.line_number -> number
 *
 * Provide the line number of the current parsing point.
 */
static VALUE
ruby_xml_reader_line_number(VALUE self)
{
  return INT2NUM(xmlTextReaderGetParserLineNumber(_SELF(self)));
}
#endif

/*
 * call-seq:
 *    reader.default? -> bool
 *
 * Return whether an Attribute node was generated from the default value 
 * defined in the DTD or schema.
 */
static VALUE
ruby_xml_reader_default(VALUE self)
{
  return xmlTextReaderIsDefault(_SELF(self)) ? Qtrue : Qfalse;
}

/*
 * call-seq:
 *    reader.namespace_declaration? -> bool
 *
 * Determine whether the current node is a namespace declaration rather than a 
 * regular attribute.
 */
static VALUE
ruby_xml_reader_namespace_declaration(VALUE self)
{
  return xmlTextReaderIsNamespaceDecl(_SELF(self)) ? Qtrue : Qfalse;
}

/*
 * call-seq:
 *    reader.empty_element? -> bool
 *
 * Check if the current node is empty.
 */
static VALUE
ruby_xml_reader_empty_element(VALUE self)
{
  return xmlTextReaderIsEmptyElement(_SELF(self)) ? Qtrue : Qfalse;
}

/*
 * call-seq:
 *    reader.valid? -> bool
 *
 * Retrieve the validity status from the parser context.
 */
static VALUE
ruby_xml_reader_valid(VALUE self)
{
  return xmlTextReaderIsValid(_SELF(self)) ? Qtrue : Qfalse;
}

/* Rdoc needs to know. */ 
#ifdef RDOC_NEVER_DEFINED
  mXML = rb_define_module("XML");
#endif

void
ruby_init_xml_reader(void)
{
  cXMLReader = rb_define_class_under(mXML, "Reader", rb_cObject);
  error_handler_block_ivar_id = rb_intern("@__error_handler_callback__"); 
 
  rb_define_singleton_method(cXMLReader, "file", ruby_xml_reader_new_file, -1);
  rb_define_singleton_method(cXMLReader, "walker", ruby_xml_reader_new_walker, 1);
  rb_define_alias(CLASS_OF(cXMLReader), "document", "walker");
  rb_define_singleton_method(cXMLReader, "new", ruby_xml_reader_new_data, -1);
  rb_define_alias(CLASS_OF(cXMLReader), "string", "new");

  rb_define_method(cXMLReader, "close", ruby_xml_reader_close, 0);

  rb_define_method(cXMLReader, "move_to_attribute", ruby_xml_reader_move_to_attr, 1);
  rb_define_method(cXMLReader, "move_to_first_attribute", ruby_xml_reader_move_to_first_attr, 0);
  rb_define_method(cXMLReader, "move_to_next_attribute", ruby_xml_reader_move_to_next_attr, 0);
  rb_define_method(cXMLReader, "move_to_element", ruby_xml_reader_move_to_element, 0);
  rb_define_method(cXMLReader, "next", ruby_xml_reader_next, 0);
  rb_define_method(cXMLReader, "next_sibling", ruby_xml_reader_next_sibling, 0);
  rb_define_method(cXMLReader, "read", ruby_xml_reader_read, 0);
  rb_define_method(cXMLReader, "read_attribute_value", ruby_xml_reader_read_attr_value, 0);
  rb_define_method(cXMLReader, "read_inner_xml", ruby_xml_reader_read_inner_xml, 0);
  rb_define_method(cXMLReader, "read_outer_xml", ruby_xml_reader_read_outer_xml, 0);
  rb_define_method(cXMLReader, "read_state", ruby_xml_reader_read_state, 0);
  rb_define_method(cXMLReader, "read_string", ruby_xml_reader_read_string, 0);

  rb_define_method(cXMLReader, "set_error_handler", ruby_xml_reader_set_error_handler, 0);
  rb_define_method(cXMLReader, "reset_error_handler", ruby_xml_reader_reset_error_handler, 0);

  rb_define_method(cXMLReader, "relax_ng_validate", ruby_xml_reader_relax_ng_validate, 1);
#if LIBXML_VERSION >= 20620
  rb_define_method(cXMLReader, "schema_validate", ruby_xml_reader_schema_validate, 1);
#endif
  
  rb_define_method(cXMLReader, "node_type", ruby_xml_reader_node_type, 0);
  rb_define_method(cXMLReader, "normalization", ruby_xml_reader_normalization, 0);
  rb_define_method(cXMLReader, "attribute_count", ruby_xml_reader_attr_count, 0);
  rb_define_method(cXMLReader, "name", ruby_xml_reader_name, 0);
  rb_define_method(cXMLReader, "local_name", ruby_xml_reader_local_name, 0);
  rb_define_method(cXMLReader, "encoding", ruby_xml_reader_encoding, 0);
  rb_define_method(cXMLReader, "base_uri", ruby_xml_reader_base_uri, 0);
  rb_define_method(cXMLReader, "namespace_uri", ruby_xml_reader_namespace_uri, 0);
  rb_define_method(cXMLReader, "xml_lang", ruby_xml_reader_xml_lang, 0);
  rb_define_method(cXMLReader, "xml_version", ruby_xml_reader_xml_version, 0);
  rb_define_method(cXMLReader, "prefix", ruby_xml_reader_prefix, 0);
  rb_define_method(cXMLReader, "depth", ruby_xml_reader_depth, 0);
  rb_define_method(cXMLReader, "quote_char", ruby_xml_reader_quote_char, 0); 
  rb_define_method(cXMLReader, "standalone", ruby_xml_reader_standalone, 0);
  
  rb_define_method(cXMLReader, "has_attributes?", ruby_xml_reader_has_attributes, 0);
  rb_define_method(cXMLReader, "[]", ruby_xml_reader_attribute, 1);
  rb_define_method(cXMLReader, "has_value?", ruby_xml_reader_has_value, 0);
  rb_define_method(cXMLReader, "value", ruby_xml_reader_value, 0);

  rb_define_method(cXMLReader, "lookup_namespace", ruby_xml_reader_lookup_namespace, 1);
  rb_define_method(cXMLReader, "expand", ruby_xml_reader_expand, 0);

#if LIBXML_VERSION >= 20618
  rb_define_method(cXMLReader, "byte_consumed", ruby_xml_reader_byte_consumed, 0);
#endif
#if LIBXML_VERSION >= 20617
  rb_define_method(cXMLReader, "column_number", ruby_xml_reader_column_number, 0);
  rb_define_method(cXMLReader, "line_number", ruby_xml_reader_line_number, 0);
#endif
  rb_define_method(cXMLReader, "default?", ruby_xml_reader_default, 0);
  rb_define_method(cXMLReader, "empty_element?", ruby_xml_reader_empty_element, 0);
  rb_define_method(cXMLReader, "namespace_declaration?", ruby_xml_reader_namespace_declaration, 0);
  rb_define_method(cXMLReader, "valid?", ruby_xml_reader_valid, 0);

  rb_define_const(cXMLReader, "LOADDTD", INT2FIX(XML_PARSER_LOADDTD));
  rb_define_const(cXMLReader, "DEFAULTATTRS", INT2FIX(XML_PARSER_DEFAULTATTRS));
  rb_define_const(cXMLReader, "VALIDATE", INT2FIX(XML_PARSER_VALIDATE));
  rb_define_const(cXMLReader, "SUBST_ENTITIES", INT2FIX(XML_PARSER_SUBST_ENTITIES));

  rb_define_const(cXMLReader, "SEVERITY_VALIDITY_WARNING", INT2FIX(XML_PARSER_SEVERITY_VALIDITY_WARNING));
  rb_define_const(cXMLReader, "SEVERITY_VALIDITY_ERROR", INT2FIX(XML_PARSER_SEVERITY_VALIDITY_ERROR));
  rb_define_const(cXMLReader, "SEVERITY_WARNING", INT2FIX(XML_PARSER_SEVERITY_WARNING));
  rb_define_const(cXMLReader, "SEVERITY_ERROR", INT2FIX(XML_PARSER_SEVERITY_ERROR));

  rb_define_const(cXMLReader, "TYPE_NONE", INT2FIX(XML_READER_TYPE_NONE));
  rb_define_const(cXMLReader, "TYPE_ELEMENT", INT2FIX(XML_READER_TYPE_ELEMENT));
  rb_define_const(cXMLReader, "TYPE_ATTRIBUTE", INT2FIX(XML_READER_TYPE_ATTRIBUTE));
  rb_define_const(cXMLReader, "TYPE_TEXT", INT2FIX(XML_READER_TYPE_TEXT));
  rb_define_const(cXMLReader, "TYPE_CDATA", INT2FIX(XML_READER_TYPE_CDATA));
  rb_define_const(cXMLReader, "TYPE_ENTITY_REFERENCE", INT2FIX(XML_READER_TYPE_ENTITY_REFERENCE));
  rb_define_const(cXMLReader, "TYPE_ENTITY", INT2FIX(XML_READER_TYPE_ENTITY));
  rb_define_const(cXMLReader, "TYPE_PROCESSING_INSTRUCTION", INT2FIX(XML_READER_TYPE_PROCESSING_INSTRUCTION));
  rb_define_const(cXMLReader, "TYPE_COMMENT", INT2FIX(XML_READER_TYPE_COMMENT));
  rb_define_const(cXMLReader, "TYPE_DOCUMENT", INT2FIX(XML_READER_TYPE_DOCUMENT));
  rb_define_const(cXMLReader, "TYPE_DOCUMENT_TYPE", INT2FIX(XML_READER_TYPE_DOCUMENT_TYPE));
  rb_define_const(cXMLReader, "TYPE_DOCUMENT_FRAGMENT", INT2FIX(XML_READER_TYPE_DOCUMENT_FRAGMENT));
  rb_define_const(cXMLReader, "TYPE_NOTATION", INT2FIX(XML_READER_TYPE_NOTATION));
  rb_define_const(cXMLReader, "TYPE_WHITESPACE", INT2FIX(XML_READER_TYPE_WHITESPACE));
  rb_define_const(cXMLReader, "TYPE_SIGNIFICANT_WHITESPACE", INT2FIX(XML_READER_TYPE_SIGNIFICANT_WHITESPACE));
  rb_define_const(cXMLReader, "TYPE_END_ELEMENT", INT2FIX(XML_READER_TYPE_END_ELEMENT));
  rb_define_const(cXMLReader, "TYPE_END_ENTITY", INT2FIX(XML_READER_TYPE_END_ENTITY));
  rb_define_const(cXMLReader, "TYPE_XML_DECLARATION", INT2FIX(XML_READER_TYPE_XML_DECLARATION));

  rb_define_const(cXMLReader, "MODE_INITIAL", INT2FIX(XML_TEXTREADER_MODE_INITIAL));
  rb_define_const(cXMLReader, "MODE_INTERACTIVE", INT2FIX(XML_TEXTREADER_MODE_INTERACTIVE));
  rb_define_const(cXMLReader, "MODE_ERROR", INT2FIX(XML_TEXTREADER_MODE_ERROR));
  rb_define_const(cXMLReader, "MODE_EOF", INT2FIX(XML_TEXTREADER_MODE_EOF));
  rb_define_const(cXMLReader, "MODE_CLOSED", INT2FIX(XML_TEXTREADER_MODE_CLOSED));
  rb_define_const(cXMLReader, "MODE_READING", INT2FIX(XML_TEXTREADER_MODE_READING));
}
