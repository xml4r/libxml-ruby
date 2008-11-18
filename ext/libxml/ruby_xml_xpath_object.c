/* $Id: $ */

#include "ruby_libxml.h"

/*
 * Document-class: LibXML::XML::XPath::Object
 *
 * A collection of nodes returned from the evaluation of an XML::XPath
 * or XML::XPointer expression. 
 *
 */
VALUE cXMLXPathObject;


xmlDocPtr
ruby_xml_xpath_object_doc(xmlXPathObjectPtr xpop)
{
  xmlDocPtr result = NULL;
  xmlNodePtr *nodes = NULL;

  if (xpop->type != XPATH_NODESET)
    return result;

  if (!xpop->nodesetval || !xpop->nodesetval->nodeTab)
    return result;

  nodes = xpop->nodesetval->nodeTab;

  if (!(*nodes))
    return result;

  return (*nodes)->doc;
}

void
ruby_xml_xpath_object_mark(xmlXPathObjectPtr xpop)
{
  int i;

  if ( xpop->type == XPATH_NODESET && xpop->nodesetval != NULL ) 
  {
    xmlDocPtr xdoc = ruby_xml_xpath_object_doc(xpop);
    if (xdoc && xdoc->_private)
      rb_gc_mark(xdoc->_private);

    for (i=0; i<xpop->nodesetval->nodeNr; i++) 
    {
      if (xpop->nodesetval->nodeTab[i]->_private)
        rb_gc_mark(xpop->nodesetval->nodeTab[i]->_private);
    }
  }
}

void
ruby_xml_xpath_object_free(xmlXPathObjectPtr xpop)
{
  /* Before freeing this xpath object, get the 
     document it is dependent on if its a nodeset. */
  xmlDocPtr xdoc = ruby_xml_xpath_object_doc(xpop);

  /* Now free the xpath result */
  xmlXPathFreeObject(xpop);

  /* Now decrement the document object if this was a 
     XPATH_NODESET.  Note this could free the document,
     which is why we do it after freeing xpop.*/
  if (xdoc)
    ruby_xml_document_decr(xdoc);
}

VALUE
ruby_xml_xpath_object_wrap(xmlXPathObjectPtr xpop)
{
  xmlDocPtr xdoc;
  VALUE rval;

  if ( xpop==NULL )
    return Qnil;

  switch(xpop->type) {
  case XPATH_NODESET:
    xdoc = ruby_xml_xpath_object_doc(xpop);
    if (xdoc)
      ruby_xml_document_incr(xdoc);
    
    rval = Data_Wrap_Struct(cXMLXPathObject,
                            ruby_xml_xpath_object_mark,
                            ruby_xml_xpath_object_free,
                            xpop);
  
    break;
  case XPATH_BOOLEAN:
    if (xpop->boolval != 0)
      rval=Qtrue;
    else
      rval=Qfalse;
 
    xmlXPathFreeObject(xpop);
    break;
  case XPATH_NUMBER:
    rval=rb_float_new(xpop->floatval);
 
    xmlXPathFreeObject(xpop);
    break;
  case XPATH_STRING:
    rval=rb_str_new2(xpop->stringval);
 
    xmlXPathFreeObject(xpop);
    break;
  default:
    xmlXPathFreeObject(xpop);
    rval=Qnil;
  }
  return rval;
}

static VALUE
ruby_xml_xpath_object_tabref(xmlXPathObjectPtr xpop, int apos) {

  if (apos < 0 )
    apos=xpop->nodesetval->nodeNr+apos;

  if (apos < 0 || apos+1 > xpop->nodesetval->nodeNr )
    return Qnil;

  switch(xpop->nodesetval->nodeTab[apos]->type) {
  case XML_ATTRIBUTE_NODE:
    return ruby_xml_attr_wrap((xmlAttrPtr)xpop->nodesetval->nodeTab[apos]);
    break;
  default:
    return ruby_xml_node2_wrap(cXMLNode,
			       xpop->nodesetval->nodeTab[apos]);
  }
}

/*
 * call-seq:
 *    xpath_object.to_a -> [node, ..., node]
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
      nodeobj = ruby_xml_xpath_object_tabref(xpop, i);
      rb_ary_push(set_ary, nodeobj);
    }
  }
  
  return(set_ary);
}

/*
 * call-seq:
 *    xpath_object.empty? -> (true|false)
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
 *    xpath_object.each { |node| ... } -> self
 * 
 * Call the supplied block for each node in this set.
 */
VALUE
ruby_xml_xpath_object_each(VALUE self)
{
  xmlXPathObjectPtr xpop;
  int i;

  if ( ruby_xml_xpath_object_empty_q(self) == Qtrue )
    return Qnil;

  Data_Get_Struct(self,xmlXPathObject,xpop);

  for (i = 0; i < xpop->nodesetval->nodeNr; i++) {
    rb_yield(ruby_xml_xpath_object_tabref(xpop,i));
  }
  return(self);
}

