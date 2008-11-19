/* $Id: ruby_xml_xpath.c 461 2008-07-15 21:35:56Z cfis $ */

/* Please see the LICENSE file for copyright and distribution information */

#include "ruby_libxml.h"
#include "ruby_xml_xpath.h"
#include "ruby_xml_xpath_expression.h"

/*
 * Document-class: LibXML::XML::XPath::Expression
 * 
 * The XML::XPath::Expression class is used to compile  
 * XPath expressions so they can be parsed only once
 * but reused multiple times.
 *
 *  doc = XML::Document.string(IO.read('some xml file'))
 *  expr = XPath::Expression.new('//first')
 *  doc.root.each do |node|
 *   result = node.find(expr) # many, many, many times
 *   # ...
 *  end
 */

VALUE cXMLXPathExpression;

void
ruby_xml_xpath_expression_free(xmlXPathCompExprPtr expr) {
  xmlXPathFreeCompExpr(expr);
}

VALUE
ruby_xml_xpath_expression_alloc(VALUE klass) {
  return Data_Wrap_Struct(cXMLXPathExpression,
						  NULL,
						  ruby_xml_xpath_expression_free,
						  NULL);
}

/* call-seq:
 *    XPath::Expression.new(expression) -> XPath::Expression
 * 
 * Compiled XPatch expression. Work faster when find called many times 
 * same expression.
 *
 *  doc = XML::Document.string('<header><first>hi</first></header>')
 *  expr = XPath::Expression.new('//first')
 *  nodes = doc.find(expr)
 */
VALUE
ruby_xml_xpath_expression_initialize(VALUE self, VALUE expression) {
  xmlXPathCompExprPtr compexpr = xmlXPathCompile(StringValueCStr(expression));  

  if (compexpr == NULL) {
	  xmlErrorPtr xerror = xmlGetLastError();
	  ruby_xml_raise(xerror);
  }

  DATA_PTR(self) = compexpr;
  return self;
}

void
ruby_init_xml_xpath_expression(void) {
  cXMLXPathExpression = rb_define_class_under(mXPath, "Expression", rb_cObject);
  rb_define_alloc_func(cXMLXPathExpression, ruby_xml_xpath_expression_alloc);
  rb_define_singleton_method(rb_cRegexp, "compile", rb_class_new_instance, 1);

  rb_define_method(cXMLXPathExpression, "initialize", ruby_xml_xpath_expression_initialize, 1);
}
