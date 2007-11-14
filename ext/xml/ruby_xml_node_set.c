/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

#include "libxml.h"
#include "ruby_xml_node_set.h"

/*
 * Document-class: XML::Node::Set
 * 
 * Includes Enumerable.
 */
VALUE cXMLNodeSet;

// TODO maybe we should support [] on nodeset?
//      Would also give us on xpath too...

/*
 * call-seq:
 *    nodeset.to_a => [node, ..., node]
 * 
 * Obtain an array of the nodes in this set.
 */
VALUE
ruby_xml_node_set_to_a(VALUE self) {
  ruby_xml_node_set *rxnset;
  VALUE r;

  Data_Get_Struct(self, ruby_xml_node_set, rxnset);
  r=ruby_xml_xpath_object_to_a(rxnset->rxpop);
#ifdef NODE_DEBUG
  fprintf(stderr,"node_set_to_a %s\n",rb_str2cstr(rb_ary_to_s(r),0));
#endif
  return r;
}


/*
 * call-seq:
 *    nodeset.each { |node| ... } => self
 * 
 * Call the supplied block for each node in this set.
 */
VALUE
ruby_xml_node_set_each(VALUE self) {
  ruby_xml_node_set *rxnset;

  Data_Get_Struct(self, ruby_xml_node_set, rxnset);
  return ruby_xml_xpath_object_each(rxnset->rxpop);
}


/*
 * call-seq:
 *    nodeset.empty? => (true|false)
 *
 * Determine whether this nodeset is empty (contains no nodes).
 */
VALUE
ruby_xml_node_set_empty_q(VALUE self) {
  ruby_xml_node_set *rxnset;

  Data_Get_Struct(self, ruby_xml_node_set, rxnset);
  return ruby_xml_xpath_object_empty_q(rxnset->rxpop);
}


/*
 * call-seq:
 *    nodeset.first => node
 *
 * Returns the first node in this node set, or nil if none exist.
 */
VALUE
ruby_xml_node_set_first(VALUE self) {
  ruby_xml_node_set *rxnset;

  Data_Get_Struct(self, ruby_xml_node_set, rxnset);
  return ruby_xml_xpath_object_first(rxnset->rxpop);
}

/*
 * call-seq:
 *    nodeset.length => num
 * 
 * Obtain the length of this nodeset.
 */
VALUE
ruby_xml_node_set_length(VALUE self) {
  ruby_xml_node_set *rxnset;

  Data_Get_Struct(self, ruby_xml_node_set, rxnset);
  return ruby_xml_xpath_object_length(rxnset->rxpop);
}


static void
ruby_xml_node_set_mark(ruby_xml_node_set *rxnset) {
  if (!NIL_P(rxnset->rxpop)) rb_gc_mark(rxnset->rxpop);
}

static void
ruby_xml_node_set_free(ruby_xml_node_set *rxnset) {
  free(rxnset);
}

VALUE
ruby_xml_node_set_new(VALUE class, VALUE rxpop)
{
  ruby_xml_node_set *rxnset;
  rxnset = ALLOC(ruby_xml_node_set);
  rxnset->rxpop=rxpop;
  return Data_Wrap_Struct(class,
			  ruby_xml_node_set_mark,
			  ruby_xml_node_set_free,
			  rxnset);
}

VALUE
ruby_xml_node_set_new2(VALUE rxpop)
{
  return ruby_xml_node_set_new(cXMLNodeSet,rxpop);
}

/*
 * call-seq:
 *    nodeset.xpath => xpath
 * 
 * Obtain the xpath corresponding to this nodeset, if any.
 */
VALUE
ruby_xml_node_set_xpath_get(VALUE self) {
  rb_raise(rb_eRuntimeError,"xpath retrival is no longer supported");
}


/*
 * call-seq:
 *    nodeset.xpath_ctxt => context
 * 
 * Return the xpath context corresponding to this nodeset,
 * if any.
 */
VALUE
ruby_xml_node_set_xpath_data_get(VALUE self) {
  rb_raise(rb_eRuntimeError,"xpath data retrival is no longer supported");
}

// Rdoc needs to know 
#ifdef RDOC_NEVER_DEFINED
	mXML = rb_define_module("XML");
  cXMLNode = rb_define_class_under(mXML, "Node", rb_cObject);
#endif

void
ruby_init_xml_node_set(void) {
  cXMLNodeSet = rb_define_class_under(cXMLNode, "Set", rb_cObject);  
  rb_include_module(cXMLNodeSet, rb_const_get(rb_cObject, rb_intern("Enumerable")));

  rb_define_method(cXMLNodeSet, "each", ruby_xml_node_set_each, 0);
  rb_define_method(cXMLNodeSet, "empty?", ruby_xml_node_set_empty_q, 0);
  rb_define_method(cXMLNodeSet, "first", ruby_xml_node_set_first, 0);
  rb_define_method(cXMLNodeSet, "length", ruby_xml_node_set_length, 0);
  rb_define_method(cXMLNodeSet, "size", ruby_xml_node_set_length, 0);
  rb_define_method(cXMLNodeSet, "to_a", ruby_xml_node_set_to_a, 0);
  rb_define_method(cXMLNodeSet, "xpath", ruby_xml_node_set_xpath_get, 0);
  rb_define_method(cXMLNodeSet, "xpath_ctxt",
		   ruby_xml_node_set_xpath_data_get, 0);
       
}
