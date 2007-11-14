/* $Id: $ */

#include "libxml.h"

/*
 * Document-class: XML::XPath::Object
 *
 * All XPath and XPointer evals result in this type.
 * nodeset should become entirely virtual and refer back to
 * this class/data structure.
 *
 */
VALUE cXMLXPathObject;

void
ruby_xml_xpath_object_mark(xmlXPathObjectPtr xpop)
{
  void * xnp;
  int i;

  if ( xpop->type == XPATH_NODESET && xpop->nodesetval != NULL ) {
    for (i=0; i<xpop->nodesetval->nodeNr; i++) {
      xnp=xpop->nodesetval->nodeTab[i]->_private;
      if (xnp != NULL)
	rb_gc_mark((VALUE)xnp);
    }
  }
}

void
ruby_xml_xpath_object_free(xmlXPathObjectPtr xpop)
{
  xmlXPathFreeObject(xpop);
}

VALUE
ruby_xml_xpath_object_wrap(xmlXPathObjectPtr xpop)
{
  if ( xpop==NULL )
    return Qnil;

  return Data_Wrap_Struct(cXMLXPathObject,
			  ruby_xml_xpath_object_mark,
			  ruby_xml_xpath_object_free,
			  xpop);
}

/*
 * call-seq:
 *    xpath_object.to_a => [node, ..., node]
 * 
 * Obtain an array of the nodes in this set.
 */
VALUE
ruby_xml_xpath_object_to_a(VALUE self)
{
  VALUE set_ary, nodeobj;
  xmlXPathObjectPtr xpop;
  int i;
  
  Data_Get_Struct(self,xmlXPathObject,xpop);

  set_ary = rb_ary_new();
  if (!((xpop->nodesetval == NULL) || (xpop->nodesetval->nodeNr == 0))) {
    for (i = 0; i < xpop->nodesetval->nodeNr; i++) {
      nodeobj = ruby_xml_node2_wrap(cXMLNode, xpop->nodesetval->nodeTab[i]);
      rb_ary_push(set_ary, nodeobj);
    }
  }
  
  return(set_ary);
}

/*
 * call-seq:
 *    xpath_object.set => Node::Set
 * 
 * Obtain the previous type object which is really
 * just a proxy back to this object. Unless the
 * type is not a NODESET, in which case it is nil.
 */
VALUE
ruby_xml_xpath_object_set(VALUE self)
{
  xmlXPathObjectPtr xpop;
  VALUE r;

  Data_Get_Struct(self,xmlXPathObject,xpop);
  r=Qnil;

  if (xpop->type == XPATH_NODESET)
    r=ruby_xml_node_set_new2(self);

  return r;
}

/*
 * call-seq:
 *    xpath_object.empty? => (true|false)
 *
 * Determine whether this nodeset is empty (contains no nodes).
 */
VALUE
ruby_xml_xpath_object_empty_q(VALUE self) {
  xmlXPathObjectPtr xpop;

  Data_Get_Struct(self,xmlXPathObject,xpop);

  if (xpop->type != XPATH_NODESET)
    return Qnil;
  
  return ( xpop->nodesetval == NULL || xpop->nodesetval->nodeNr <= 0 ) ? Qtrue : Qfalse;
}

/*
 * call-seq:
 *    xpath_object.each { |node| ... } => self
 * 
 * Call the supplied block for each node in this set.
 */
VALUE
ruby_xml_xpath_object_each(VALUE self)
{
  xmlXPathObjectPtr xpop;
  int i;
  VALUE nodeobj;

  if ( ruby_xml_xpath_object_empty_q(self) == Qtrue )
    return Qnil;

  Data_Get_Struct(self,xmlXPathObject,xpop);

  for (i = 0; i < xpop->nodesetval->nodeNr; i++) {
    switch(xpop->nodesetval->nodeTab[i]->type) {
    case XML_ATTRIBUTE_NODE:
      nodeobj = ruby_xml_attr_wrap(cXMLAttr,
				   (xmlAttrPtr)xpop->nodesetval->nodeTab[i]);
      break;
    default:
      nodeobj = ruby_xml_node2_wrap(cXMLNode, xpop->nodesetval->nodeTab[i]);
    }

    rb_yield(nodeobj);
  }
  return(self);
}