/*
 * call-seq:
 *    xpath_object.first -> node
 *
 * Returns the first node in this node set, or nil if none exist.
 */
VALUE
ruby_xml_xpath_object_first(VALUE self) {
  if ( ruby_xml_xpath_object_empty_q(self) == Qtrue )
    return Qnil;
  
  return ruby_xml_xpath_object_tabref((xmlXPathObjectPtr)DATA_PTR(self),0);
}

/*
 * call-seq:
 * xpath_object[i] -> node
 *
 * array index into set of nodes
 */
VALUE
ruby_xml_xpath_object_aref(VALUE self, VALUE aref) {
  if ( ruby_xml_xpath_object_empty_q(self) == Qtrue )
    return Qnil;

  return ruby_xml_xpath_object_tabref((xmlXPathObjectPtr)DATA_PTR(self),
				      NUM2INT(aref));
}

/*
 * call-seq:
 *    xpath_object.length -> num
 * 
 * Obtain the length of the nodesetval node list.
 */
VALUE
ruby_xml_xpath_object_length(VALUE self) {
  xmlXPathObjectPtr xpop;

  if (ruby_xml_xpath_object_empty_q(self) == Qtrue)
    return INT2FIX(0);

  Data_Get_Struct(self,xmlXPathObject,xpop);

  return INT2NUM(xpop->nodesetval->nodeNr);
}

/*
 * call-seq:
 *    xpath_object.xpath_type -> int
 * 
 * Returns the XPath type of the result object. 
 * Possible values are defined as constants
 * on the XML::XPath class and include:
 * 
 * * XML::XPath::UNDEFINED
 * * XML::XPath::NODESET
 * * XML::XPath::BOOLEAN
 * * XML::XPath::NUMBER
 * * XML::XPath::STRING
 * * XML::XPath::POINT
 * * XML::XPath::RANGE
 * * XML::XPath::LOCATIONSET
 * * XML::XPath::USERS
 * * XML::XPath::XSLT_TREE 
 */
VALUE
ruby_xml_xpath_object_get_type(VALUE self)
{
  xmlXPathObjectPtr xpop;

  Data_Get_Struct(self,xmlXPathObject,xpop);

  return INT2FIX(xpop->type);
}


/*
 * call-seq:
 *    xpath_object.string -> String
 * 
 * Returns the original XPath expression as a string.
 */
VALUE
ruby_xml_xpath_object_string(VALUE self)
{
  xmlXPathObjectPtr xpop;

  Data_Get_Struct(self,xmlXPathObject,xpop);

  if (xpop->stringval == NULL)
    return Qnil;

  return rb_str_new2((const char*) xpop->stringval);
}

/* 
 * call-seq:
 *    nodes.debug -> (true|false)
 * 
 * Dump libxml debugging information to stdout.
 * Requires Libxml be compiled with debugging enabled.
 */
VALUE
ruby_xml_xpath_object_debug(VALUE self) {
  xmlXPathObjectPtr xpop;
  
  #ifndef LIBXML_DEBUG_ENABLED
  rb_raise(rb_eTypeError, "libxml was not compiled with debug support.");
  #endif

  Data_Get_Struct(self, xmlXPathObject, xpop);
  xmlXPathDebugDumpObject(stdout, xpop, 0);
  return Qnil;
}

void
ruby_init_xml_xpath_object(void) {
  cXMLXPathObject = rb_define_class_under(mXPath, "Object", rb_cObject);
  rb_include_module(cXMLXPathObject, rb_mEnumerable);
  rb_define_attr(cXMLXPathObject, "context", 1, 0);
  rb_define_method(cXMLXPathObject, "each", ruby_xml_xpath_object_each, 0);
  rb_define_method(cXMLXPathObject, "xpath_type", ruby_xml_xpath_object_get_type, 0);
  rb_define_method(cXMLXPathObject, "empty?", ruby_xml_xpath_object_empty_q, 0);
  rb_define_method(cXMLXPathObject, "first", ruby_xml_xpath_object_first, 0);
  rb_define_method(cXMLXPathObject, "length", ruby_xml_xpath_object_length, 0);
  rb_define_method(cXMLXPathObject, "size", ruby_xml_xpath_object_length, 0);
  rb_define_method(cXMLXPathObject, "to_a", ruby_xml_xpath_object_to_a, 0);
  rb_define_method(cXMLXPathObject, "[]", ruby_xml_xpath_object_aref, 1);
  rb_define_method(cXMLXPathObject, "string", ruby_xml_xpath_object_string, 0);
  rb_define_method(cXMLXPathObject, "debug", ruby_xml_xpath_object_debug, 0);
}
