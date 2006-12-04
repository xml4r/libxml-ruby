/* Copyright (c) 2006 Apple Computer Inc.
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

VALUE
ruby_xml_reader_new_walker(VALUE self, VALUE doc)
{
  ruby_xml_document *rxd;
  xmlTextReaderPtr reader;
  
  Data_Get_Struct(doc, ruby_xml_document, rxd);
 
  reader = xmlReaderWalker(rxd->doc);  
  if (reader == NULL)
    rb_raise(rb_eRuntimeError, "cannot create text reader for given document");

  return ruby_xml_reader_new(self, reader); 
}

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

static VALUE
ruby_xml_reader_close(VALUE self)
{
  return INT2FIX(xmlTextReaderClose(_SELF(self)));
}

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

static VALUE
ruby_xml_reader_move_to_first_attr(VALUE self)
{
  return INT2FIX(xmlTextReaderMoveToFirstAttribute(_SELF(self)));
}

static VALUE
ruby_xml_reader_move_to_next_attr(VALUE self)
{
  return INT2FIX(xmlTextReaderMoveToNextAttribute(_SELF(self)));
}

static VALUE
ruby_xml_reader_move_to_element(VALUE self)
{
  return INT2FIX(xmlTextReaderMoveToElement(_SELF(self)));
}

static VALUE
ruby_xml_reader_next(VALUE self)
{
  return INT2FIX(xmlTextReaderNext(_SELF(self)));
}

static VALUE
ruby_xml_reader_next_sibling(VALUE self)
{
  return INT2FIX(xmlTextReaderNextSibling(_SELF(self)));
}

static VALUE
ruby_xml_reader_node_type(VALUE self)
{
  return INT2FIX(xmlTextReaderNodeType(_SELF(self)));
}

static VALUE
ruby_xml_reader_normalization(VALUE self)
{
  return INT2FIX(xmlTextReaderNormalization(_SELF(self)));
}

static VALUE
ruby_xml_reader_read(VALUE self)
{
  return INT2FIX(xmlTextReaderRead(_SELF(self)));
}

static VALUE
ruby_xml_reader_read_attr_value(VALUE self)
{
  return INT2FIX(xmlTextReaderReadAttributeValue(_SELF(self)));
}

static VALUE
ruby_xml_reader_read_inner_xml(VALUE self)
{
  return CSTR2RVAL2(xmlTextReaderReadInnerXml(_SELF(self)));
}

static VALUE
ruby_xml_reader_read_outer_xml(VALUE self)
{
  return CSTR2RVAL2(xmlTextReaderReadOuterXml(_SELF(self)));
}

static VALUE
ruby_xml_reader_read_state(VALUE self)
{
  return INT2FIX(xmlTextReaderReadState(_SELF(self)));
}

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

static VALUE
ruby_xml_reader_reset_error_handler(VALUE self)
{
  xmlTextReaderSetErrorHandler(_SELF(self), NULL, NULL);
  return self;
}

static VALUE
ruby_xml_reader_relax_ng_validate(VALUE self, VALUE rng)
{
  return INT2FIX(xmlTextReaderRelaxNGValidate(_SELF(self), NIL_P(rng) ? NULL : RVAL2CSTR(rng)));
}

#if LIBXML_VERSION >= 20620
static VALUE
ruby_xml_reader_schema_validate(VALUE self, VALUE xsd)
{
  return INT2FIX(xmlTextReaderSchemaValidate(_SELF(self), NIL_P(xsd) ? NULL : RVAL2CSTR(xsd)));
}
#endif

static VALUE
ruby_xml_reader_name(VALUE self)
{
  return CSTR2RVAL(xmlTextReaderConstName(_SELF(self)));
}

static VALUE
ruby_xml_reader_local_name(VALUE self)
{
  return CSTR2RVAL(xmlTextReaderConstLocalName(_SELF(self)));
}

static VALUE
ruby_xml_reader_attr_count(VALUE self)
{
  return INT2FIX(xmlTextReaderAttributeCount(_SELF(self)));
}

static VALUE
ruby_xml_reader_encoding(VALUE self)
{
  return CSTR2RVAL(xmlTextReaderConstEncoding(_SELF(self)));
}

static VALUE
ruby_xml_reader_base_uri(VALUE self)
{
  return CSTR2RVAL(xmlTextReaderConstBaseUri(_SELF(self)));
}

static VALUE
ruby_xml_reader_namespace_uri(VALUE self)
{
  return CSTR2RVAL(xmlTextReaderConstNamespaceUri(_SELF(self)));
}

static VALUE
ruby_xml_reader_value(VALUE self)
{
  return CSTR2RVAL(xmlTextReaderConstValue(_SELF(self)));
}

static VALUE
ruby_xml_reader_prefix(VALUE self)
{
  return CSTR2RVAL(xmlTextReaderConstPrefix(_SELF(self)));
}

static VALUE
ruby_xml_reader_depth(VALUE self)
{
  return INT2FIX(xmlTextReaderDepth(_SELF(self)));
}

static VALUE
ruby_xml_reader_quote_char(VALUE self)
{
  return INT2FIX(xmlTextReaderQuoteChar(_SELF(self)));
}

static VALUE
ruby_xml_reader_standalone(VALUE self)
{
  return INT2FIX(xmlTextReaderStandalone(_SELF(self)));
}

static VALUE
ruby_xml_reader_xml_lang(VALUE self)
{
  return CSTR2RVAL(xmlTextReaderConstXmlLang(_SELF(self)));
}

static VALUE
ruby_xml_reader_xml_version(VALUE self)
{
  return CSTR2RVAL(xmlTextReaderConstXmlVersion(_SELF(self)));
}

static VALUE
ruby_xml_reader_has_attributes(VALUE self)
{
  return xmlTextReaderHasAttributes(_SELF(self)) ? Qtrue : Qfalse;
}

static VALUE
ruby_xml_reader_has_value(VALUE self)
{
  return xmlTextReaderHasValue(_SELF(self)) ? Qtrue : Qfalse;
}

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

static VALUE
ruby_xml_reader_lookup_namespace(VALUE self, VALUE prefix)
{
  return CSTR2RVAL2(xmlTextReaderLookupNamespace(_SELF(self), (const xmlChar *)RVAL2CSTR(prefix)));
}

static VALUE
ruby_xml_reader_expand(VALUE self)
{
  xmlNodePtr node;

  node = xmlTextReaderExpand(_SELF(self));
  if (NIL_P(node))
    return Qnil;

  return ruby_xml_node_new(cXMLNode, node);
}

#if LIBXML_VERSION >= 20618
static VALUE
ruby_xml_reader_byte_consumed(VALUE self)
{
  return INT2NUM(xmlTextReaderByteConsumed(_SELF(self)));
}
#endif

#if LIBXML_VERSION >= 20617
static VALUE
ruby_xml_reader_column_number(VALUE self)
{
  return INT2NUM(xmlTextReaderGetParserColumnNumber(_SELF(self)));
}

static VALUE
ruby_xml_reader_line_number(VALUE self)
{
  return INT2NUM(xmlTextReaderGetParserLineNumber(_SELF(self)));
}
#endif

static VALUE
ruby_xml_reader_default(VALUE self)
{
  return xmlTextReaderIsDefault(_SELF(self)) ? Qtrue : Qfalse;
}

static VALUE
ruby_xml_reader_namespace_declaration(VALUE self)
{
  return xmlTextReaderIsNamespaceDecl(_SELF(self)) ? Qtrue : Qfalse;
}

static VALUE
ruby_xml_reader_empty_element(VALUE self)
{
  return xmlTextReaderIsEmptyElement(_SELF(self)) ? Qtrue : Qfalse;
}

static VALUE
ruby_xml_reader_valid(VALUE self)
{
  return xmlTextReaderIsValid(_SELF(self)) ? Qtrue : Qfalse;
}

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