/*
 * call-seq:
 *    xpath_object.first => node
 *
 * Returns the first node in this node set, or nil if none exist.
 */
VALUE
ruby_xml_xpath_object_first(VALUE self) {
  VALUE nodeobj;

  xmlXPathObjectPtr xpop;

  if ( ruby_xml_xpath_object_empty_q(self) == Qtrue )
    return Qnil;

  Data_Get_Struct(self,xmlXPathObject,xpop);
  
  switch(xpop->nodesetval->nodeTab[0]->type) {
  case XML_ATTRIBUTE_NODE:
    nodeobj = ruby_xml_attr_wrap(cXMLAttr,
				 (xmlAttrPtr)xpop->nodesetval->nodeTab[0]);
    break;
  default:
    nodeobj = ruby_xml_node2_wrap(cXMLNode,
				  xpop->nodesetval->nodeTab[0]);
  }
  
  return(nodeobj);
}

/*
 * call-seq:
 *    xpath_object.length => num
 * 
 * Obtain the length of the nodesetval node list.
 */
VALUE
ruby_xml_xpath_object_length(VALUE self) {
  xmlXPathObjectPtr xpop;

  if ( ruby_xml_xpath_object_empty_q(self) == Qtrue )
    return INT2FIX(0);

  Data_Get_Struct(self,xmlXPathObject,xpop);

  return INT2NUM(xpop->nodesetval->nodeNr);
}

VALUE
ruby_xml_xpath_object_set_type(VALUE self)
{
  xmlXPathObjectPtr xpop;

  Data_Get_Struct(self,xmlXPathObject,xpop);

  return INT2FIX(xpop->type);
}

VALUE
ruby_xml_xpath_object_string(VALUE self)
{
  xmlXPathObjectPtr xpop;

  Data_Get_Struct(self,xmlXPathObject,xpop);

  if (xpop->stringval == NULL)
    return Qnil;

  return rb_str_new2((const char*) xpop->stringval);
}

// Rdoc needs to know
#ifdef RDOC_NEVER_DEFINED
  mXML = rb_define_module("XML");
  cXMLXPath = rb_define_class_under(mXML, "XPath", rb_cObject);
#endif

void
ruby_init_xml_xpath_object(void) {
  cXMLXPathObject = rb_define_class_under(cXMLXPath, "Object", rb_cObject);

  rb_include_module(cXMLXPathObject, rb_const_get(rb_cObject, rb_intern("Enumerable")));

  rb_define_method(cXMLXPathObject, "each", ruby_xml_xpath_object_each, 0);
  rb_define_method(cXMLXPathObject, "set_type", ruby_xml_xpath_object_set_type, 0);
  rb_define_method(cXMLXPathObject, "empty?", ruby_xml_xpath_object_empty_q, 0);
  rb_define_method(cXMLXPathObject, "first", ruby_xml_xpath_object_first, 0);
  rb_define_method(cXMLXPathObject, "length", ruby_xml_xpath_object_length, 0);
  rb_define_method(cXMLXPathObject, "size", ruby_xml_xpath_object_length, 0);
  rb_define_method(cXMLXPathObject, "to_a", ruby_xml_xpath_object_to_a, 0);

  rb_define_method(cXMLXPathObject, "string", ruby_xml_xpath_object_string, 0);

  rb_include_module(cXMLNodeSet, rb_const_get(rb_cObject, rb_intern("Enumerable")));

  /* Give the NodeSet type, but it is pointless */
  rb_define_method(cXMLXPathObject, "set", ruby_xml_xpath_object_set, 0);
  
}
