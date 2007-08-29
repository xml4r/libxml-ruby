/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

#include "libxml.h"
#include "ruby_xml_attr.h"

VALUE cXMLAttr;

void ruby_xml_attr_free(ruby_xml_attr_t *rx) {
  if (rx->attr == NULL ) return;

  if (rx->attr->parent == NULL && rx->attr->doc == NULL ) {
#ifdef NODE_DEBUG
    fprintf(stderr,"free rxn=0x%x xn=0x%x o=0x%x\n",(long)rxn,(long)rxn->node,(long)rxn->node->_private);
#endif
    rx->attr->_private=NULL;
    xmlFreeProp(rx->attr);
  }

  rx->attr=NULL;
  //  fprintf(stderr,"%0x ",(long)rxn);
  free(rx);
}

void
ruby_xml_attr_mark(ruby_xml_attr_t *rx) {
  xmlNodePtr node;
  if (rx->attr == NULL ) return;

  if (rx->attr->_private == NULL ) {
    rb_warning("XmlAttr is not bound! (%s:%d)",
	       __FILE__,__LINE__);
    return;
  }

  if (rx->attr->doc != NULL ) {
    if (rx->attr->doc->_private == NULL )
      rb_warning("XmlAttr Doc is not bound! (%s:%d)",
		 __FILE__,__LINE__);
    else {
      rb_gc_mark((VALUE)rx->attr->doc->_private);
#ifdef NODE_DEBUG
      fprintf(stderr,"mark rx=0x%x xn=0x%x o=0x%x\n",(long)rx,(long)rx->attr,(long)rx->attr->_private);
#endif
    }
  } else if (rx->attr->parent != NULL ) {
    if (rx->attr->parent->_private == NULL )
      rb_warning("XmlAttr Parent is not bound! (%s:%d)",
		 __FILE__,__LINE__);
    node=rx->attr->parent;
    while (node->parent != NULL )
      node=node->parent;
    if (node->_private != NULL) {
      rb_gc_mark((VALUE)node->_private);
#ifdef NODE_DEBUG
      fprintf(stderr,"mark rx=0x%x xn=0x%x o=0x%x\n",(long)0,(long)node,(long)node->_private);
#endif
    }
  }
}

VALUE
ruby_xml_attr_wrap(VALUE class, xmlAttrPtr xnode)
{
  VALUE obj;
  ruby_xml_attr_t *rx;

  // This node is already wrapped
  if (xnode->_private != NULL)
    return (VALUE)xnode->_private;

  obj=Data_Make_Struct(class,ruby_xml_attr_t,ruby_xml_attr_mark,
		       ruby_xml_attr_free,rx);

  rx->attr=xnode;
  xnode->_private=(void*)obj;
#ifdef NODE_DEBUG
  fprintf(stderr,"wrap rxn=0x%x xn=0x%x o=0x%x\n",(long)rxn,(long)xnode,(long)obj);
#endif
  return obj;
}

/*
 * Only use this when a xmlAttr has just been created since
 * oblitterates the _private. Not exposed to ruby interp.
 */
VALUE
ruby_xml_attr_new(VALUE class, xmlAttrPtr xnode)
{
  xnode->_private=NULL;
  return ruby_xml_attr_wrap(class,xnode);
}
/*
 * call-seq:
 *    attr.child => node
 * 
 * Obtain this attribute's child attribute(s).
 */
VALUE
ruby_xml_attr_child_get(VALUE self) {
  ruby_xml_attr_t *rxa;
  Data_Get_Struct(self, ruby_xml_attr_t, rxa);
  if (rxa->attr->children == NULL)
    return(Qnil);
  else
    return(ruby_xml_node2_wrap(cXMLNode, rxa->attr->children));
}


/*
 * call-seq:
 *    attr.child? => (true|false)
 * 
 * Determine whether this attribute has child attributes.
 */
VALUE
ruby_xml_attr_child_q(VALUE self) {
  ruby_xml_attr_t *rxa;
  Data_Get_Struct(self, ruby_xml_attr_t, rxa);
  if (rxa->attr->children == NULL)
    return(Qfalse);
  else
    return(Qtrue);
}


/*
 * call-seq:
 *    attr.doc => document
 * 
 * Obtain the XML::Document this attribute is associated with,
 * if any.
 */
