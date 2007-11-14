/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

#include "libxml.h"
#include "ruby_xml_xpath.h"

/*
 * Document-class: XML::XPath
 * 
 * Includes Enumerable.
 */
VALUE cXMLXPath;
VALUE eXMLXPathInvalidPath;

#ifdef LIBXML_XPATH_ENABLED

/*
 * call-seq:
 *    xpath.debug => (true|false)
 * 
 * Dump libxml debugging information to stdout.
 * Requires Libxml be compiled with debugging enabled.
 */
VALUE
ruby_xml_xpath_debug(VALUE self) {
#ifdef LIBXML_DEBUG_ENABLED
  ruby_xml_xpath *rxxp;
  Data_Get_Struct(self, ruby_xml_xpath, rxxp);

  if (rxxp->xpop != NULL) {
    xmlXPathDebugDumpObject(stdout, rxxp->xpop, 0);
    return(Qtrue);
  } else {
    return(Qfalse);
  }
#else
  rb_warn("libxml does not have debugging turned on");
  return(Qfalse);
#endif
}

///////////////////////////////////////////////////
// TODO xpath_find is throwing TypeError:
//
//    TypeError: can't convert nil into String
//
// When given a namespace when non exist.
void
ruby_xml_xpath_register_namespaces(VALUE nslist, VALUE xxpc, int level) {
  char *cp;
  long i;
  VALUE rprefix, ruri;
  ruby_xml_ns *rxns;

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
    ruby_xml_xpath_context_register_namespace(xxpc, rprefix, ruri);
    break;
  case T_ARRAY:
    if ( level == 0 ) {
      for (i = 0; i < RARRAY(nslist)->len; i++) {
	ruby_xml_xpath_register_namespaces(RARRAY(nslist)->ptr[i],xxpc,1);
      }
    }
    else {
      // tuples of prefix/uri
      if (RARRAY(RARRAY(nslist)->ptr[i])->len == 2) {
	rprefix = RARRAY(RARRAY(nslist)->ptr[i])->ptr[0];
	ruri = RARRAY(RARRAY(nslist)->ptr[i])->ptr[1];
	ruby_xml_xpath_context_register_namespace(xxpc, rprefix, ruri);
      } else {
	rb_raise(rb_eArgError, "nested array must be an array of strings, prefix and href/uri");
      }
    }
    break;
  default:
    if (rb_obj_is_kind_of(nslist, cXMLNS) == Qtrue) {
      Data_Get_Struct(nslist, ruby_xml_ns, rxns);
      rprefix = rb_str_new2((const char*)rxns->ns->prefix);
      ruri = rb_str_new2((const char*)rxns->ns->href);
      ruby_xml_xpath_context_register_namespace(xxpc, rprefix, ruri);
    } else
      rb_raise(rb_eArgError, "Invalid argument type, only accept string, array of strings, or an array of arrays");
  }
}
/*
 * call-seq:
 *    XML::XPath.find(path, namespaces = [any]) => xpath
 * 
 * Find nodes matching the specified xpath (and optionally any of the
 * supplied namespaces) and return as an XML::Node::Set.
 * 
 * The optional namespaces argument may take one of
 * two forms:
 * 
 * * A string in the form of: "prefix:uri", or
 * * An array of:
 *   * strings in the form like above
 *   * arrays in the form of ['prefix','uri']
 * 
 * If not specified, matching nodes from any namespace
 * will be included.
 */
