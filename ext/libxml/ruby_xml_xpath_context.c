/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

#include "ruby_libxml.h"
#include "ruby_xml_xpath_context.h"
#include <st.h>


/*
 * Document-class: LibXML::XPath::Context
 * 
 * The LibXML::XPath::Context class is used to by the XPath engine
 * to evaluate an XPath expression.  Generally you should not
 * directly use this class, and instead use either LibXML::Document#find
 * or LibXML::Node#find.
 */

VALUE cXMLXPathContext;


void
ruby_xml_xpath_context_free(xmlXPathContextPtr ctxt) {
  xmlXPathFreeContext(ctxt);
}


VALUE
ruby_xml_xpath_context_alloc(VALUE klass) {
  return Data_Wrap_Struct(cXMLXPathContext,
			  NULL,
			  ruby_xml_xpath_context_free,
			  NULL);
}

/* call-seq:
 *    XPath::Context.new(node) -> XPath::Context
 * 
 * Creates a new XPath context for the specified document.  The
 * context can then be used to evaluate an XPath expression.
 *
 *  doc = LibXML::Document.string('<header><first>hi</first></header>')
 *  context = XPath::Context.new(doc)
 *  nodes = XPath::Object.new('//first', context)
 *  nodes.length == 1
 */
VALUE
ruby_xml_xpath_context_initialize(VALUE self, VALUE node) {
  ruby_xml_document_t *rxd;
  VALUE document;
  #ifndef LIBXML_XPATH_ENABLED
  rb_raise(rb_eTypeError, "libxml was not compiled with XPath support.");
  #endif

  if (rb_obj_is_kind_of(node, cXMLNode) == Qtrue)
  {
    document = rb_funcall(node, rb_intern("doc"), 0);
    if NIL_P(document)
     rb_raise(rb_eTypeError, "Supplied node must belong to a document.");
  }
  else if (rb_obj_is_kind_of(node, cXMLDocument) == Qtrue)
  {
    document = node;
  }
  else
  {
    rb_raise(rb_eTypeError, "Supplied argument must be a document or node.");
  }
  
  Data_Get_Struct(document, ruby_xml_document_t, rxd);
  DATA_PTR(self) = xmlXPathNewContext(rxd->doc);
  
  /* Save the doc as an attribute, this will expose it to Ruby's GC. */
  rb_iv_set(self, "@doc", document);    

  return self;
}


/*
 * call-seq:
 *    context.register_namespace(prefix, uri) -> (true|false)
 * 
 * Register the specified namespace URI with the specified prefix
 * in this context.
 
 *   context.register_namespace('xi', 'http://www.w3.org/2001/XInclude')
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

/* call-seq:
 *    context.register_namespaces_from_node(node) -> self
 * 
 * Helper method to read in namespaces defined on a node.
 *
 *  doc = LibXML::Document.string('<header><first>hi</first></header>')
 *  context = XPath::Context.new(doc)
 *  context.register_namespaces_from_node(doc.root)
 */
VALUE
ruby_xml_xpath_context_register_namespaces_from_node(VALUE self, VALUE node) {
  xmlXPathContextPtr xctxt;
  xmlNodePtr xnode;
  xmlNsPtr *xnsArr;

  Data_Get_Struct(self, xmlXPathContext, xctxt);
  
  if (rb_obj_is_kind_of(node, cXMLDocument) == Qtrue)
  {
    ruby_xml_document_t *rdoc;
    Data_Get_Struct(node, ruby_xml_document_t, rdoc);
    xnode = xmlDocGetRootElement(rdoc->doc);
  }
  else if (rb_obj_is_kind_of(node, cXMLNode) == Qtrue)
  {
    Data_Get_Struct(node, xmlNode, xnode);
  }
  else
  {
    rb_raise(rb_eTypeError, "The first argument must be a document or node.");
  } 
  
  xnsArr = xmlGetNsList(xnode->doc, xnode);
  
  if (xnsArr)
  {
    xmlNsPtr xns = *xnsArr;
    
    while (xns) {
      /* If there is no prefix, then this is the default namespace.
         Skip it for now. */
      if (xns->prefix)
      {
        VALUE prefix = rb_str_new2(xns->prefix);
        VALUE uri = rb_str_new2(xns->href);
        ruby_xml_xpath_context_register_namespace(self, prefix, uri);
      }
      xns = xns->next;
    }
    xmlFree(xnsArr);
  }
    
  return self;
}

static int
iterate_ns_hash(st_data_t prefix, st_data_t uri, st_data_t self)
{
  ruby_xml_xpath_context_register_namespace(self, prefix, uri);
  return ST_CONTINUE;
}