VALUE
ruby_xml_attr_doc_get(VALUE self) {
  ruby_xml_attr_t *rxa;
  Data_Get_Struct(self, ruby_xml_attr_t, rxa);
  if (rxa->attr->doc == NULL)
    return(Qnil);
  else
    return(ruby_xml_document_wrap(cXMLDocument, rxa->attr->doc));
}

/*
 * call-seq:
 *    attr.doc? => (true|false)
 * 
 * Determine whether this attribute is associated with an
 * XML::Document.
 */
VALUE
ruby_xml_attr_doc_q(VALUE self) {
  ruby_xml_attr_t *rxa;
  Data_Get_Struct(self, ruby_xml_attr_t, rxa);
  if (rxa->attr->doc == NULL)
    return(Qfalse);
  else
    return(Qtrue);
}


/*
 * call-seq:
 *    attr.last => node
 * 
 * Obtain the last attribute.
 */
VALUE
ruby_xml_attr_last_get(VALUE self) {
  ruby_xml_attr_t *rxa;
  Data_Get_Struct(self, ruby_xml_attr_t, rxa);
  if (rxa->attr->last == NULL)
    return(Qnil);
  else
    return(ruby_xml_node2_wrap(cXMLNode, rxa->attr->last));
}


/*
 * call-seq:
 *    attr.last? => (true|false)
 * 
 * Determine whether this is the last attribute.
 */
VALUE
ruby_xml_attr_last_q(VALUE self) {
  ruby_xml_attr_t *rxa;
  Data_Get_Struct(self, ruby_xml_attr_t, rxa);
  if (rxa->attr->last == NULL)
    return(Qfalse);
  else
    return(Qtrue);
}

/*
 * call-seq:
 *    attr.name => "name"
 * 
 * Obtain this attribute's name.
 */
VALUE
ruby_xml_attr_name_get(VALUE self) {
  ruby_xml_attr_t *rxa;
  Data_Get_Struct(self, ruby_xml_attr_t, rxa);

  if (rxa->attr->name == NULL)
    return(Qnil);
  else
    return(rb_str_new2((const char*)rxa->attr->name));
}

/*
 * call-seq:
 *    attr.next => node
 * 
 * Obtain the next attribute.
 */
VALUE
ruby_xml_attr_next_get(VALUE self) {
  ruby_xml_attr_t *rxa;
  Data_Get_Struct(self, ruby_xml_attr_t, rxa);
  if (rxa->attr->next == NULL)
    return(Qnil);
  else
    return(ruby_xml_attr_wrap(cXMLAttr, rxa->attr->next));
}


/*
 * call-seq:
 *    attr.next? => (true|false)
 * 
 * Determine whether there is a next attribute.
 */
VALUE
ruby_xml_attr_next_q(VALUE self) {
  ruby_xml_attr_t *rxa;
  Data_Get_Struct(self, ruby_xml_attr_t, rxa);
  if (rxa->attr->next == NULL)
    return(Qfalse);
  else
    return(Qtrue);
}


/*
 * call-seq:
 *    attr.type_name => "attribute"
 * 
 * Obtain this attribute node's type name.
 */
VALUE
ruby_xml_attr_node_type_name(VALUE self) {
  return(rb_str_new2("attribute"));
}


/*
 * call-seq:
 *    attr.ns => namespace
 * 
 * Obtain this attribute's associated XML::NS, if any.
 */
VALUE
ruby_xml_attr_ns_get(VALUE self) {
  ruby_xml_attr_t *rxa;
  Data_Get_Struct(self, ruby_xml_attr_t, rxa);
  if (rxa->attr->ns == NULL)
    return(Qnil);
  else
    return(ruby_xml_ns_new2(cXMLNS, NULL, rxa->attr->ns));
}


/*
 * call-seq:
 *    attr.ns? => (true|false)
 * 
 * Determine whether this attribute has an associated 
 * namespace.
 */
VALUE
ruby_xml_attr_ns_q(VALUE self) {
  ruby_xml_attr_t *rxa;
  Data_Get_Struct(self, ruby_xml_attr_t, rxa);
  if (rxa->attr->ns == NULL)
    return(Qfalse);
  else
    return(Qtrue);
}


