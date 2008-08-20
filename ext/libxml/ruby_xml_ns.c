/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

#include "ruby_libxml.h"
#include "ruby_xml_ns.h"

VALUE cXMLNS;


VALUE
ruby_xml_ns_alloc(VALUE klass) {
  return Data_Wrap_Struct(cXMLNS, NULL, NULL, NULL);
}

/*
 * call-seq:
 *    initialize(node, "href", "prefix")
 * 
 * Create a new namespace attached to the specified node with the
 * give prefix and namespace.
 *
 *  XML::NS.new(node, "xlink", "http://www.w3.org/1999/xlink")
 */
VALUE
ruby_xml_ns_initialize(VALUE self, VALUE node, VALUE href, VALUE prefix) {
  xmlNodePtr xnode;
  xmlChar *xmlPrefix;
	xmlNsPtr xns;

  Data_Get_Struct(node, xmlNode, xnode);
	/* Prefix can be null - that means its the default namespace */
	xmlPrefix = NIL_P(prefix) ? NULL : StringValuePtr(prefix);
	xns = xmlNewNs(xnode, (xmlChar*)StringValuePtr(href), xmlPrefix);

  DATA_PTR(self) = xns;
  return self;  
}

VALUE
ruby_xml_ns_wrap(xmlNsPtr xns) {
  return(Data_Wrap_Struct(cXMLNS, NULL, NULL, xns));
}


/*
 * call-seq:
 *    ns.href -> "href"
 * 
 * Obtain the namespace's href.
 */
VALUE
ruby_xml_ns_href_get(VALUE self) {
  xmlNsPtr xns;
  Data_Get_Struct(self, xmlNs, xns);
  if (xns == NULL || xns->href == NULL)
    return(Qnil);
  else
    return(rb_str_new2((const char*)xns->href));
}


/*
 * call-seq:
 *    ns.href? -> (true|false)
 * 
 * Determine whether this namespace has an href.
 */
VALUE
ruby_xml_ns_href_q(VALUE self) {
  xmlNsPtr xns;
  Data_Get_Struct(self, xmlNs, xns);
  if (xns == NULL || xns->href == NULL)
    return(Qfalse);
  else
    return(Qtrue);
}


/*
 * call-seq:
 *    ns.next -> ns
 * 
 * Obtain the next namespace.
 */
VALUE
ruby_xml_ns_next(VALUE self) {
  xmlNsPtr xns;
  Data_Get_Struct(self, xmlNs, xns);
  if (xns == NULL || xns->next == NULL)
    return(Qnil);
  else
    return(ruby_xml_ns_wrap(xns->next));
}


/*
 * call-seq:
 *    ns.prefix -> "prefix"
 *    ns.to_s   -> "prefix"
 * 
 * Obtain the namespace's prefix.
 */
VALUE
ruby_xml_ns_prefix_get(VALUE self) {
  xmlNsPtr xns;
  Data_Get_Struct(self, xmlNs, xns);
  if (xns == NULL || xns->prefix == NULL)
    return(Qnil);
  else
    return(rb_str_new2((const char*)xns->prefix));
}


/*
 * call-seq:
 *    ns.prefix? -> (true|false)
 * 
 * Determine whether this namespace has a prefix.
 */
VALUE
ruby_xml_ns_prefix_q(VALUE self) {
  xmlNsPtr xns;
  Data_Get_Struct(self, xmlNs, xns);
  if (xns == NULL || xns->prefix == NULL)
    return(Qfalse);
  else
    return(Qtrue);
}

// Rdoc needs to know 
#ifdef RDOC_NEVER_DEFINED
  mLibXML = rb_define_module("LibXML");
  mXML = rb_define_module_under(mLibXML, "XML");
#endif

void
ruby_init_xml_ns(void) {
  cXMLNS = rb_define_class_under(mXML, "NS", rb_cObject);
  rb_define_alloc_func(cXMLNS, ruby_xml_ns_alloc);
  rb_define_method(cXMLNS, "initialize", ruby_xml_ns_initialize, 3);
  rb_define_method(cXMLNS, "href", ruby_xml_ns_href_get, 0);
  rb_define_method(cXMLNS, "href?", ruby_xml_ns_href_q, 0);
  rb_define_method(cXMLNS, "next", ruby_xml_ns_next, 0);
  rb_define_method(cXMLNS, "prefix", ruby_xml_ns_prefix_get, 0);
  rb_define_method(cXMLNS, "prefix?", ruby_xml_ns_prefix_q, 0);
  rb_define_method(cXMLNS, "to_s", ruby_xml_ns_prefix_get, 0);
}
