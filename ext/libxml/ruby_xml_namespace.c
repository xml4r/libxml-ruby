/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

#include "ruby_libxml.h"
#include "ruby_xml_namespace.h"

VALUE cXMLNamespace;

/* Document-class: LibXML::XML::NS
 *
 * The NS class is used to query information about
 * xml namespaces associated with particular nodes.
 * It can also be used to associate new namespaces
 * with an node. */

static VALUE rxml_namespace_alloc(VALUE klass)
{
  return Data_Wrap_Struct(klass, NULL, NULL, NULL);
}

/*
 * call-seq:
 *    initialize(node, "prefix", "href") -> XML::NS
 *
 * Create a new namespace and attaches it to the specified node.
 * The namespace is therefore valid for the node and all its
 * children.  You may not create two namespaces with the same
 * prefix on the same node.
 *
 *   ns = XML::NS.new(node, "xlink", "http://www.w3.org/1999/xlink")
 */
static VALUE rxml_namespace_initialize(VALUE self, VALUE node, VALUE prefix,
    VALUE href)
{
  xmlNodePtr xnode;
  xmlChar *xmlPrefix;
  xmlNsPtr xns;

  Check_Type(node, T_DATA);
  Data_Get_Struct(node, xmlNode, xnode);

  /* Prefix can be null - that means its the default namespace */
  xmlPrefix = NIL_P(prefix) ? NULL : (xmlChar *)StringValuePtr(prefix);
  xns = xmlNewNs(xnode, (xmlChar*) StringValuePtr(href), xmlPrefix);

  if (!xns)
    rxml_raise(&xmlLastError);

  DATA_PTR(self) = xns;
  return self;
}

VALUE rxml_namespace_wrap(xmlNsPtr xns)
{
  return (Data_Wrap_Struct(cXMLNamespace, NULL, NULL, xns));
}

/*
 * call-seq:
 *    ns.href -> "href"
 *
 * Obtain the namespace's href.
 */
static VALUE rxml_namespace_href_get(VALUE self)
{
  xmlNsPtr xns;
  Data_Get_Struct(self, xmlNs, xns);
  if (xns == NULL || xns->href == NULL)
    return (Qnil);
  else
    return (rb_str_new2((const char*) xns->href));
}

/*
 * call-seq:
 *    ns.href? -> (true|false)
 *
 * Determine whether this namespace has an href.
 */
static VALUE rxml_namespace_href_q(VALUE self)
{
  xmlNsPtr xns;
  Data_Get_Struct(self, xmlNs, xns);
  if (xns == NULL || xns->href == NULL)
    return (Qfalse);
  else
    return (Qtrue);
}

/*
 * call-seq:
 *    ns.next -> ns
 *
 * Obtain the next namespace.
 */
static VALUE rxml_namespace_next(VALUE self)
{
  xmlNsPtr xns;
  Data_Get_Struct(self, xmlNs, xns);
  if (xns == NULL || xns->next == NULL)
    return (Qnil);
  else
    return (rxml_namespace_wrap(xns->next));
}

/*
 * call-seq:
 *    ns.prefix -> "prefix"
 *    ns.to_s   -> "prefix"
 *
 * Obtain the namespace's prefix.
 */
static VALUE rxml_namespace_prefix_get(VALUE self)
{
  xmlNsPtr xns;
  Data_Get_Struct(self, xmlNs, xns);
  if (xns == NULL || xns->prefix == NULL)
    return (Qnil);
  else
    return (rb_str_new2((const char*) xns->prefix));
}

/*
 * call-seq:
 *    ns.prefix? -> (true|false)
 *
 * Determine whether this namespace has a prefix.
 */
static VALUE rxml_namespace_prefix_q(VALUE self)
{
  xmlNsPtr xns;
  Data_Get_Struct(self, xmlNs, xns);
  if (xns == NULL || xns->prefix == NULL)
    return (Qfalse);
  else
    return (Qtrue);
}

// Rdoc needs to know
#ifdef RDOC_NEVER_DEFINED
mLibXML = rb_define_module("LibXML");
mXML = rb_define_module_under(mLibXML, "XML");
#endif

void ruby_init_xml_namespace(void)
{
  cXMLNamespace = rb_define_class_under(mXML, "Namespace", rb_cObject);
  rb_define_alloc_func(cXMLNamespace, rxml_namespace_alloc);
  rb_define_method(cXMLNamespace, "initialize", rxml_namespace_initialize, 3);
  rb_define_method(cXMLNamespace, "href", rxml_namespace_href_get, 0);
  rb_define_method(cXMLNamespace, "href?", rxml_namespace_href_q, 0);
  rb_define_method(cXMLNamespace, "next", rxml_namespace_next, 0);
  rb_define_method(cXMLNamespace, "prefix", rxml_namespace_prefix_get, 0);
  rb_define_method(cXMLNamespace, "prefix?", rxml_namespace_prefix_q, 0);
  rb_define_method(cXMLNamespace, "to_s", rxml_namespace_prefix_get, 0);
}