/*
 * call-seq:
 *    attr.parent => node
 * 
 * Obtain this attribute node's parent.
 */
VALUE
ruby_xml_attr_parent_get(VALUE self) {
  ruby_xml_attr_t *rxa;
  Data_Get_Struct(self, ruby_xml_attr_t, rxa);
  if (rxa->attr->parent == NULL)
    return(Qnil);
  else
    return(ruby_xml_node2_wrap(cXMLNode, rxa->attr->parent));
}


/*
 * call-seq:
 *    attr.parent? => (true|false)
 * 
 * Determine whether this attribute has a parent.
 */
VALUE
ruby_xml_attr_parent_q(VALUE self) {
  ruby_xml_attr_t *rxa;
  Data_Get_Struct(self, ruby_xml_attr_t, rxa);
  if (rxa->attr->parent == NULL)
    return(Qfalse);
  else
    return(Qtrue);
}


/*
 * call-seq:
 *    attr.prev => node
 * 
 * Obtain the previous attribute.
 */
VALUE
ruby_xml_attr_prev_get(VALUE self) {
  ruby_xml_attr_t *rxa;
  Data_Get_Struct(self, ruby_xml_attr_t, rxa);
  if (rxa->attr->prev == NULL)
    return(Qnil);
  else
    return(ruby_xml_attr_wrap(cXMLAttr, rxa->attr->prev));
}


/*
 * call-seq:
 *    attr.prev? => (true|false)
 * 
 * Determine whether there is a previous attribute.
 */
VALUE
ruby_xml_attr_prev_q(VALUE self) {
  ruby_xml_attr_t *rxa;
  Data_Get_Struct(self, ruby_xml_attr_t, rxa);
  if (rxa->attr->prev == NULL)
    return(Qfalse);
  else
    return(Qtrue);
}


/*
 * call-seq:
 *    attr.value => "value"
 * 
 * Obtain the value of this attribute.
 */
VALUE
ruby_xml_attr_value(VALUE self) {
  ruby_xml_attr_t *rxa;
  xmlChar *value;

  Data_Get_Struct(self, ruby_xml_attr_t, rxa);
  if (ruby_xml_attr_parent_q(self) == Qtrue) {
    value = xmlGetProp(rxa->attr->parent, rxa->attr->name);
    if (value != NULL)
      return(rb_str_new2((const char*)value));
  }
  return(Qnil);
}

// Rdoc needs to know 
#ifdef RDOC_NEVER_DEFINED
  mXML = rb_define_module("XML");
#endif

void
ruby_init_xml_attr(void) {
  cXMLAttr = rb_define_class_under(mXML, "Attr", rb_cObject);
  rb_define_method(cXMLAttr, "child", ruby_xml_attr_child_get, 0);
  rb_define_method(cXMLAttr, "child?", ruby_xml_attr_child_q, 0);
  rb_define_method(cXMLAttr, "doc", ruby_xml_attr_doc_get, 0);
  rb_define_method(cXMLAttr, "doc?", ruby_xml_attr_doc_q, 0);
  rb_define_method(cXMLAttr, "last", ruby_xml_attr_last_get, 0);
  rb_define_method(cXMLAttr, "last?", ruby_xml_attr_last_q, 0);
  rb_define_method(cXMLAttr, "name", ruby_xml_attr_name_get, 0);
  rb_define_method(cXMLAttr, "next", ruby_xml_attr_next_get, 0);
  rb_define_method(cXMLAttr, "next?", ruby_xml_attr_next_q, 0);
  rb_define_method(cXMLAttr, "node_type_name", ruby_xml_attr_node_type_name, 0);
  rb_define_method(cXMLAttr, "ns", ruby_xml_attr_ns_get, 0);
  rb_define_method(cXMLAttr, "ns?", ruby_xml_attr_ns_q, 0);
  rb_define_method(cXMLAttr, "parent", ruby_xml_attr_parent_get, 0);
  rb_define_method(cXMLAttr, "parent?", ruby_xml_attr_parent_q, 0);
  rb_define_method(cXMLAttr, "prev", ruby_xml_attr_prev_get, 0);
  rb_define_method(cXMLAttr, "prev?", ruby_xml_attr_prev_q, 0);
  rb_define_method(cXMLAttr, "value", ruby_xml_attr_value, 0);
}
