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
  int i;
  ruby_xml_node_set *rxnset;
  VALUE nodeobj, set_ary;

  Data_Get_Struct(self, ruby_xml_node_set, rxnset);

  set_ary = rb_ary_new();
  if (!((rxnset->node_set == NULL) || (rxnset->node_set->nodeNr == 0))) {
    for (i = 0; i < rxnset->node_set->nodeNr; i++) {
      nodeobj = ruby_xml_node_new_ptr(cXMLNode, rxnset->xd, rxnset->node_set->nodeTab[i]);
      rb_ary_push(set_ary, nodeobj);
    }
  }
  
  return(set_ary);
}


/*
 * call-seq:
 *    nodeset.each { |node| ... } => self
 * 
 * Call the supplied block for each node in this set.
 */
VALUE
ruby_xml_node_set_each(VALUE self) {
  int i;
  ruby_xml_node_set *rxnset;
  VALUE nodeobj;

  Data_Get_Struct(self, ruby_xml_node_set, rxnset);

  if (rxnset->node_set == NULL)
    return(Qnil);

  for (i = 0; i < rxnset->node_set->nodeNr; i++) {
    switch(rxnset->node_set->nodeTab[i]->type) {
    case XML_ATTRIBUTE_NODE:
      nodeobj = ruby_xml_attr_new2(cXMLAttr, rxnset->xd, (xmlAttrPtr)rxnset->node_set->nodeTab[i]);
      break;
    default:
      nodeobj = ruby_xml_node_new_ptr(cXMLNode, rxnset->xd, rxnset->node_set->nodeTab[i]);
    }

    rb_yield(nodeobj);
  }
  return(self);
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
  return ( rxnset->node_set == NULL || rxnset->node_set->nodeNr <= 0 ) ? Qtrue : Qfalse;
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
  VALUE nodeobj;

  Data_Get_Struct(self, ruby_xml_node_set, rxnset);
  
  if (rxnset->node_set == NULL || rxnset->node_set->nodeNr < 1)
    return(Qnil);
    
  switch(rxnset->node_set->nodeTab[0]->type) {
    case XML_ATTRIBUTE_NODE:
      nodeobj = ruby_xml_attr_new2(cXMLAttr, rxnset->xd, (xmlAttrPtr)rxnset->node_set->nodeTab[0]);
      break;
    default:
      nodeobj = ruby_xml_node_new_ptr(cXMLNode, rxnset->xd, rxnset->node_set->nodeTab[0]);
  }

  return(nodeobj);
}


void
ruby_xml_node_set_free(ruby_xml_node_set *rxnset) {
  void *data;

  switch(rxnset->data_type) {
  case RUBY_LIBXML_SRC_TYPE_NULL:
    break;
  case RUBY_LIBXML_SRC_TYPE_XPATH:
    data = (void*)(rx_xpath_data *)rxnset->data;
    free((rx_xpath_data *)data);
  default:
    rb_fatal("Unknown data type, %d", rxnset->data_type);
  }

  /* Don't need to free the node set because the nodeset is a child of
     the XPath object that created the set.
    if (rxnset->node_set != NULL)
      xmlXPathFreeNodeSet(rxnset->node_set);
  */

  free(rxnset);
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
  if (rxnset->node_set == NULL)
    return(INT2FIX(0));
  else
    return(INT2NUM(rxnset->node_set->nodeNr));
}


static void
ruby_xml_node_set_mark(ruby_xml_node_set *rxnset) {
  if (rxnset == NULL) return;
  if (!NIL_P(rxnset->xd)) rb_gc_mark(rxnset->xd);
  if (!NIL_P(rxnset->xpath)) rb_gc_mark(rxnset->xpath);
}


VALUE
ruby_xml_node_set_new(VALUE class, VALUE xd, VALUE xpath,
			    xmlNodeSetPtr node_set) {
  ruby_xml_node_set *rxnset;

  rxnset = ALLOC(ruby_xml_node_set);
  rxnset->node_set = node_set;
  rxnset->data = NULL;
  rxnset->data_type = RUBY_LIBXML_SRC_TYPE_NULL;
  rxnset->xd = xd;
  rxnset->xpath = xpath;
  return(Data_Wrap_Struct(class, ruby_xml_node_set_mark,
			  ruby_xml_node_set_free, rxnset));
}


VALUE
ruby_xml_node_set_new2(VALUE xd, VALUE xpath,
			     xmlNodeSetPtr node_set) {
  return(ruby_xml_node_set_new(cXMLNodeSet, xd, xpath, node_set));
}


/*
 * call-seq:
 *    nodeset.xpath => xpath
 * 
 * Obtain the xpath corresponding to this nodeset, if any.
 */
VALUE
ruby_xml_node_set_xpath_get(VALUE self) {
  ruby_xml_node_set *rxnset;

  Data_Get_Struct(self, ruby_xml_node_set, rxnset);
  if (NIL_P(rxnset->xpath))
    return(Qnil);
  else
    return(rxnset->xpath);
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
  ruby_xml_node_set *rxnset;
  rx_xpath_data *rxxpd;

  Data_Get_Struct(self, ruby_xml_node_set, rxnset);
  if (rxnset->data_type != RUBY_LIBXML_SRC_TYPE_XPATH)
    return(Qnil);

  rxxpd = (rx_xpath_data *)rxnset->data;
  return(rxxpd->ctxt);
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
