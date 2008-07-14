/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

#include "ruby_libxml.h"
#include "ruby_xml_xpath.h"
#include "ruby_xml_xpath_context.h"

/*
 * Document-class: LibXML::XPath
 * 
 * The LibXML::XPath class is used to query XML documents. It is used
 * via the LibXML::Document#find method.
 *
 *  document.find('/foo', namespaces) -> LibXML::XPath::Object
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

VALUE mXPath;
VALUE eXMLXPathInvalidPath;

// Rdoc needs to know 
#ifdef RDOC_NEVER_DEFINED
  mLibXML = rb_define_module("LibXML");
#endif

void
ruby_init_xml_xpath(void) {
  mXPath = rb_define_module_under(mLibXML, "XPath");
  eXMLXPathInvalidPath = rb_define_class_under(mXPath, "InvalidPath", eXMLError);

  rb_define_const(mXPath, "UNDEFINED", INT2NUM(XPATH_UNDEFINED));
  rb_define_const(mXPath, "NODESET", INT2NUM(XPATH_NODESET));
  rb_define_const(mXPath, "BOOLEAN", INT2NUM(XPATH_BOOLEAN));
  rb_define_const(mXPath, "NUMBER", INT2NUM(XPATH_NUMBER));
  rb_define_const(mXPath, "STRING", INT2NUM(XPATH_STRING));
  rb_define_const(mXPath, "POINT", INT2NUM(XPATH_POINT));
  rb_define_const(mXPath, "RANGE", INT2NUM(XPATH_RANGE));
  rb_define_const(mXPath, "LOCATIONSET", INT2NUM(XPATH_LOCATIONSET));
  rb_define_const(mXPath, "USERS", INT2NUM(XPATH_USERS));
  rb_define_const(mXPath, "XSLT_TREE", INT2NUM(XPATH_XSLT_TREE));

  ruby_init_xml_xpath_object();
}
