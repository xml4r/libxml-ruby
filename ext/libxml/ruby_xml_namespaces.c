/* Please see the LICENSE file for copyright and distribution information */

#include "ruby_libxml.h"
#include "ruby_xml_namespaces.h"

VALUE cXMLNamespaces;

static const rb_data_type_t rxml_namespaces_type = {
  "libxml/namespaces",
  {NULL, NULL, NULL},
};

/* Document-class: LibXML::XML::Namespaces
 *
 * The XML::Namespaces class is used to access information about
 * a node's namespaces.  For each node, libxml maintains:
 *
 * * The node's namespace (#namespace)
 * * Which namespaces are defined on the node (#definnitions)
 * * Which namespaces are in scope for the node (#each)
 *
 * Let's look at an example:
 *
 *   <soap:Envelope xmlns:soap="http://schemas.xmlsoap.org/soap/envelope/"
 *                  xmlns:xsd="http://www.w3.org/2001/XMLSchema">
 *     <soap:Body>
 *       <order xmlns="http://mynamespace.com"/>
 *     </soap:Body>
 *   </soap>
 *
 * The Envelope node is in the soap namespace.  It contains
 * two namespace definitions, one for soap and one for xsd.
 * 
 * The Body node is also in the soap namespace and does not
 * contain any namespaces.  However, the soap and xsd namespaces
 * are both in context.
 *
 * The order node is in its default namespace and contains
 * one namespace definition (http://mynamespace.com).  There
 * are three namespaces in context soap, xsd and the 
 * default namespace.
*/

static VALUE rxml_namespaces_alloc(VALUE klass)
{
  return TypedData_Wrap_Struct(klass, &rxml_namespaces_type, NULL);
}

/*
 * call-seq:
 *    initialize(XML::Node) -> XML::Namespaces
 *
 * Creates a new namespaces object.  Generally you
 * do not call this method directly, but instead
 * access a namespaces object via XML::Node#namespaces.
 *
 * Usage:
 *
 *   doc = XML::Document.string('<soap:Envelope xmlns:soap="http://schemas.xmlsoap.org/soap/envelope/"/>')
 *   namespaces = new XML::Namespaces(doc.root)
 */
static VALUE rxml_namespaces_initialize(VALUE self, VALUE node)
{
  xmlNodePtr xnode;

  TypedData_Get_Struct(node, xmlNode, &rxml_node_data_type, xnode);

  RTYPEDDATA_DATA(self) = xnode;
  return self;
}

/*
 * call-seq:
 *    namespaces.definitions -> [XML::Namespace, XML::Namespace]
 *
 * Returns an array of XML::Namespace objects that are 
 * defined on this node.
 *
 * Usage:
 *
 *   doc = XML::Document.string('<soap:Envelope xmlns:soap="http://schemas.xmlsoap.org/soap/envelope/"/>')
 *   defs = doc.root.namespaces.definitions
 */
