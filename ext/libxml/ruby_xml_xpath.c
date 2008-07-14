/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

#include "ruby_libxml.h"
#include "ruby_xml_xpath.h"
#include "ruby_xml_xpath_context.h"

/*
 * Document-class: XML::XPath
 * 
 * The XML::XPath class is used to query XML documents. It is used
 * via the XML::Document#find method.
 *
 *  document.find('/foo', namespaces) -> XML::XPath::Object
 *
 * The optional namespaces parameter can be a string, array or
 * hash table.
 *
 *   document.find('/foo', 'xlink:http://www.w3.org/1999/xlink')
 *   document.find('/foo', ['xlink:http://www.w3.org/1999/xlink',
 *                          'xi:http://www.w3.org/2001/XInclude')
 *   document.find('/foo', 'xlink' => 'http://www.w3.org/1999/xlink',
 *                             'xi' => 'http://www.w3.org/2001/XInclude')
 *
 *
 * === Working With Namespaces
 *
 * Finding namespaced elements and attributes can be tricky.  
 * Lets work through some examples using the following xml document:
 *
 *  <soap:Envelope xmlns:soap="http://schemas.xmlsoap.org/soap/envelope/">
 *    <soap:Body>
 *      <getManufacturerNamesResponse xmlns="http://services.somewhere.com">
 *        <IDAndNameList xmlns="http://services.somewhere.com">
 *          <ns1:IdAndName xmlns:ns1="http://domain.somewhere.com"/>
 *        </IDAndNameList>
 *      </getManufacturerNamesResponse>
 *  </soap:Envelope>
 *
 *  # Since the soap namespace is defined on the root 
 *  # node we can directly use it.
 *  doc.find('/soap:Envelope')
 *
 *  # Since the ns1 namespace is not defined on the root node
 *  # we have to first register it with the xpath engine.
 *  doc.find('//ns1:IdAndName', 
 *           'ns1:http://domain.somewhere.com')
 *             
 *  # Since the getManufacturerNamesResponse element uses a default 
 *  # namespace we first have to give it a prefix and register
 *  # it with the xpath engine.
 *  doc.find('//ns:getManufacturerNamesResponse', 
 *            'ns:http://services.somewhere.com')
 *
 *  # Here is an example showing a complex namespace aware
 *  # xpath expression.
 *  doc.find('/soap:Envelope/soap:Body/ns0:getManufacturerNamesResponse/ns0:IDAndNameList/ns1:IdAndName', 
             ['ns0:http://services.somewhere.com', 'ns1:http://domain.somewhere.com'])
*/

VALUE mXMLXPath;
VALUE eXMLXPathInvalidPath;

#ifdef LIBXML_XPATH_ENABLED


/*
 * call-seq:
 *    xpath.debug -> (true|false)
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

/*
 * call-seq:
 *    XML::XPath.find(node, "xpath", namespaces = [any]) -> XML::XPath::Object
 * 
 * Return nodes matching the specified xpath as a XML::XPath::Object.
 * 
 * Parameters:
 * node - A document or node.  IThe xpath expression will be evaluated in 
 *        the context of this node (the root node for documents).
 *        node as the current node.
 * xpath - The xpath expression as a string
 * namespaces - An optional list of namespaces (see XML::XPath for information).
 */
VALUE
ruby_xml_xpath_find(VALUE class, VALUE node, VALUE xpath_expr, VALUE nslist) {
#ifdef LIBXML_XPATH_ENABLED
  xmlNodePtr xnode;
  xmlXPathContextPtr xctxt;
  VALUE document;
  VALUE context;
  VALUE argv[1];

  if (rb_obj_is_kind_of(node, cXMLDocument) == Qtrue)
  {
    ruby_xml_document_t *rdoc;
    Data_Get_Struct(node, ruby_xml_document_t, rdoc);
    document = node;
    xnode = xmlDocGetRootElement(rdoc->doc);
  }
  else if (rb_obj_is_kind_of(node, cXMLNode) == Qtrue)
  {
    document = rb_funcall(node, rb_intern("doc"), 0);
    Data_Get_Struct(node, xmlNode, xnode);
  }
  else  
  {
    rb_raise(rb_eTypeError, "The first argument must be a document or node.");
  }
  
  /* Create a new XPath context and set the current node */
  argv[0] = document;
  context = rb_class_new_instance(1, argv, cXMLXPathContext);
  Data_Get_Struct(context, xmlXPathContext, xctxt);
  xctxt->node = xnode;
  
  /* Setup context namespaces from the node to make the user's life
     a bit easier. */
  ruby_xml_xpath_context_register_namespaces_from_node(context, node);
  
  /* Register extra, user specified namespaces. */
  if (!NIL_P(nslist))
    ruby_xml_xpath_context_register_namespaces(context, nslist);

  // And now return the result
  return ruby_xml_xpath_context_find(context, xpath_expr);
#else
  rb_warn("libxml was compiled without XPath support");
  return(Qfalse);
#endif
}


VALUE
ruby_xml_xpath_findva(int argc, VALUE *argv, VALUE class) {
  VALUE document;
  VALUE xpath_expr;
  VALUE nslist;
  
  if ( argc < 2 || argc > 3 )
    rb_raise(rb_eArgError, "wrong number of arguments (2 or 3)");

  document = argv[0];
  xpath_expr = argv[1];
  nslist = (argc == 3) ? argv[2] :Qnil;
  
  return ruby_xml_xpath_find(class, document, xpath_expr, nslist);
}

// Rdoc needs to know 
#ifdef RDOC_NEVER_DEFINED
  mXML = rb_define_module("XML");
#endif

void
ruby_init_xml_xpath(void) {
  mXMLXPath = rb_define_module_under(mXML, "XPath");
  eXMLXPathInvalidPath = rb_define_class_under(mXMLXPath, "InvalidPath", eXMLError);

  rb_define_const(mXMLXPath, "UNDEFINED", INT2NUM(XPATH_UNDEFINED));
  rb_define_const(mXMLXPath, "NODESET", INT2NUM(XPATH_NODESET));
  rb_define_const(mXMLXPath, "BOOLEAN", INT2NUM(XPATH_BOOLEAN));
  rb_define_const(mXMLXPath, "NUMBER", INT2NUM(XPATH_NUMBER));
  rb_define_const(mXMLXPath, "STRING", INT2NUM(XPATH_STRING));
  rb_define_const(mXMLXPath, "POINT", INT2NUM(XPATH_POINT));
  rb_define_const(mXMLXPath, "RANGE", INT2NUM(XPATH_RANGE));
  rb_define_const(mXMLXPath, "LOCATIONSET", INT2NUM(XPATH_LOCATIONSET));
  rb_define_const(mXMLXPath, "USERS", INT2NUM(XPATH_USERS));
  rb_define_const(mXMLXPath, "XSLT_TREE", INT2NUM(XPATH_XSLT_TREE));

  rb_define_singleton_method(mXMLXPath, "find", ruby_xml_xpath_findva, -1);

//  rb_define_method(mXMLXPath, "debug", ruby_xml_xpath_debug, 0);
  ruby_init_xml_xpath_object();
}

#endif /* ifdef LIBXML_XPATH_ENABLED */
