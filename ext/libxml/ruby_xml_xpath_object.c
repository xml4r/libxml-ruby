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

static VALUE rxml_xpath_object_free(rxml_xpath_object* rxpop)
{
  xfree(rxpop);
}

static VALUE rxml_xpath_object_mark(rxml_xpath_object* rxpop)
{
  rb_gc_mark(rxpop->nodes);
  rb_gc_mark(rxpop->xpath_string);
  rb_gc_mark(rxpop->xpath_type);
}

static VALUE rxml_xpath_object_tabref(xmlXPathObjectPtr xpop, int apos)
{
  if (apos < 0)
    apos = xpop->nodesetval->nodeNr + apos;

  if (apos < 0 || apos + 1 > xpop->nodesetval->nodeNr)
    return Qnil;

  switch (xpop->nodesetval->nodeTab[apos]->type)
  {
  case XML_ATTRIBUTE_NODE:
    return rxml_attr_wrap((xmlAttrPtr) xpop->nodesetval->nodeTab[apos]);
    break;
  case XML_NAMESPACE_DECL:
    return rxml_namespace_wrap((xmlNsPtr)xpop->nodesetval->nodeTab[apos]);
    break;
  default:
    return rxml_node_wrap(xpop->nodesetval->nodeTab[apos]);
  }
}

static VALUE rxml_xpath_object_nodeset_wrap(xmlXPathObjectPtr xpop)
{
  int i;
  rxml_xpath_object* rxpop = ALLOC(rxml_xpath_object);
  rxpop->nodes = rb_ary_new();

  for (i = 0; i < xpop->nodesetval->nodeNr; i++)
  {
    VALUE node = rxml_xpath_object_tabref(xpop, i);
    rb_ary_push(rxpop->nodes, node);
  }

  rxpop->xpath_string = (xpop->stringval) ? rb_str_new2((const char*) xpop->stringval) : Qnil;
  rxpop->xpath_type = INT2FIX(xpop->type);

  return Data_Wrap_Struct(cXMLXPathObject, rxml_xpath_object_mark, rxml_xpath_object_free, rxpop);
}

VALUE rxml_xpath_object_wrap(xmlXPathObjectPtr xpop)
{
  VALUE result = Qnil;

  switch (xpop->type)
  {
  case XPATH_NODESET:
    result = rxml_xpath_object_nodeset_wrap(xpop);
    break;
  case XPATH_BOOLEAN:
    result = (xpop->boolval != 0) ? Qtrue : Qfalse;
    break;
  case XPATH_NUMBER:
    result = rb_float_new(xpop->floatval);
    break;
  case XPATH_STRING:
    result = rb_str_new2((const char*)xpop->stringval);
    break;
  default:
    rb_raise(rb_eArgError, "Unkown xpath object type: %d", xpop->type);
  }

  xmlXPathFreeObject(xpop);
  return result;
}

/*
 * call-seq:
 *    xpath_object.each { |node| ... } -> self
 *
 * Call the supplied block for each node in this set.
 */
static VALUE rxml_xpath_object_each(VALUE self)
{
  int i;
  rxml_xpath_object* rxpop;
  Data_Get_Struct(self, rxml_xpath_object, rxpop);
 
  for (i = 0; i < RARRAY_LEN(rxpop->nodes); i++)
  {
    rb_yield(rb_ary_entry(rxpop->nodes, i));
  }
  return (self);
}

/*
 * call-seq:
 * xpath_object[i] -> node
 *
 * array index into set of nodes
 */
static VALUE rxml_xpath_object_aref(VALUE self, VALUE aref)
{
  rxml_xpath_object* rxpop;
  Data_Get_Struct(self, rxml_xpath_object, rxpop);
  return rb_ary_entry(rxpop->nodes, NUM2INT(aref));
}

/*
 * call-seq:
 *    xpath_object.length -> num
 * Obtain the length of the nodesetval node list.
 */
static VALUE rxml_xpath_object_length(VALUE self)
{
  rxml_xpath_object* rxpop;
  Data_Get_Struct(self, rxml_xpath_object, rxpop);
  return INT2NUM(RARRAY_LEN(rxpop->nodes));
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
static VALUE rxml_xpath_object_get_type(VALUE self)
{
  rxml_xpath_object* rxpop;
  Data_Get_Struct(self, rxml_xpath_object, rxpop);
  return rxpop->xpath_type;
}

/*
 * call-seq:
 *    xpath_object.string -> String
 *
 * Returns the original XPath expression as a string.
 */
static VALUE rxml_xpath_object_string(VALUE self)
{
  rxml_xpath_object* rxpop;
  Data_Get_Struct(self, rxml_xpath_object, rxpop);
  return rxpop->xpath_string;
}

// Rdoc needs to know 
#ifdef RDOC_NEVER_DEFINED
  mLibXML = rb_define_module("LibXML");
  mXPath = rb_define_module_under(mLibXML, "XPath");
#endif

void ruby_init_xml_xpath_object(void)
{
  cXMLXPathObject = rb_define_class_under(mXPath, "Object", rb_cArray);
  rb_include_module(cXMLXPathObject, rb_mEnumerable);
  rb_define_attr(cXMLXPathObject, "context", 1, 0);
  rb_define_method(cXMLXPathObject, "each", rxml_xpath_object_each, 0);
  rb_define_method(cXMLXPathObject, "xpath_type", rxml_xpath_object_get_type, 0);
  rb_define_method(cXMLXPathObject, "length", rxml_xpath_object_length, 0);
  rb_define_method(cXMLXPathObject, "[]", rxml_xpath_object_aref, 1);
  rb_define_method(cXMLXPathObject, "string", rxml_xpath_object_string, 0);
}