VALUE
ruby_xml_xpath_find(VALUE class, VALUE anode, VALUE xpath_expr, VALUE nslist) {
#ifdef LIBXML_XPATH_ENABLED
  xmlXPathCompExprPtr comp;
  xmlXPathObjectPtr xxpop;
  ruby_xml_node *node;
  ruby_xml_xpath *rxxp;
  xmlXPathContextPtr ctxt;
  ruby_xml_document_t *rdocp;
  VALUE rnode, xxpc;
  VALUE rxpop;

  if (rb_obj_is_kind_of(anode, cXMLDocument) == Qtrue) {
    xxpc = ruby_xml_xpath_context_new(anode);
    Data_Get_Struct(anode, ruby_xml_document_t, rdocp);
#ifdef DEBUG
    fprintf(stderr,"rdocp=0x%x root=0x%x\n",rdocp,xmlDocGetRootElement(rdocp->doc));
#endif
    rnode=ruby_xml_node2_wrap(cXMLNode,xmlDocGetRootElement(rdocp->doc));
#ifdef DEBUG
    fprintf(stderr,"rnode 0x%x 0x%x\n",rnode,xmlDocGetRootElement(rdocp->doc)->_private);
#endif
    Data_Get_Struct(rnode, ruby_xml_node, node);
  } else if ( rb_obj_is_kind_of(anode, cXMLNode) == Qtrue) {
    xxpc = ruby_xml_xpath_context_new(anode);
    Data_Get_Struct(anode, ruby_xml_node, node);
  } else
    rb_raise(rb_eTypeError, "arg 1 must be XML::Document or XML::Node within a document %s", rb_obj_as_string(anode));
  
  if (NIL_P(xxpc))
    return(Qnil);

  Data_Get_Struct(xxpc,xmlXPathContext,ctxt);
  // XXX Is this legal? Set a subtree to apply xpath?
  ctxt->node = node->node;

  // XXX is setting ->namespaces used?
  if (node->node->type == XML_DOCUMENT_NODE) {
    ctxt->namespaces = xmlGetNsList(node->node->doc,
				    xmlDocGetRootElement(node->node->doc));
  } else {
    ctxt->namespaces = xmlGetNsList(node->node->doc, node->node);
  }

  ctxt->nsNr = 0;
  if (ctxt->namespaces != NULL) {
    while (ctxt->namespaces[ctxt->nsNr] != NULL)
      ctxt->nsNr++;
  }

  if ( ! NIL_P(nslist) )
    ruby_xml_xpath_register_namespaces(nslist,xxpc,0);

  comp = xmlXPathCompile((xmlChar*)StringValuePtr(xpath_expr));

  if (comp == NULL) {
    rb_raise(eXMLXPathInvalidPath,
	     "Invalid XPath expression (expr does not compile)");
  }
  xxpop=xmlXPathCompiledEval(comp, ctxt);
  if ( xxpop == NULL )
    return Qnil;

#define ALT
#ifdef ALT
  rxpop = ruby_xml_xpath_object_wrap(xxpop);
#else
  rxpop = Data_Wrap_Struct(cXMLXPathObject,
			   ruby_xml_xpath_object_mark,
			   ruby_xml_xpath_object_free,
			   xxpop);
#endif

#ifdef NODE_DEBUG
  fprintf(stderr,"xpo 0x%x class=%s\n",
	  rxpop,
	  rb_class2name(rb_obj_class(rxpop)));
#endif

  xmlXPathFreeCompExpr(comp);

  if (rxpop == Qnil)
    rb_raise(eXMLXPathInvalidPath,
	     "Invalid XPath expression for this document");

  return rxpop;
#else
  rb_warn("libxml was compiled without XPath support");
  return(Qfalse);
#endif
}


VALUE
ruby_xml_xpath_findva(int argc, VALUE *argv, VALUE class) {
  if ( argc < 2 || argc > 3 )
    rb_raise(rb_eArgError, "wrong number of arguments (1 or 2)");

  return ruby_xml_xpath_find(class,argv[0],argv[1],(argc==3)?argv[2]:Qnil);
}

VALUE
ruby_xml_xpath_find2(VALUE anode, VALUE xpath_expr, VALUE nslist) {
  return ruby_xml_xpath_find(cXMLXPath, anode, xpath_expr, nslist);
}

// Rdoc needs to know 
#ifdef RDOC_NEVER_DEFINED
  mXML = rb_define_module("XML");
#endif

void
ruby_init_xml_xpath(void) {
  cXMLXPath = rb_define_class_under(mXML, "XPath", rb_cObject);
  rb_include_module(cXMLNode, rb_const_get(rb_cObject, rb_intern("Enumerable")));

  eXMLXPathInvalidPath = rb_define_class_under(cXMLXPath, "InvalidPath", eXMLError);

  rb_define_const(cXMLXPath, "UNDEFINED", INT2NUM(XPATH_UNDEFINED));
  rb_define_const(cXMLXPath, "NODESET", INT2NUM(XPATH_NODESET));
  rb_define_const(cXMLXPath, "BOOLEAN", INT2NUM(XPATH_BOOLEAN));
  rb_define_const(cXMLXPath, "NUMBER", INT2NUM(XPATH_NUMBER));
  rb_define_const(cXMLXPath, "STRING", INT2NUM(XPATH_STRING));
  rb_define_const(cXMLXPath, "POINT", INT2NUM(XPATH_POINT));
  rb_define_const(cXMLXPath, "RANGE", INT2NUM(XPATH_RANGE));
  rb_define_const(cXMLXPath, "LOCATIONSET", INT2NUM(XPATH_LOCATIONSET));
  rb_define_const(cXMLXPath, "USERS", INT2NUM(XPATH_USERS));
  rb_define_const(cXMLXPath, "XSLT_TREE", INT2NUM(XPATH_XSLT_TREE));

  rb_define_singleton_method(cXMLXPath, "find", ruby_xml_xpath_findva, -1);

  rb_define_method(cXMLXPath, "debug", ruby_xml_xpath_debug, 0);

  ruby_init_xml_xpath_object();
}

#endif /* ifdef LIBXML_XPATH_ENABLED */
