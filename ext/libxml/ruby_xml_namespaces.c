/* $Id: ruby_xml_ns.c 660 2008-12-02 08:52:06Z cfis $ */

/* Please see the LICENSE file for copyright and distribution information */

#include "ruby_libxml.h"
#include "ruby_xml_namespaces.h"

VALUE cXMLNamespaces;

/* Document-class: LibXML::XML::Namespaces
 *
 * The NS class is used to query information about
 * xml namespaces associated with particular nodes.
 * It can also be used to associate new namespaces
 * with an node. */

static VALUE rxml_namespaces_alloc(VALUE klass)
{
  return Data_Wrap_Struct(klass, NULL, NULL, NULL);
}

/*
 * call-seq:
 *    initialize(node) -> XML::Namespaces
 *
 * Create a new namespaces object.
 */
static VALUE rxml_namespaces_initialize(VALUE self, VALUE node)
{
  xmlNodePtr xnode;

  Check_Type(node, T_DATA);
  Data_Get_Struct(node, xmlNode, xnode);

  DATA_PTR(self) = xnode;
  return self;
}

/*
 * call-seq:
 *    namespaces.definitions -> [XML::Namespace, XML::Namespace]
 *
 * Returns an array of namespace objects that are defined
 * on this node.
 */
static VALUE rxml_namespaces_definitions(VALUE self)
{
  xmlNodePtr xnode;
  xmlNsPtr xns;
  VALUE arr;

  Data_Get_Struct(self, xmlNode, xnode);

  arr = rb_ary_new();
  xns = xnode->nsDef;

  while (xns)
  {
    VALUE anamespace = rxml_namespace_wrap(xns);
    rb_ary_push(arr, anamespace);
    xns = xns->next;
  }

  return arr;
}

/*
 * call-seq:
 *    namespaces.each {|XML::Namespace|}
 *
 * Iterates over the namespace objects that are
 * in scope on this node.
 */
static VALUE rxml_namespaces_each(VALUE self)
{
  xmlNodePtr xnode;
  xmlNsPtr *nsList, *xns;

  Data_Get_Struct(self, xmlNode, xnode);

  nsList = xmlGetNsList(xnode->doc, xnode);

  if (nsList == NULL)
    return (Qnil);

  for (xns = nsList; *xns != NULL; xns++)
  {
    VALUE ns = rxml_namespace_wrap(*xns);
    rb_yield(ns);
  }
  xmlFree(nsList);

  return Qnil;
}

/*
 * call-seq:
 *    namespaces.find_by_href(href) -> XML::Namespace
 *
 * Searches for a namespace that has the specified href.
 * The search starts at the current node and works upward
 * through the node's parents.  If a namespace is found,
 * then an XML::Namespace instance is returned, otherwise nil
 * is returned.
 */
static VALUE rxml_namespaces_find_by_href(VALUE self, VALUE href)
{
  xmlNodePtr xnode;
  xmlNsPtr xns;

  Check_Type(href, T_STRING);
  Data_Get_Struct(self, xmlNode, xnode);

  xns = xmlSearchNsByHref(xnode->doc, xnode, (xmlChar*) StringValuePtr(href));
  if (xns)
    return rxml_namespace_wrap(xns);
  else
    return Qnil;
}

/*
 * call-seq:
 *    namespaces.find_by_prefix(prefix=nil) -> XML::Namespace
 *
 * Searches for a namespace that has the specified prefix.
 * The search starts at the current node and works upward
 * through the node's parents.  If a namespace is found,
 * then an XML::NS instance is returned, otherwise nil
 * is returned.
 */
static VALUE rxml_namespaces_find_by_prefix(VALUE self, VALUE prefix)
{
  xmlNodePtr xnode;
  xmlNsPtr xns;
  xmlChar* xprefix = NULL;

  
  if (!NIL_P(prefix))
  {
    Check_Type(prefix, T_STRING);
    xprefix = (xmlChar*) StringValuePtr(prefix);
  }

  Data_Get_Struct(self, xmlNode, xnode);
  
  xns = xmlSearchNs(xnode->doc, xnode, xprefix);
  if (xns)
    return rxml_namespace_wrap(xns);
  else
    return Qnil;
}

/*
 * call-seq:
 *    namespaces.namespace -> XML::Namespace
 *
 * Returns the current node's namespace.
 */
static VALUE rxml_namespaces_namespace_get(VALUE self)
{
  xmlNodePtr xnode;
  Data_Get_Struct(self, xmlNode, xnode);
  return rxml_namespace_wrap(xnode->ns);
}

/*
 * call-seq:
 *    namespaces.namespace = XML::Namespace
 *
 * Sets the current node's namespace.
 */
static VALUE rxml_namespaces_namespace_set(VALUE self, VALUE ns)
{
  xmlNodePtr xnode;
  xmlNsPtr xns;

  Data_Get_Struct(self, xmlNode, xnode);

  Check_Type(ns, T_DATA);
  Data_Get_Struct(ns, xmlNs, xns);

  xmlSetNs(xnode, xns);
  return self;
}

/*
 * call-seq:
 *    namespaces.node -> XML::Node
 *
 * Returns the current node.
 */
static VALUE rxml_namespaces_node_get(VALUE self)
{
  xmlNodePtr xnode;
  Data_Get_Struct(self, xmlNode, xnode);
  return rxml_node_wrap(cXMLNode, xnode);
}



// Rdoc needs to know
#ifdef RDOC_NEVER_DEFINED
mLibXML = rb_define_module("LibXML");
mXML = rb_define_module_under(mLibXML, "XML");
#endif

void ruby_init_xml_namespaces(void)
{
  cXMLNamespaces = rb_define_class_under(mXML, "Namespaces", rb_cObject);
  rb_include_module(cXMLNamespaces, rb_mEnumerable);

  rb_define_alloc_func(cXMLNamespaces, rxml_namespaces_alloc);
  rb_define_method(cXMLNamespaces, "initialize", rxml_namespaces_initialize, 1);

  rb_define_method(cXMLNamespaces, "definitions", rxml_namespaces_definitions, 0);
  rb_define_method(cXMLNamespaces, "each", rxml_namespaces_each, 0);
  rb_define_method(cXMLNamespaces, "find_by_href", rxml_namespaces_find_by_href, 1);
  rb_define_method(cXMLNamespaces, "find_by_prefix", rxml_namespaces_find_by_prefix, 1);
  rb_define_method(cXMLNamespaces, "namespace", rxml_namespaces_namespace_get, 0);
  rb_define_method(cXMLNamespaces, "namespace=", rxml_namespaces_namespace_set, 1);
  rb_define_method(cXMLNamespaces, "node", rxml_namespaces_node_get, 0);
}
