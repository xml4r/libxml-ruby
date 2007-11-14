/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

#include "libxml.h"
#include "ruby_xml_xpath_context.h"

VALUE cXMLXPathContext;

/*
 * call-seq:
 *    context.doc => document
 * 
 * Obtain the XML::Document associated with this XPath.
 */
VALUE
ruby_xml_xpath_context_doc_get(VALUE self) {
  xmlXPathContextPtr ctxt;
  Data_Get_Struct(self, xmlXPathContext, ctxt);

  return ruby_xml_document_wrap2(ctxt->doc);
}


void
ruby_xml_xpath_context_free(xmlXPathContextPtr ctxt) {
  if (ctxt != NULL) {
    xmlXPathFreeContext(ctxt);
    ctxt = NULL;
  }
}


void
ruby_xml_xpath_context_mark(xmlXPathContextPtr ctxt) {
  if (ctxt == NULL ) return;
  if (ctxt->doc != NULL && ctxt->doc->_private != NULL)
    rb_gc_mark((VALUE)ctxt->doc->_private);
}


VALUE
ruby_xml_xpath_context_wrap(xmlXPathContextPtr ctxt) {
  return Data_Wrap_Struct(cXMLXPathContext,
			  ruby_xml_xpath_context_mark,
			  ruby_xml_xpath_context_free,
			  ctxt);
}


VALUE
ruby_xml_xpath_context_new(VALUE anode) {
  ruby_xml_document_t *rxd;
  ruby_xml_node *node;
  xmlXPathContextPtr ctxt;
  
  if (rb_obj_is_kind_of(anode,cXMLDocument) == Qtrue ) {
    Data_Get_Struct(anode,ruby_xml_document_t,rxd);
    if (rxd->doc == NULL) return(Qnil);

    ctxt = xmlXPathNewContext(rxd->doc);
    if (ctxt == NULL) return(Qnil);

  } else if (rb_obj_is_kind_of(anode,cXMLNode) == Qtrue ) {
    Data_Get_Struct(anode, ruby_xml_node, node);
    if (node->node->doc == NULL)
      rb_raise(rb_eTypeError,"Supplied node must be part of a document");

    ctxt = xmlXPathNewContext(node->node->doc);
    if (ctxt == NULL) return(Qnil);

  } else {
    rb_raise(rb_eTypeError,"create context requires a document or node. Supplied a %s?",
	     rb_obj_as_string(anode));
  }

  return ruby_xml_xpath_context_wrap(ctxt);
}

/*
 * call-seq:
 *    context.register_namespace(prefix, uri) => (true|false)
 * 
 * Register the specified namespace URI with the specified prefix
 * in this context.
 */
VALUE
ruby_xml_xpath_context_register_namespace(VALUE self, VALUE prefix, VALUE uri) {
  xmlXPathContextPtr ctxt;

  Data_Get_Struct(self, xmlXPathContext, ctxt);
  if (xmlXPathRegisterNs(ctxt, 
                         (xmlChar*)StringValuePtr(prefix), 
                         (xmlChar*)StringValuePtr(uri))
      == 0) {
    return(Qtrue);
  } else {
    /* Should raise an exception, IMHO (whose?, why shouldnt it? -danj)*/
    rb_warning("register namespace failed");
    return(Qfalse);
  }
}

// Rdoc needs to know 
#ifdef RDOC_NEVER_DEFINED
  mXML = rb_define_module("XML");
  cXMLXPath = rb_define_class_under(mXML, "XPath", rb_cObject);
#endif

void
ruby_init_xml_xpath_context(void) {
  cXMLXPathContext = rb_define_class_under(cXMLXPath, "Context", rb_cObject);

  rb_define_method(cXMLXPathContext, "register_namespace",
		   ruby_xml_xpath_context_register_namespace, 2);
  rb_define_method(cXMLXPathContext, "doc",
		   ruby_xml_xpath_context_doc_get, 0);
}
