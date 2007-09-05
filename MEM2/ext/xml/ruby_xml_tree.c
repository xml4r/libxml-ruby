/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

#include "libxml.h"
#include "ruby_xml_tree.h"

VALUE cXMLTree;

// Rdoc needs to know 
#ifdef RDOC_NEVER_DEFINED
  mXML = rb_define_module("XML");
#endif

void
ruby_init_xml_tree(void) {
  cXMLTree = rb_define_class_under(mXML, "Tree", rb_cObject);

  rb_define_const(cXMLTree, "ELEMENT_NODE", INT2FIX(XML_ELEMENT_NODE));
  rb_define_const(cXMLTree, "ATTRIBUTE_NODE", INT2FIX(XML_ATTRIBUTE_NODE));
  rb_define_const(cXMLTree, "TEXT_NODE", INT2FIX(XML_TEXT_NODE));
  rb_define_const(cXMLTree, "CDATA_SECTION_NODE", INT2FIX(XML_CDATA_SECTION_NODE));
  rb_define_const(cXMLTree, "ENTITY_REF_NODE", INT2FIX(XML_ENTITY_REF_NODE));
  rb_define_const(cXMLTree, "ENTITY_NODE", INT2FIX(XML_ENTITY_NODE));
  rb_define_const(cXMLTree, "PI_NODE", INT2FIX(XML_PI_NODE));
  rb_define_const(cXMLTree, "COMMENT_NODE", INT2FIX(XML_COMMENT_NODE));
  rb_define_const(cXMLTree, "DOCUMENT_NODE", INT2FIX(XML_DOCUMENT_NODE));
  rb_define_const(cXMLTree, "DOCUMENT_TYPE_NODE", INT2FIX(XML_DOCUMENT_TYPE_NODE));
  rb_define_const(cXMLTree, "DOCUMENT_FRAG_NODE", INT2FIX(XML_DOCUMENT_FRAG_NODE));
  rb_define_const(cXMLTree, "NOTATION_NODE", INT2FIX(XML_NOTATION_NODE));
  rb_define_const(cXMLTree, "HTML_DOCUMENT_NODE", INT2FIX(XML_HTML_DOCUMENT_NODE));
  rb_define_const(cXMLTree, "DTD_NODE", INT2FIX(XML_DTD_NODE));
  rb_define_const(cXMLTree, "ELEMENT_DECL", INT2FIX(XML_ELEMENT_DECL));
  rb_define_const(cXMLTree, "ATTRIBUTE_DECL", INT2FIX(XML_ATTRIBUTE_DECL));
  rb_define_const(cXMLTree, "ENTITY_DECL", INT2FIX(XML_ENTITY_DECL));
  rb_define_const(cXMLTree, "NAMESPACE_DECL", INT2FIX(XML_NAMESPACE_DECL));
  rb_define_const(cXMLTree, "XINCLUDE_START", INT2FIX(XML_XINCLUDE_START));
  rb_define_const(cXMLTree, "XINCLUDE_END", INT2FIX(XML_XINCLUDE_END));

#ifdef LIBXML_DOCB_ENABLED
  rb_define_const(cXMLTree, "DOCB_DOCUMENT_NODE", INT2FIX(XML_DOCB_DOCUMENT_NODE));
#endif
}
