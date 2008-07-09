/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

#include "ruby_libxml.h"
#include "ruby_xml_xpointer.h"

VALUE cXMLXPointer;
VALUE eXMLXPointerInvalidExpression;

VALUE
ruby_xml_xpointer_point(VALUE class, VALUE rnode, VALUE xptr_str) {
#ifdef LIBXML_XPTR_ENABLED
  xmlNodePtr xnode;
  xmlXPathContextPtr ctxt;
  VALUE rxptr_xpth_ctxt, rxxp;
  xmlXPathObjectPtr xpath;

  Check_Type(xptr_str, T_STRING);
  if (rb_obj_is_kind_of(rnode, cXMLNode) == Qfalse)
    rb_raise(rb_eTypeError, "require an XML::Node object");

  Data_Get_Struct(rnode, xmlNodePtr, xnode);

  rxptr_xpth_ctxt =
    ruby_xml_xpath_context_wrap(ctxt=xmlXPtrNewContext(xnode->doc, xnode, NULL));

  if (NIL_P(rxptr_xpth_ctxt))
    return(Qnil);

  xpath = xmlXPtrEval((xmlChar*)StringValuePtr(xptr_str), ctxt);
  if (xpath == NULL)
    rb_raise(eXMLXPointerInvalidExpression, "invalid xpointer expression");

  rxxp = ruby_xml_xpath_object_wrap(xpath);
  return(rxxp);
#else
  rb_warn("libxml was compiled without XPointer support");
  return(Qfalse);
#endif
}


VALUE
ruby_xml_xpointer_point2(VALUE node, VALUE xptr_str) {
  return(ruby_xml_xpointer_point(cXMLXPointer, node, xptr_str));
}


/*
 * call-seq:
 *    XML::XPointer.range(start_node, end_node) -> xpath
 * 
 * Create an xpath representing the range between the supplied
 * start and end node.
 */
VALUE
ruby_xml_xpointer_range(VALUE class, VALUE rstart, VALUE rend) {
#ifdef LIBXML_XPTR_ENABLED
  xmlNodePtr start, end;
  VALUE rxxp;
  xmlXPathObjectPtr xpath;

  if (rb_obj_is_kind_of(rstart, cXMLNode) == Qfalse)
    rb_raise(rb_eTypeError, "require an XML::Node object as a starting point");
  if (rb_obj_is_kind_of(rend, cXMLNode) == Qfalse)
    rb_raise(rb_eTypeError, "require an XML::Node object as an ending point");

  Data_Get_Struct(rstart, xmlNodePtr, start);
  if (start == NULL)
    return(Qnil);

  Data_Get_Struct(rend, xmlNodePtr, end);
  if (end == NULL)
    return(Qnil);

  xpath = xmlXPtrNewRangeNodes(start, end);
  if (xpath == NULL)
    rb_fatal("You shouldn't be able to have this happen");

  rxxp = ruby_xml_xpath_object_wrap(xpath);
  return(rxxp);
#else
  rb_warn("libxml was compiled without XPointer support");
  return(Qfalse);
#endif
}

// Rdoc needs to know 
#ifdef RDOC_NEVER_DEFINED
  mXML = rb_define_module("XML");
#endif

void
ruby_init_xml_xpointer(void) {
  cXMLXPointer = rb_define_class_under(mXML, "XPointer", rb_cObject);
  eXMLXPointerInvalidExpression = rb_define_class_under(cXMLXPointer, "InvalidExpression", eXMLError);

  rb_define_singleton_method(cXMLXPointer, "range", ruby_xml_xpointer_range, 2);
}
