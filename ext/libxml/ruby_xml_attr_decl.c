/* $Id: ruby_xml_attr.c 666 2008-12-07 00:16:50Z cfis $ */

/* Please see the LICENSE file for copyright and distribution information */

/*
 * Document-class: LibXML::XML::AttrDecl
 *
 */

#include "ruby_libxml.h"

VALUE cXMLAttrDecl;

void rxml_attr_decl_free(xmlAttributePtr xattribute)
{
  if (!xattribute)
    return;

  xattribute->_private = NULL;
  if (xattribute->parent == NULL && xattribute->doc == NULL)
  {
    rb_raise(rb_eRuntimeError, "%s: NYI", __FUNCTION__);
  }
}

void rxml_attr_decl_mark(xmlAttributePtr xattribute)
{
  if (xattribute == NULL)
    return;

  if (xattribute->_private == NULL)
  {
    rb_warning("AttrDecl is not bound! (%s:%d)", __FILE__, __LINE__);
    return;
  }

  rxml_node_mark_common((xmlNodePtr) xattribute);
}

VALUE rxml_attr_decl_wrap(xmlAttributePtr xattribute)
{
  VALUE result;
  // This node is already wrapped
  if (xattribute->_private != NULL)
    return (VALUE) xattribute->_private;

  result = Data_Wrap_Struct(cXMLAttrDecl, rxml_attr_decl_mark, rxml_attr_decl_free, xattribute);

  xattribute->_private = (void*) result;

  return result;
}

static VALUE rxml_attr_decl_alloc(VALUE klass)
{
  return Data_Wrap_Struct(klass, rxml_attr_decl_mark, rxml_attr_decl_free, NULL);
}

/*
 * call-seq:
 *    attr_decl.initialize(node, "name", "value")
 *
 * Creates a new attribute for the node.
 *
 * node: The XML::Node that will contain the attribute
 * name: The name of the attribute
 * value: The value of the attribute
 *
 *  attr = XML::Attr.new(doc.root, 'name', 'libxml')
 */
static VALUE rxml_attr_decl_initialize(int argc, VALUE *argv, VALUE self)
{
  rb_raise(rb_eRuntimeError, "%s: NYI", __FUNCTION__);
  return Qnil;
}

/*
 * call-seq:
 *    attr_decl.child -> node
 *
 * Obtain this attribute's child attribute(s).
 */
static VALUE rxml_attr_decl_child_get(VALUE self)
{
  xmlAttributePtr xattribute;
  Data_Get_Struct(self, xmlAttribute, xattribute);
  if (xattribute->children == NULL)
    return Qnil;
  else
    return rxml_node_wrap((xmlNodePtr) xattribute->children);
}

/*
 * call-seq:
 *    attr_decl.child? -> (true|false)
 *
 * Returns whether this attribute has child attributes.
 */
static VALUE rxml_attr_decl_child_q(VALUE self)
{
  xmlAttributePtr xattribute;
  Data_Get_Struct(self, xmlAttribute, xattribute);
  if (xattribute->children == NULL)
    return Qfalse;
  else
    return Qtrue;
}

/*
 * call-seq:
 *    attr_decl.doc -> XML::Document
 *
 * Returns this attribute's document.
 *
 *  doc.root.attributes.get_attribute('name').doc == doc
 */
static VALUE rxml_attr_decl_doc_get(VALUE self)
{
  xmlAttributePtr xattribute;
  Data_Get_Struct(self, xmlAttribute, xattribute);
  if (xattribute->doc == NULL)
    return Qnil;
  else
    return rxml_document_wrap(xattribute->doc);
}

/*
 * call-seq:
 *    attr_decl.doc? -> (true|false)
 *
 * Determine whether this attribute is associated with an
 * XML::Document.
 */
static VALUE rxml_attr_decl_doc_q(VALUE self)
{
  xmlAttributePtr xattribute;
  Data_Get_Struct(self, xmlAttribute, xattribute);
  if (xattribute->doc == NULL)
    return Qfalse;
  else
    return Qtrue;
}

/*
 * call-seq:
 *    attr_decl.last -> node
 *
 * Obtain the last attribute.
 */
