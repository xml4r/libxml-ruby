/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

#include "libxml.h"
#include "ruby_xml_attribute.h"

VALUE cXMLAttribute;

// TODO Wtf is this about? It's not referenced outside this file AFAIK...

VALUE
ruby_xml_attribute_child_get(VALUE self) {
  ruby_xml_attribute *rxa;
  Data_Get_Struct(self, ruby_xml_attribute, rxa);
  if (rxa->attribute->children == NULL)
    return(Qnil);
  else
    return(ruby_xml_node_new_ptr(cXMLNode, rxa->xd, rxa->attribute->children));
}


VALUE
ruby_xml_attribute_children_q(VALUE self) {
  ruby_xml_attribute *rxa;
  Data_Get_Struct(self, ruby_xml_attribute, rxa);
  if (rxa->attribute->children == NULL)
    return(Qfalse);
  else
    return(Qtrue);
}


VALUE
ruby_xml_attribute_default_get(VALUE self) {
  ruby_xml_attribute *rxa;
  Data_Get_Struct(self, ruby_xml_attribute, rxa);

  if (rxa->attribute->defaultValue == NULL)
    return(Qnil);
  else
    return(rb_str_new2((const char*)rxa->attribute->defaultValue));
}


VALUE
ruby_xml_attribute_element_get(VALUE self) {
  ruby_xml_attribute *rxa;
  Data_Get_Struct(self, ruby_xml_attribute, rxa);

  if (rxa->attribute->elem == NULL)
    return(Qnil);
  else
    return(rb_str_new2((const char*)rxa->attribute->elem));
}


void
ruby_xml_attribute_free(ruby_xml_attribute *rxa) {
 if (rxa->attribute != NULL && !rxa->is_ptr) {
    xmlUnlinkNode((xmlNodePtr)rxa->attribute);
    xmlFreeNode((xmlNodePtr)rxa->attribute);
    rxa->attribute = NULL;
  }

  free(rxa);
}


VALUE
ruby_xml_attribute_last_get(VALUE self) {
  ruby_xml_attribute *rxa;
  Data_Get_Struct(self, ruby_xml_attribute, rxa);
  if (rxa->attribute->last == NULL)
    return(Qnil);
  else
    return(ruby_xml_node_new_ptr(cXMLNode, rxa->xd, rxa->attribute->last));
}


VALUE
ruby_xml_attribute_last_q(VALUE self) {
  ruby_xml_attribute *rxa;
  Data_Get_Struct(self, ruby_xml_attribute, rxa);
  if (rxa->attribute->last == NULL)
    return(Qfalse);
  else
    return(Qtrue);
}


static void
ruby_xml_attribute_mark(ruby_xml_attribute *rxa) {
  if (rxa == NULL) return;
  if (!NIL_P(rxa->xd)) rb_gc_mark(rxa->xd);
}


VALUE
ruby_xml_attribute_name_get(VALUE self) {
  ruby_xml_attribute *rxa;
  Data_Get_Struct(self, ruby_xml_attribute, rxa);

  if (rxa->attribute->name == NULL)
    return(Qnil);
  else
    return(rb_str_new2((const char*)rxa->attribute->name));
}


VALUE
ruby_xml_attribute_new(VALUE class, VALUE xd, xmlAttributePtr attribute) {
  ruby_xml_attribute *rxa;

  rxa = ALLOC(ruby_xml_attribute);
  rxa->is_ptr = 0;
  rxa->attribute = attribute;
  if (NIL_P(xd))
    rxa->xd = Qnil;
  else
    rxa->xd = xd;
  return(Data_Wrap_Struct(class, ruby_xml_attribute_mark,
			  ruby_xml_attribute_free, rxa));
}


VALUE
ruby_xml_attribute_new2(VALUE class, VALUE xd, xmlAttributePtr attribute) {
  ruby_xml_attribute *rxa;

  rxa = ALLOC(ruby_xml_attribute);
  rxa->is_ptr = 1;
  rxa->attribute = attribute;
  if (NIL_P(xd))
    rxa->xd = Qnil;
  else
    rxa->xd = xd;
  return(Data_Wrap_Struct(class, ruby_xml_attribute_mark,
			  ruby_xml_attribute_free, rxa));
}


VALUE
ruby_xml_attribute_next_get(VALUE self) {
  ruby_xml_attribute *rxa;
  Data_Get_Struct(self, ruby_xml_attribute, rxa);
  if (rxa->attribute->next == NULL)
    return(Qnil);
  else
    return(ruby_xml_node_new_ptr(cXMLNode, rxa->xd, rxa->attribute->next));
}


VALUE
ruby_xml_attribute_next_q(VALUE self) {
  ruby_xml_attribute *rxa;
  Data_Get_Struct(self, ruby_xml_attribute, rxa);
  if (rxa->attribute->next == NULL)
    return(Qfalse);
  else
    return(Qtrue);
}


VALUE
ruby_xml_attribute_node_type_name(VALUE self) {
  return(rb_str_new2("attribute"));
}


VALUE
ruby_xml_attribute_prefix_get(VALUE self) {
  ruby_xml_attribute *rxa;
  Data_Get_Struct(self, ruby_xml_attribute, rxa);

  if (rxa->attribute->prefix == NULL)
    return(Qnil);
  else
    return(rb_str_new2((const char*)rxa->attribute->prefix));
}


VALUE
ruby_xml_attribute_prev_get(VALUE self) {
  ruby_xml_attribute *rxa;
  Data_Get_Struct(self, ruby_xml_attribute, rxa);
  if (rxa->attribute->prev == NULL)
    return(Qnil);
  else
    return(ruby_xml_node_new_ptr(cXMLNode, rxa->xd, rxa->attribute->prev));
}

VALUE
ruby_xml_attribute_prev_q(VALUE self) {
  ruby_xml_attribute *rxa;
  Data_Get_Struct(self, ruby_xml_attribute, rxa);
  if (rxa->attribute->prev == NULL)
    return(Qfalse);
  else
    return(Qtrue);
}

// Rdoc maybe doesn't need to know 
// #ifdef RDOC_NEVER_DEFINED
//   mXML = rb_define_module("XML");
// #endif

void
ruby_init_xml_attribute(void) {
  cXMLAttribute = rb_define_class_under(mXML, "Attribute", rb_cObject);
  rb_define_method(cXMLAttribute, "child", ruby_xml_attribute_child_get, 0);
  rb_define_method(cXMLAttribute, "children?", ruby_xml_attribute_children_q, 0);
  rb_define_method(cXMLAttribute, "default", ruby_xml_attribute_default_get, 0);
  rb_define_method(cXMLAttribute, "element", ruby_xml_attribute_element_get, 0);
  rb_define_method(cXMLAttribute, "last", ruby_xml_attribute_last_get, 0);
  rb_define_method(cXMLAttribute, "last?", ruby_xml_attribute_last_q, 0);
  rb_define_method(cXMLAttribute, "node_type_name", ruby_xml_attribute_node_type_name, 0);
  rb_define_method(cXMLAttribute, "name", ruby_xml_attribute_name_get, 0);
  rb_define_method(cXMLAttribute, "next", ruby_xml_attribute_next_get, 0);
  rb_define_method(cXMLAttribute, "next?", ruby_xml_attribute_next_q, 0);
  rb_define_method(cXMLAttribute, "prefix", ruby_xml_attribute_prefix_get, 0);
  rb_define_method(cXMLAttribute, "prev", ruby_xml_attribute_prev_get, 0);
  rb_define_method(cXMLAttribute, "prev?", ruby_xml_attribute_prev_q, 0);
}
