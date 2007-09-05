/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

#include "libxml.h"
#include "ruby_xml_ns.h"

VALUE cXMLNS;

/*
 * call-seq:
 *    ns.href => "href"
 * 
 * Obtain the namespace's href.
 */
VALUE
ruby_xml_ns_href_get(VALUE self) {
  ruby_xml_ns *rxns;
  Data_Get_Struct(self, ruby_xml_ns, rxns);
  if (rxns->ns == NULL || rxns->ns->href == NULL)
    return(Qnil);
  else
    return(rb_str_new2((const char*)rxns->ns->href));
}


/*
 * call-seq:
 *    ns.href? => (true|false)
 * 
 * Determine whether this namespace has an href.
 */
VALUE
ruby_xml_ns_href_q(VALUE self) {
  ruby_xml_ns *rxns;
  Data_Get_Struct(self, ruby_xml_ns, rxns);
  if (rxns->ns == NULL || rxns->ns->href == NULL)
    return(Qfalse);
  else
    return(Qtrue);
}


void
ruby_xml_ns_free(ruby_xml_ns *rxns) {
  if (rxns->ns != NULL && !rxns->is_ptr) {
    xmlFreeNs(rxns->ns);
    rxns->ns = NULL;
  }

  free(rxns);
}


static void
ruby_xml_ns_mark(ruby_xml_ns *rxns) {
  if (rxns == NULL) return;
  if (!NIL_P(rxns->xd)) rb_gc_mark(rxns->xd);
}


VALUE
ruby_xml_ns_new(VALUE class, VALUE xd, xmlNsPtr ns) {
  ruby_xml_ns *rxns;

  rxns = ALLOC(ruby_xml_ns);
  rxns->is_ptr = 0;
  rxns->ns = ns;
  rxns->xd = xd;
  return(Data_Wrap_Struct(class, ruby_xml_ns_mark,
			  ruby_xml_ns_free, rxns));
}


VALUE
ruby_xml_ns_new2(VALUE class, VALUE xd, xmlNsPtr ns) {
  ruby_xml_ns *rxns;

  rxns = ALLOC(ruby_xml_ns);
  rxns->is_ptr = 1;
  rxns->ns = ns;
  rxns->xd = xd;
  return(Data_Wrap_Struct(class, ruby_xml_ns_mark,
			  ruby_xml_ns_free, rxns));
}


/*
 * call-seq:
 *    ns.next => ns
 * 
 * Obtain the next namespace.
 */
VALUE
ruby_xml_ns_next(VALUE self) {
  ruby_xml_ns *rxns;
  Data_Get_Struct(self, ruby_xml_ns, rxns);
  if (rxns->ns == NULL || rxns->ns->next == NULL)
    return(Qnil);
  else
    return(ruby_xml_ns_new2(cXMLNS, rxns->xd, rxns->ns->next));
}


/*
 * call-seq:
 *    ns.prefix => "prefix"
 *    ns.to_s   => "prefix"
 * 
 * Obtain the namespace's prefix.
 */
VALUE
ruby_xml_ns_prefix_get(VALUE self) {
  ruby_xml_ns *rxns;
  Data_Get_Struct(self, ruby_xml_ns, rxns);
  if (rxns->ns == NULL || rxns->ns->prefix == NULL)
    return(Qnil);
  else
    return(rb_str_new2((const char*)rxns->ns->prefix));
}


/*
 * call-seq:
 *    ns.prefix? => (true|false)
 * 
 * Determine whether this namespace has a prefix.
 */
VALUE
ruby_xml_ns_prefix_q(VALUE self) {
  ruby_xml_ns *rxns;
  Data_Get_Struct(self, ruby_xml_ns, rxns);
  if (rxns->ns == NULL || rxns->ns->prefix == NULL)
    return(Qfalse);
  else
    return(Qtrue);
}

// Rdoc needs to know 
#ifdef RDOC_NEVER_DEFINED
  mXML = rb_define_module("XML");
#endif

void
ruby_init_xml_ns(void) {
  cXMLNS = rb_define_class_under(mXML, "NS", rb_cObject);
  rb_define_method(cXMLNS, "href", ruby_xml_ns_href_get, 0);
  rb_define_method(cXMLNS, "href?", ruby_xml_ns_href_q, 0);
  rb_define_method(cXMLNS, "next", ruby_xml_ns_next, 0);
  rb_define_method(cXMLNS, "prefix", ruby_xml_ns_prefix_get, 0);
  rb_define_method(cXMLNS, "prefix?", ruby_xml_ns_prefix_q, 0);
  rb_define_method(cXMLNS, "to_s", ruby_xml_ns_prefix_get, 0);
}
