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

// TODO Maybe we should support [] or some other kind of access if poss.

/*
 * call-seq:
 *    xpath.each { |node| ... } => self
 * 
 * Call the supplied block for each matching node.
 */
VALUE
ruby_xml_xpath_each(VALUE self) {
  ruby_xml_xpath *rxxp;
  VALUE rxnset;

  Data_Get_Struct(self, ruby_xml_xpath, rxxp);

  if (rxxp->xpop == NULL || rxxp->xpop->type != XPATH_NODESET)
    return(Qnil);

  rxnset = ruby_xml_node_set_new(cXMLNodeSet, rxxp->xd, self,
				 rxxp->xpop->nodesetval);
  ruby_xml_node_set_each(rxnset);
  return(rxnset);
}

///////////////////////////////////////////////////
// TODO xpath_find is throwing TypeError:
//
//    TypeError: can't convert nil into String
//
// When given a namespace when non exist.

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
ruby_xml_xpath_find(int argc, VALUE *argv, VALUE class) {
#ifdef LIBXML_XPATH_ENABLED
  xmlXPathCompExprPtr comp;
  ruby_xml_node *node;
  ruby_xml_xpath *rxxp;
  ruby_xml_xpath_context *rxxpc;
  ruby_xml_ns *rxns;
  VALUE rnode, rprefix, ruri, xxpc, xpath, xpath_expr;
  char *cp;
  long i;

  switch(argc) {
  case 3:
    /* array of namespaces we allow.
     *
     * Accept either:
     *   A string in the form of: "prefix:uri", or
     *   An array of:
     *     *) strings in the form like above
     *     *) arrays in the form of ['prefix','uri']
     */

    /* Intentionally fall through, we deal with the last arg below
     * after the XPathContext object has been setup */
  case 2:
    rnode = argv[0];
    xpath_expr = argv[1];
    break;
  default:
    rb_raise(rb_eArgError, "wrong number of arguments (1 or 2)");
  }

  Data_Get_Struct(rnode, ruby_xml_node, node);

  xxpc = ruby_xml_xpath_context_new4(rnode);
  if (NIL_P(xxpc))
    return(Qnil);
  Data_Get_Struct(xxpc, ruby_xml_xpath_context, rxxpc);

  xpath = ruby_xml_xpath_new(cXMLXPath, rnode, xxpc, NULL);
  Data_Get_Struct(xpath, ruby_xml_xpath, rxxp);

  rxxpc->ctxt->node = node->node;
  if (node->node->type == XML_DOCUMENT_NODE) {
    rxxpc->ctxt->namespaces = xmlGetNsList(node->node->doc,
					   xmlDocGetRootElement(node->node->doc));
  } else {
    rxxpc->ctxt->namespaces = xmlGetNsList(node->node->doc, node->node);
  }

  rxxpc->ctxt->nsNr = 0;
  if (rxxpc->ctxt->namespaces != NULL) {
    while (rxxpc->ctxt->namespaces[rxxpc->ctxt->nsNr] != NULL)
      rxxpc->ctxt->nsNr++;
  }

  /* Need to loop through the 2nd argument and iterate through the
   * list of namespaces that we want to allow */
  if (argc == 3) {
    switch (TYPE(argv[2])) {
    case T_STRING:
      cp = strchr(StringValuePtr(argv[2]), (int)':');
      if (cp == NULL) {
	rprefix = argv[2];
	ruri = Qnil;
      } else {
	rprefix = rb_str_new(StringValuePtr(argv[2]), (int)((long)cp - (long)StringValuePtr(argv[2])));
	ruri = rb_str_new2(&cp[1]);
      }
      /* Should test the results of this */
      ruby_xml_xpath_context_register_namespace(xxpc, rprefix, ruri);
      break;
    case T_ARRAY:
      for (i = 0; i < RARRAY(argv[2])->len; i++) {
	switch (TYPE(RARRAY(argv[2])->ptr[i])) {
	case T_STRING:
	  cp = strchr(StringValuePtr(RARRAY(argv[2])->ptr[i]), (int)':');
	  if (cp == NULL) {
	    rprefix = RARRAY(argv[2])->ptr[i];
	    ruri = Qnil;
	  } else {
	    rprefix = rb_str_new(StringValuePtr(RARRAY(argv[2])->ptr[i]), (int)((long)cp - (long)StringValuePtr(RARRAY(argv[2])->ptr[i])));
	    ruri = rb_str_new2(&cp[1]);
	  }
	  /* Should test the results of this */
	  ruby_xml_xpath_context_register_namespace(xxpc, rprefix, ruri);
	  break;
	case T_ARRAY:
	  if (RARRAY(RARRAY(argv[2])->ptr[i])->len == 2) {
	    rprefix = RARRAY(RARRAY(argv[2])->ptr[i])->ptr[0];
	    ruri = RARRAY(RARRAY(argv[2])->ptr[i])->ptr[1];
	    ruby_xml_xpath_context_register_namespace(xxpc, rprefix, ruri);
	  } else {
	    rb_raise(rb_eArgError, "nested array must be an array of strings, prefix and href/uri");
	  }
	  break;
	default:
	  if (rb_obj_is_kind_of(RARRAY(argv[2])->ptr[i], cXMLNS) == Qtrue) {
	    Data_Get_Struct(argv[2], ruby_xml_ns, rxns);
	    rprefix = rb_str_new2((const char*)rxns->ns->prefix);
	    ruri = rb_str_new2((const char*)rxns->ns->href);
	    ruby_xml_xpath_context_register_namespace(xxpc, rprefix, ruri);
	  } else
	    rb_raise(rb_eArgError, "Invalid argument type, only accept string, array of strings, or an array of arrays");
	}
      }
      break;
    default:
      if (rb_obj_is_kind_of(argv[2], cXMLNS) == Qtrue) {
	Data_Get_Struct(argv[2], ruby_xml_ns, rxns);
	rprefix = rb_str_new2((const char*)rxns->ns->prefix);
	ruri = rb_str_new2((const char*)rxns->ns->href);
	ruby_xml_xpath_context_register_namespace(xxpc, rprefix, ruri);
      } else
	rb_raise(rb_eArgError, "Invalid argument type, only accept string, array of strings, or an array of arrays");
    }
  }
  comp = xmlXPathCompile((xmlChar*)StringValuePtr(xpath_expr));

  if (comp == NULL) {
    xmlXPathFreeCompExpr(comp);
    rb_raise(eXMLXPathInvalidPath, "Invalid XPath expression");
  }
  rxxp->xpop = xmlXPathCompiledEval(comp, rxxpc->ctxt);
  xmlXPathFreeCompExpr(comp);

  if (rxxpc->ctxt->namespaces != NULL)
    xmlFree(rxxpc->ctxt->namespaces);

  if (rxxp->xpop == NULL)
    rb_raise(eXMLXPathInvalidPath,
	     "Invalid XPath expression for this document");

  if (rxxp->xpop->type != XPATH_NODESET)
    return(Qnil);

  return(ruby_xml_node_set_new2(ruby_xml_document_wrap(cXMLDocument,node->node->doc),
				xpath,
				rxxp->xpop->nodesetval));
#else
  rb_warn("libxml was compiled without XPath support");
  return(Qfalse);
#endif
}