static VALUE rxml_attr_decl_last_get(VALUE self)
{
  xmlAttributePtr xattribute;
  Data_Get_Struct(self, xmlAttribute, xattribute);
  if (xattribute->last == NULL)
    return Qnil;
  else
    return rxml_node_wrap(xattribute->last);
}

/*
 * call-seq:
 *    attr_decl.last? -> (true|false)
 *
 * Determine whether this is the last attribute.
 */
static VALUE rxml_attr_decl_last_q(VALUE self)
{
  xmlAttributePtr xattribute;
  Data_Get_Struct(self, xmlAttribute, xattribute);
  if (xattribute->last == NULL)
    return Qfalse;
  else
    return Qtrue;
}

/*
 * call-seq:
 *    attr_decl.name -> "name"
 *
 * Obtain this attribute's name.
 */
static VALUE rxml_attr_decl_name_get(VALUE self)
{
  xmlAttributePtr xattribute;
  Data_Get_Struct(self, xmlAttribute, xattribute);

  if (xattribute->name == NULL)
    return Qnil;
  else
    return rb_str_new2((const char*) xattribute->name);
}

/*
 * call-seq:
 *    attr_decl.next -> node
 *
 * Obtain the next attribute.
 */
static VALUE rxml_attr_decl_next_get(VALUE self)
{
  xmlAttributePtr xattribute;
  Data_Get_Struct(self, xmlAttribute, xattribute);
  if (xattribute->next == NULL)
    return Qnil;
  else
    return rxml_attr_decl_wrap((xmlAttributePtr)xattribute->next);
}

/*
 * call-seq:
 *    attr_decl.next? -> (true|false)
 *
 * Determine whether there is a next attribute.
 */
static VALUE rxml_attr_decl_next_q(VALUE self)
{
  xmlAttributePtr xattribute;
  Data_Get_Struct(self, xmlAttribute, xattribute);
  if (xattribute->next == NULL)
    return Qfalse;
  else
    return Qtrue;
}

/*
 * call-seq:
 *    attr_decl.type -> num
 *
 * Obtain this node's type identifier.
 */
static VALUE rxml_attr_decl_node_type(VALUE self)
{
  xmlAttrPtr xattr;
  Data_Get_Struct(self, xmlAttr, xattr);
  return INT2NUM(xattr->type);
}

/*
 * call-seq:
 *    attr_decl.ns -> namespace
 *
 * Obtain this attribute's associated XML::NS, if any.
 */
static VALUE rxml_attr_decl_ns_get(VALUE self)
{
  rb_raise(rb_eRuntimeError, "%s: NYI", __FUNCTION__);
  return Qnil;
}

/*
 * call-seq:
 *    attr_decl.ns? -> (true|false)
 *
 * Determine whether this attribute has an associated
 * namespace.
 */
static VALUE rxml_attr_decl_ns_q(VALUE self)
{
  rb_raise(rb_eRuntimeError, "%s: NYI", __FUNCTION__);
  return Qfalse;
}

/*
 * call-seq:
 *    attr_decl.parent -> node
 *
 * Obtain this attribute node's parent.
 */
static VALUE rxml_attr_decl_parent_get(VALUE self)
{
  xmlAttributePtr xattribute;
  Data_Get_Struct(self, xmlAttribute, xattribute);
  if (xattribute->parent == NULL)
    return Qnil;
  else
    return rxml_node_wrap((xmlNodePtr)xattribute->parent); /* Is this correct? */
}

/*
 * call-seq:
 *    attr_decl.parent? -> (true|false)
 *
 * Determine whether this attribute has a parent.
 */
static VALUE rxml_attr_decl_parent_q(VALUE self)
{
  xmlAttributePtr xattribute;
  Data_Get_Struct(self, xmlAttribute, xattribute);
  if (xattribute->parent == NULL)
    return Qfalse;
  else
    return Qtrue;
}

/*
 * call-seq:
 *    attr_decl.prev -> node
 *
 * Obtain the previous attribute.
 */
static VALUE rxml_attr_decl_prev_get(VALUE self)
{
  xmlAttributePtr xattribute;
  Data_Get_Struct(self, xmlAttribute, xattribute);
  if (xattribute->prev == NULL)
    return Qnil;
  else
    return rxml_attr_decl_wrap((xmlAttributePtr)xattribute->prev);
}

