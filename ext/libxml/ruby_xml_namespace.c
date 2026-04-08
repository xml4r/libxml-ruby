/* Please see the LICENSE file for copyright and distribution information */

#include "ruby_libxml.h"
#include "ruby_xml_namespace.h"

VALUE cXMLNamespace;

const rb_data_type_t rxml_namespace_type = {
  "libxml/namespace",
  {NULL, NULL, NULL},
};

/* Document-class: LibXML::XML::Namespace
 *
 * The Namespace class represents an XML namespace.
 * To add a namespace to a node, create a new instance
 * of this class.  Note that this does *not* assign the
 * node to the namespace. To do that see the 
 * XML::Namespaces#namespace method.
 *
 * Usage:
 *
 *   node = XML::Node.new('<Envelope>')
 *   XML::Namespace.new(node, 'soap', 'http://schemas.xmlsoap.org/soap/envelope/')
 *   assert_equal("<Envelope xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\"/>", node.to_s)
 *   assert_nil(node.namespaces.namespace)
 */

/* Namespaces are owned and freed by their nodes.  Thus, its easier for the
   ruby bindings to not manage attribute memory management. */

static VALUE rxml_namespace_alloc(VALUE klass)
{
  return TypedData_Wrap_Struct(klass, &rxml_namespace_type, NULL);
}

VALUE rxml_namespace_wrap(xmlNsPtr xns)
{
  return TypedData_Wrap_Struct(cXMLNamespace, &rxml_namespace_type, xns);
}


/*
 * call-seq:
 *    initialize(node, "prefix", "href") -> XML::Namespace
 *
 * Create a new namespace and adds it to the specified node.
 * Note this does *not* assign the node to the namespace.
 * To do that see the XML::Namespaces#namespace method.
 */
static VALUE rxml_namespace_initialize(VALUE self, VALUE node, VALUE prefix,
    VALUE href)
{
  xmlNodePtr xnode;
  xmlChar *xmlPrefix;
  xmlNsPtr xns;

  TypedData_Get_Struct(node, xmlNode, &rxml_node_data_type, xnode);
  xmlResetLastError();

  /* Prefix can be null - that means its the default namespace */
  xmlPrefix = NIL_P(prefix) ? NULL : (xmlChar *)StringValuePtr(prefix);
  xns = xmlNewNs(xnode, (xmlChar*) StringValuePtr(href), xmlPrefix);

  RTYPEDDATA_DATA(self) = xns;
  return self;
}

/*
 * call-seq:
 *    ns.href -> "href"
 *
 * Usage:
 *
 *   doc = XML::Document.string('<soap:Envelope xmlns:soap="http://schemas.xmlsoap.org/soap/envelope/"/>')
 *   ns = doc.root.namespaces.find_by_href('http://schemas.xmlsoap.org/soap/envelope/')
 *   assert_equal('http://schemas.xmlsoap.org/soap/envelope/', ns.href)
 */
static VALUE rxml_namespace_href_get(VALUE self)
{
  xmlNsPtr xns;
  TypedData_Get_Struct(self, xmlNs, &rxml_namespace_type, xns);
  if (xns->href == NULL)
    return Qnil;
  else
    return rxml_new_cstr( xns->href, NULL);
}

/*
 * call-seq:
 *    ns.node_type -> num
 *
 * Obtain this namespace's type identifier.
 */
static VALUE rxml_namespace_node_type(VALUE self)
{
  xmlNsPtr xns;
  TypedData_Get_Struct(self, xmlNs, &rxml_namespace_type, xns);
  return INT2NUM(xns->type);
}

/*
 * call-seq:
 *    ns.prefix -> "prefix"
 *
 * Obtain the namespace's prefix.
 *
 * Usage:
 *
 *   doc = XML::Document.string('<soap:Envelope xmlns:soap="http://schemas.xmlsoap.org/soap/envelope/"/>')
 *   ns = doc.root.namespaces.find_by_href('http://schemas.xmlsoap.org/soap/envelope/')
 *   assert_equal('soap', ns.prefix)
 */
static VALUE rxml_namespace_prefix_get(VALUE self)
{
  xmlNsPtr xns;
  TypedData_Get_Struct(self, xmlNs, &rxml_namespace_type, xns);
  if (xns->prefix == NULL)
    return Qnil;
  else
    return rxml_new_cstr( xns->prefix, NULL);
}

/*
 * call-seq:
 *    ns.next -> XML::Namespace
 *
 * Obtain the next namespace.
 *
 * Usage:
 *
 *   doc = XML::Document.string('<soap:Envelope xmlns:soap="http://schemas.xmlsoap.org/soap/envelope/"/>')
 *   ns = doc.root.namespaces.find_by_href('http://schemas.xmlsoap.org/soap/envelope/')
 *   assert_nil(ns.next)
 */
static VALUE rxml_namespace_next(VALUE self)
{
  xmlNsPtr xns;
  TypedData_Get_Struct(self, xmlNs, &rxml_namespace_type, xns);
  /* Guard against libxml2's XPath hack where xns->next stores a parent
     element pointer instead of the next namespace (see xmlXPathNodeSetAddNs
     in xpath.c).  xmlNs.type and xmlNode.type share the same struct offset,
     so checking the type is safe even when next points to an xmlNode. */
  if (xns == NULL || xns->next == NULL || xns->next->type != XML_LOCAL_NAMESPACE)
    return (Qnil);

  return rxml_namespace_wrap(xns->next);
}

void rxml_init_namespace(void)
{
  cXMLNamespace = rb_define_class_under(mXML, "Namespace", rb_cObject);
  rb_define_alloc_func(cXMLNamespace, rxml_namespace_alloc);
  rb_define_method(cXMLNamespace, "initialize", rxml_namespace_initialize, 3);
  rb_define_method(cXMLNamespace, "href", rxml_namespace_href_get, 0);
  rb_define_method(cXMLNamespace, "next", rxml_namespace_next, 0);
  rb_define_method(cXMLNamespace, "node_type", rxml_namespace_node_type, 0);
  rb_define_method(cXMLNamespace, "prefix", rxml_namespace_prefix_get, 0);
}