VALUE
ruby_xml_xpath_find2(int argc, VALUE *argv) {
  return(ruby_xml_xpath_find(argc, argv, cXMLXPath));
}


void
ruby_xml_xpath_free(ruby_xml_xpath *rxxp) {
  if (rxxp->xpop != NULL) {
    xmlXPathFreeObject(rxxp->xpop);
    rxxp->xpop = NULL;
  }

  free(rxxp);
}


void
ruby_xml_xpath_mark(ruby_xml_xpath *rxxp) {
  if (rxxp == NULL) return;
  if (!NIL_P(rxxp->ctxt)) rb_gc_mark(rxxp->ctxt);
  if (!NIL_P(rxxp->xd)) rb_gc_mark(rxxp->xd);
}


VALUE
ruby_xml_xpath_new(VALUE class, VALUE xd, VALUE ctxt,
			 xmlXPathObjectPtr xpop) {
  ruby_xml_xpath *rxxp;

  rxxp = ALLOC(ruby_xml_xpath);
  rxxp->ctxt = ctxt;
  rxxp->xd = xd;
  rxxp->xpop = xpop;
  return(Data_Wrap_Struct(class, ruby_xml_xpath_mark,
			  ruby_xml_xpath_free, rxxp));
}


/*
 * call-seq:
 *    xpath.set => nodeset
 * 
 * Obtain an XML::Node::Set with nodes matching this xpath.
 */
VALUE
ruby_xml_xpath_set(VALUE self) {
  ruby_xml_xpath *rxxp;
  Data_Get_Struct(self, ruby_xml_xpath, rxxp);

  if (rxxp->xpop == NULL || rxxp->xpop->type != XPATH_NODESET)
    return(Qnil);

  return(ruby_xml_node_set_new(cXMLNodeSet, rxxp->xd, self,
			       rxxp->xpop->nodesetval));
}


/*
 * call-seq:
 *    xpath.set_type => num
 * 
 * Obtains the type identifier of this xpath
 * set.
 */
VALUE
ruby_xml_xpath_set_type(VALUE self) {
  ruby_xml_xpath *rxxp;
  Data_Get_Struct(self, ruby_xml_xpath, rxxp);

  return(INT2FIX(rxxp->xpop->type));
}

// TODO maybe 'string' should alias as 'to_s'?

/*
 * call-seq:
 *    xpath.string => "xpath" 
 * 
 * Obtain a string representation of this xpath.
 */
VALUE
ruby_xml_xpath_string(VALUE self) {
  ruby_xml_xpath *rxxp;
  Data_Get_Struct(self, ruby_xml_xpath, rxxp);

  if (rxxp->xpop->stringval == NULL)
    return(Qnil);
  else
    return(rb_str_new2((const char*)rxxp->xpop->stringval));
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

  rb_define_singleton_method(cXMLXPath, "find", ruby_xml_xpath_find, 2);

  rb_define_method(cXMLXPath, "debug", ruby_xml_xpath_debug, 0);
  rb_define_method(cXMLXPath, "each", ruby_xml_xpath_each, 0);
  rb_define_method(cXMLXPath, "set", ruby_xml_xpath_set, 0);
  rb_define_method(cXMLXPath, "set_type", ruby_xml_xpath_set_type, 0);
  rb_define_method(cXMLXPath, "string", ruby_xml_xpath_string, 0);
}

#endif /* ifdef LIBXML_XPATH_ENABLED */