/*
 * call-seq:
 *    attr_decl.prev? -> (true|false)
 *
 * Determine whether there is a previous attribute.
 */
static VALUE rxml_attr_decl_prev_q(VALUE self)
{
  xmlAttributePtr xattribute;
  Data_Get_Struct(self, xmlAttribute, xattribute);
  if (xattribute->prev == NULL)
    return Qfalse;
  else
    return Qtrue;
}

/*
 * call-seq:
 *     node.remove! -> nil
 *
 * Removes this attribute from it's parent.
 */
static VALUE rxml_attr_decl_remove_ex(VALUE self)
{
  rb_raise(rb_eRuntimeError, "%s: NYI", __FUNCTION__);
  return Qnil;
}

/*
 * call-seq:
 *    attr_decl.value -> "value"
 *
 * Obtain the value of this attribute.
 */
VALUE rxml_attr_decl_value_get(VALUE self)
{
  xmlAttributePtr xattribute;
  xmlChar *value;
  VALUE result = Qnil;

  Data_Get_Struct(self, xmlAttribute, xattribute);

  if (xattribute->defaultValue)
    return rb_str_new2((const char *)xattribute->defaultValue);
  else
    return Qnil;
}

/*
 * call-seq:
 *    attr_decl.value = "value"
 *
 * Sets the value of this attribute.
 */
VALUE rxml_attr_decl_value_set(VALUE self, VALUE val)
{
  rb_raise(rb_eRuntimeError, "%s: NYI", __FUNCTION__);
  return self;
}

// Rdoc needs to know
#ifdef RDOC_NEVER_DEFINED
mLibXML = rb_define_module("LibXML");
mXML = rb_define_module_under(mLibXML, "XML");
#endif

void ruby_init_xml_attr_decl(void)
{
  cXMLAttrDecl = rb_define_class_under(mXML, "AttrDecl", rb_cObject);
  rb_define_alloc_func(cXMLAttrDecl, rxml_attr_decl_alloc);
  rb_define_method(cXMLAttrDecl, "initialize", rxml_attr_decl_initialize, -1);
  rb_define_method(cXMLAttrDecl, "child", rxml_attr_decl_child_get, 0);
  rb_define_method(cXMLAttrDecl, "child?", rxml_attr_decl_child_q, 0);
  rb_define_method(cXMLAttrDecl, "doc", rxml_attr_decl_doc_get, 0);
  rb_define_method(cXMLAttrDecl, "doc?", rxml_attr_decl_doc_q, 0);
  rb_define_method(cXMLAttrDecl, "last", rxml_attr_decl_last_get, 0);
  rb_define_method(cXMLAttrDecl, "last?", rxml_attr_decl_last_q, 0);
  rb_define_method(cXMLAttrDecl, "name", rxml_attr_decl_name_get, 0);
  rb_define_method(cXMLAttrDecl, "next", rxml_attr_decl_next_get, 0);
  rb_define_method(cXMLAttrDecl, "next?", rxml_attr_decl_next_q, 0);
  rb_define_method(cXMLAttrDecl, "node_type", rxml_attr_decl_node_type, 0);
  rb_define_method(cXMLAttrDecl, "ns", rxml_attr_decl_ns_get, 0);
  rb_define_method(cXMLAttrDecl, "ns?", rxml_attr_decl_ns_q, 0);
  rb_define_method(cXMLAttrDecl, "parent", rxml_attr_decl_parent_get, 0);
  rb_define_method(cXMLAttrDecl, "parent?", rxml_attr_decl_parent_q, 0);
  rb_define_method(cXMLAttrDecl, "prev", rxml_attr_decl_prev_get, 0);
  rb_define_method(cXMLAttrDecl, "prev?", rxml_attr_decl_prev_q, 0);
  rb_define_method(cXMLAttrDecl, "remove!", rxml_attr_decl_remove_ex, 0);
  rb_define_method(cXMLAttrDecl, "value", rxml_attr_decl_value_get, 0);
  rb_define_method(cXMLAttrDecl, "value=", rxml_attr_decl_value_set, 1);
}