static VALUE rxml_namespaces_definitions(VALUE self)
{
  xmlNodePtr xnode;
  xmlNsPtr xns;
  VALUE arr;

  TypedData_Get_Struct(self, xmlNode, &rxml_namespaces_type, xnode);

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
 * in context for this node.
 *
 * Usage:
 *
 *   doc = XML::Document.string('<soap:Envelope xmlns:soap="http://schemas.xmlsoap.org/soap/envelope/"/>')
 *   doc.root.namespaces.each do |ns|
 *     ..
 *   end
 */
static VALUE rxml_namespaces_each_yield(VALUE data)
{
  xmlNsPtr*nsList = (xmlNsPtr*)data;
  xmlNsPtr*xns;

  for (xns = nsList; *xns != NULL; xns++)
  {
    VALUE ns = rxml_namespace_wrap(*xns);
    rb_yield(ns);
  }

  return Qnil;
}

static VALUE rxml_namespaces_free_list(VALUE data)
{
  xmlNsPtr*nsList = (xmlNsPtr*)data;
  xmlFree(nsList);
  return Qnil;
}

static VALUE rxml_namespaces_each(VALUE self)
{
  xmlNodePtr xnode;
  xmlNsPtr*nsList;

  TypedData_Get_Struct(self, xmlNode, &rxml_namespaces_type, xnode);

  nsList = xmlGetNsList(xnode->doc, xnode);

  if (nsList == NULL)
    return (Qnil);

  rb_ensure(rxml_namespaces_each_yield, (VALUE)nsList,
            rxml_namespaces_free_list, (VALUE)nsList);

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
 *
 * Usage:
 *
 *   doc = XML::Document.string('<soap:Envelope xmlns:soap="http://schemas.xmlsoap.org/soap/envelope/"/>')
 *   ns = doc.root.namespaces.find_by_href('http://schemas.xmlsoap.org/soap/envelope/')
 *   assert_equal('soap', ns.prefix)
 *   assert_equal('http://schemas.xmlsoap.org/soap/envelope/', ns.href)
 */
static VALUE rxml_namespaces_find_by_href(VALUE self, VALUE href)
{
  xmlNodePtr xnode;
  xmlNsPtr xns;

  Check_Type(href, T_STRING);
  TypedData_Get_Struct(self, xmlNode, &rxml_namespaces_type, xnode);

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
 * then an XML::Namespace instance is returned, otherwise nil
 * is returned.
 *
 * Usage:
 *
 *   doc = XML::Document.string('<soap:Envelope xmlns:soap="http://schemas.xmlsoap.org/soap/envelope/"/>')
 *   ns = doc.root.namespaces.find_by_prefix('soap')
 *   assert_equal('soap', ns.prefix)
 *   assert_equal('http://schemas.xmlsoap.org/soap/envelope/', ns.href)
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

  TypedData_Get_Struct(self, xmlNode, &rxml_namespaces_type, xnode);
  
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
 *
 * Usage:
 *
 *   doc = XML::Document.string('<soap:Envelope xmlns:soap="http://schemas.xmlsoap.org/soap/envelope/"/>')
 *   ns = doc.root.namespaces.namespace
 *   assert_equal('soap', ns.prefix)
 *   assert_equal('http://schemas.xmlsoap.org/soap/envelope/', ns.href)
 */
static VALUE rxml_namespaces_namespace_get(VALUE self)
{
  xmlNodePtr xnode;
  TypedData_Get_Struct(self, xmlNode, &rxml_namespaces_type, xnode);

  if (xnode->ns)
    return rxml_namespace_wrap(xnode->ns);
  else
    return Qnil;
}

/*
 * call-seq:
 *    namespaces.namespace = XML::Namespace
 *
 * Sets the current node's namespace.  
 *
 * Basic usage:
 *
 *   # Create a node
 *   node = XML::Node.new('Envelope')
 *   
 *   # Define the soap namespace - this does *not* put the node in the namespace
 *   ns = XML::Namespace.new(node, 'soap', 'http://schemas.xmlsoap.org/soap/envelope/')
 *   assert_equal("<Envelope xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\"/>", node.to_s)
 *
 *   # Now put the node in the soap namespace, not how the string representation changes
 *   node.namespaces.namespace = ns
 *   assert_equal("<soap:Envelope xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\"/>", node.to_s)
 */
static VALUE rxml_namespaces_namespace_set(VALUE self, VALUE ns)
{
  xmlNodePtr xnode;
  xmlNsPtr xns;

  TypedData_Get_Struct(self, xmlNode, &rxml_namespaces_type, xnode);

  TypedData_Get_Struct(ns, xmlNs, &rxml_namespace_type, xns);

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
  TypedData_Get_Struct(self, xmlNode, &rxml_namespaces_type, xnode);
  return rxml_node_wrap(xnode);
}

void rxml_init_namespaces(void)
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