/*
 * call-seq:
 *    context.register_namespaces(["prefix:uri"]) -> self
 * 
 * Register the specified namespaces in this context.
 *
 *   context.register_namespaces('xi:http://www.w3.org/2001/XInclude')
 *   context.register_namespaces(['xlink:http://www.w3.org/1999/xlink',
 *                                'xi:http://www.w3.org/2001/XInclude')
 *   context.register_namespaces('xlink' => 'http://www.w3.org/1999/xlink',
 *                                  'xi' => 'http://www.w3.org/2001/XInclude')
 */
VALUE
ruby_xml_xpath_context_register_namespaces(VALUE self, VALUE nslist) {
  char *cp;
  long i;
  VALUE rprefix, ruri;

  /* Need to loop through the 2nd argument and iterate through the
   * list of namespaces that we want to allow */
  switch (TYPE(nslist)) {
  case T_STRING:
    cp = strchr(StringValuePtr(nslist), (int)':');
    if (cp == NULL) {
      rprefix = nslist;
      ruri = Qnil;
    } else {
      rprefix = rb_str_new(StringValuePtr(nslist), (int)((long)cp - (long)StringValuePtr(nslist)));
      ruri = rb_str_new2(&cp[1]);
    }
    /* Should test the results of this */
    ruby_xml_xpath_context_register_namespace(self, rprefix, ruri);
    break;
  case T_ARRAY:
    for (i = 0; i < RARRAY(nslist)->len; i++) {
      ruby_xml_xpath_context_register_namespaces(self, RARRAY(nslist)->ptr[i]);
    }
    break;
  case T_HASH:
    st_foreach(RHASH(nslist)->tbl, iterate_ns_hash, self);
    break;
  default:
    rb_raise(rb_eArgError, "Invalid argument type, only accept string, array of strings, or an array of arrays");
  }
  return self;
}

/*
 * call-seq:
 *    context.node = node
 * 
 * Set the current node used by the XPath engine
 
 *  doc = LibXML::Document.string('<header><first>hi</first></header>')
 *  context.node = doc.root.first
 */
VALUE
ruby_xml_xpath_context_node_set(VALUE self, VALUE node) {
  xmlXPathContextPtr xctxt;
  xmlNodePtr xnode;
  
  Data_Get_Struct(self, xmlXPathContext, xctxt);
  Data_Get_Struct(node, xmlNode, xnode);
  xctxt->node = xnode;
  return node;
}

/*
 * call-seq:
 *    context.find("xpath") -> LibXML::XPath::Object
 * 
 * Find nodes matching the specified XPath expression
 */
VALUE
ruby_xml_xpath_context_find(VALUE self, VALUE xpath_expr) {
  xmlXPathContextPtr xctxt;
  xmlXPathObjectPtr xobject;
  VALUE result;

  Data_Get_Struct(self, xmlXPathContext, xctxt);
  xobject = xmlXPathEval((xmlChar*)StringValuePtr(xpath_expr), xctxt);
  
  if (xobject == NULL)
    rb_raise(eXMLXPathInvalidPath,
	     "Invalid XPath expression (expr could not be evaluated)");

  result = ruby_xml_xpath_object_wrap(xobject);
  rb_iv_set(result, "@context", self);
  return result;  
}


// Rdoc needs to know 
#ifdef RDOC_NEVER_DEFINED
  mLibXML = rb_define_module("LibXML");
  mXPath = rb_define_module_under(mLibXML, "XPath");
#endif

void
ruby_init_xml_xpath_context(void) {
  cXMLXPathContext = rb_define_class_under(mXPath, "Context", rb_cObject);
  rb_define_alloc_func(cXMLXPathContext, ruby_xml_xpath_context_alloc);
  rb_define_method(cXMLXPathContext, "initialize", ruby_xml_xpath_context_initialize, 1);
  rb_define_method(cXMLXPathContext, "register_namespaces", ruby_xml_xpath_context_register_namespaces, 1);
  rb_define_method(cXMLXPathContext, "register_namespaces_from_node", ruby_xml_xpath_context_register_namespaces_from_node, 1);
  rb_define_method(cXMLXPathContext, "register_namespace", ruby_xml_xpath_context_register_namespace, 2);
  rb_define_method(cXMLXPathContext, "node=", ruby_xml_xpath_context_node_set, 1);
  rb_define_method(cXMLXPathContext, "find", ruby_xml_xpath_context_find, 1);
  rb_define_attr(cXMLXPathContext, "doc", 1, 0);
}
