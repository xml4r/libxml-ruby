/* $Id: ruby_xml_attributes.c 300 2008-07-01 19:14:15Z cfis $ */

/* Please see the LICENSE file for copyright and distribution information */

#include "ruby_libxml.h"
#include "ruby_xml_attributes.h"

VALUE cXMLAttributes;

void
ruby_xml_attributes_mark(xmlNodePtr xnode) {
  ruby_xml_node_mark_common(xnode);
}

/*
 * Create new attributes instance.  Not exposed to ruby.
 */
VALUE
ruby_xml_attributes_new(xmlNodePtr xnode)
{
  return Data_Wrap_Struct(cXMLAttributes,
                          ruby_xml_attributes_mark, NULL,
                          xnode);
}

/*
 * call-seq:
 *    attributes.xnode => xnode
 * 
 * Obtain this attributes parent element.
 */
VALUE
ruby_xml_attributes_node_get(VALUE self) {
  xmlNodePtr *xnode;
  Data_Get_Struct(self, xmlNodePtr, xnode);
  return(ruby_xml_node2_wrap(cXMLNode, xnode));
}


/*
 * call-seq:
 *    attributes.get_attribute("name") => xattr
 * 
 * Returns the named attribute.
 */
VALUE
ruby_xml_attributes_get_attribute(VALUE self, VALUE name) {
  xmlNodePtr xnode;
  xmlAttrPtr xattr;

  name = check_string_or_symbol(name);
  
  Data_Get_Struct(self, xmlNodePtr, xnode);
  
  xattr = xmlHasProp(xnode, (xmlChar*)StringValuePtr(name));
  
  if (xattr)
    return(ruby_xml_attr_wrap(xattr));
  else
    return(Qnil);
}

/*
 * call-seq:
 *    attributes.get_attribute_ns("namespace", "name") => xattr
 * 
 * Returns the named attribute.
 */
VALUE
ruby_xml_attributes_get_attribute_ns(VALUE self, VALUE namespace, VALUE name) {
  xmlNodePtr xnode;
  xmlAttrPtr xattr;

  name = check_string_or_symbol(name);
  
  Data_Get_Struct(self, xmlNodePtr, xnode);
  
  xattr = xmlHasNsProp(xnode, (xmlChar*)StringValuePtr(name), (xmlChar*)StringValuePtr(namespace));
  
  if (xattr)
    return(ruby_xml_attr_wrap(xattr));
  else
    return(Qnil);
}

/*
 * call-seq:
 *    attributes["name"]          => "string"
 * 
 * Obtain the value of the named attribute.
 */
VALUE
ruby_xml_attributes_attribute_get(VALUE self, VALUE name) {
  VALUE xattr = ruby_xml_attributes_get_attribute(self, name);
  if NIL_P(xattr)
    return(Qnil);
  else
    return ruby_xml_attr_value_get(xattr);
}  

/*
 * call-seq:
 *    attributes["name"] = "string"
 * 
 * Set the value of the named attribute.
 */
VALUE
ruby_xml_attributes_attribute_set(VALUE self, VALUE name, VALUE value) {
  VALUE xattr = ruby_xml_attributes_get_attribute(self, name);
  if NIL_P(xattr)
  {
    xmlNodePtr xnode;
    xmlAttrPtr xattr;
    VALUE args[3];

    args[0] = ruby_xml_attributes_node_get(self);
    args[1] = name;
    args[2] = value;

    return rb_class_new_instance(sizeof(args)/sizeof(VALUE), args, cXMLAttr);
  }
  else
  {
    return ruby_xml_attr_value_set(xattr, value);
  }
}  


/*
 * call-seq:
 *    attributes.each => iterator
 * 
 * Iterate over each attribute.
 */
VALUE
ruby_xml_attributes_each(VALUE self) {
  xmlNodePtr xnode;
  xmlAttrPtr xattr;
  Data_Get_Struct(self, xmlNodePtr, xnode);

  xattr = xnode->properties;

  while (xattr)
  {
     VALUE attr = ruby_xml_attr_wrap(xattr);
     rb_yield(attr);
     xattr = xattr->next;
  }
  
  return self;
}  
  
/*
 * call-seq:
 *    attributes.length => iterator
 * 
 * Returns the number of attributes.
 */
VALUE
ruby_xml_attributes_length(VALUE self) {
  int length = 0;
  xmlNodePtr xnode;
  xmlAttrPtr xattr;
  Data_Get_Struct(self, xmlNodePtr, xnode);

  xattr = xnode->properties;

  while (xattr)
  {
     length++;
     xattr = xattr->next;
  }
  
  return INT2NUM(length);
}  

/*
 * call-seq:
 *    attributes.first => XML::Attr
 * 
 * Returns the first attribute.
 */
VALUE
ruby_xml_attributes_first(VALUE self) {
  xmlNodePtr xnode;
  Data_Get_Struct(self, xmlNodePtr, xnode);

  if (xnode->type == XML_ELEMENT_NODE) {
    xmlAttrPtr xattr = xnode->properties;
    
    if (xattr) {
      return(ruby_xml_attr_wrap(xattr));
    }
  } 
  return(Qnil);
}

    
void
ruby_init_xml_attributes(void) {
  cXMLAttributes = rb_define_class_under(mXML, "Attributes", rb_cObject);
  rb_include_module(cXMLAttributes, rb_const_get(rb_cObject, rb_intern("Enumerable")));
  rb_define_method(cXMLAttributes, "xnode", ruby_xml_attributes_node_get, 0);
  rb_define_method(cXMLAttributes, "get_attribute", ruby_xml_attributes_get_attribute, 1);
  rb_define_method(cXMLAttributes, "get_attribute_ns", ruby_xml_attributes_get_attribute_ns, 2);
  rb_define_method(cXMLAttributes, "[]", ruby_xml_attributes_attribute_get, 1);
  rb_define_method(cXMLAttributes, "[]=", ruby_xml_attributes_attribute_set, 2);
  rb_define_method(cXMLAttributes, "each", ruby_xml_attributes_each, 0);
  rb_define_method(cXMLAttributes, "length", ruby_xml_attributes_length, 0);
  rb_define_method(cXMLAttributes, "first", ruby_xml_attributes_first, 0);
}
