/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

#include "libxml.h"
#include "ruby_xml_node.h"

VALUE cXMLNode;
VALUE eXMLNodeSetNamespace;
VALUE eXMLNodeFailedModify;
VALUE eXMLNodeUnknownType;

static VALUE
check_string_or_symbol( VALUE val ) {
  if( TYPE(val) != T_STRING && TYPE(val) != T_SYMBOL ) {
    rb_raise(rb_eTypeError, "wrong argument type %s (expected String or Symbol)", 
    rb_obj_classname(val) );
  }
  return rb_obj_as_string( val );
}

/*
 * call-seq:
 *    node.attribute? => (true|false)
 * 
 * Determine whether this is an attribute node,
 */
VALUE
ruby_xml_node_attribute_q(VALUE self) {
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);
  if (rxn->node->type == XML_ATTRIBUTE_NODE)
    return(Qtrue);
  else
    return(Qfalse);
}


/*
 * call-seq:
 *    node.attribute_decl? => (true|false)
 * 
 * Determine whether this is an attribute declaration node,
 */
VALUE
ruby_xml_node_attribute_decl_q(VALUE self) {
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);
  if (rxn->node->type == XML_ATTRIBUTE_DECL)
    return(Qtrue);
  else
    return(Qfalse);
}


/*
 * call-seq:
 *    node.base => "uri"
 * 
 * Obtain this node's base URI.
 */
VALUE
ruby_xml_node_base_get(VALUE self) {
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);
  if (rxn->node->doc == NULL)
    return(Qnil);
    
  // TODO some NULL checking, raises ArgumentError in Ruby:
  //                          ArgumentError: NULL pointer given

  return(rb_str_new2((const char*)xmlNodeGetBase(rxn->node->doc, rxn->node)));
}


// TODO node_base_set should support setting back to nil

/*
 * call-seq:
 *    node.base = "uri"
 * 
 * Set this node's base URI.
 */
VALUE
ruby_xml_node_base_set(VALUE self, VALUE uri) {
  ruby_xml_node *node;

  Check_Type(uri, T_STRING);
  Data_Get_Struct(self, ruby_xml_node, node);
  if (node->node->doc == NULL)
    return(Qnil);

  xmlNodeSetBase(node->node, (xmlChar*)StringValuePtr(uri));
  return(Qtrue);
}


/*
 * call-seq:
 *    node.cdata? => (true|false)
 * 
 * Determine whether this is a #CDATA node
 */
VALUE
ruby_xml_node_cdata_q(VALUE self) {
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);
  if (rxn->node->type == XML_CDATA_SECTION_NODE)
    return(Qtrue);
  else
    return(Qfalse);
}


/*
 * call-seq:
 *    node.comment? => (true|false)
 * 
 * Determine whether this is a comment node
 */
VALUE
ruby_xml_node_comment_q(VALUE self) {
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);
  if (rxn->node->type == XML_COMMENT_NODE)
    return(Qtrue);
  else
    return(Qfalse);
}


/*
 * call-seq:
 *    node << ("string" | node) => node
 * 
 * Add the specified string or XML::Node to this node's
 * content.
 */
VALUE
ruby_xml_node_content_add(VALUE self, VALUE obj) {
  ruby_xml_node *node;
  VALUE str;

  Data_Get_Struct(self, ruby_xml_node, node);
  /* XXX This should only be legal for a CDATA type node, I think,
   * resulting in a merge of content, as if a string were passed
   * danj 070827
   */
  if (rb_obj_is_kind_of(obj, cXMLNode)) {
    ruby_xml_node_child_set(self, obj);
    return(self);
  } else if (TYPE(obj) == T_STRING) {
    xmlNodeAddContent(node->node, (xmlChar*)StringValuePtr(obj));
    return(self);
  } else {
    str = rb_obj_as_string(obj);
    if (NIL_P(str) || TYPE(str) != T_STRING)
      rb_raise(rb_eTypeError, "invalid argument: must be string or XML::Node");

    xmlNodeAddContent(node->node, (xmlChar*)StringValuePtr(str));
    return(self);
  }
}


/*
 * call-seq:
 *    node.content => "string"
 * 
 * Obtain this node's content as a string.
 */
VALUE
ruby_xml_node_content_get(VALUE self) {
  ruby_xml_node *rxn;
  xmlChar *content;
  VALUE out;
  
  Data_Get_Struct(self, ruby_xml_node, rxn);
  content = xmlNodeGetContent(rxn->node);
  out = rb_str_new2((const char *) content);
  xmlFree(content);
 
  return out;
}

/*
 * call-seq:
 *    node.content = "string"
 * 
 * Set this node's content to the specified string.
 */ 
VALUE
ruby_xml_node_content_set(VALUE self, VALUE content) {
  ruby_xml_node *node;

  Check_Type(content, T_STRING);
  Data_Get_Struct(self, ruby_xml_node, node);
  // XXX docs indicate need for escaping entites, need to be done? danj
  xmlNodeSetContent(node->node, (xmlChar*)StringValuePtr(content));
  return(Qtrue);
}


/*
 * call-seq:
 *    node.content_stripped => "string"
 * 
 * Obtain this node's stripped content.
 * 
 * *Deprecated*: Stripped content can be obtained via the
 * +content+ method.
 */
VALUE
ruby_xml_node_content_stripped_get(VALUE self) {
  ruby_xml_node *rxn;

  Data_Get_Struct(self, ruby_xml_node, rxn);
  if (rxn->node->content == NULL)
    return(Qnil);
  else
    return(rb_str_new2((const char*)xmlNodeGetContent(rxn->node)));
}

/*
 * call-seq:
 *    node.child => node
 * 
 * Obtain this node's first child node, if any.
 */
VALUE
ruby_xml_node_child_get(VALUE self) {
  ruby_xml_node *node;
  xmlNodePtr tmp;

  Data_Get_Struct(self, ruby_xml_node, node);

  switch (node->node->type) {
  case XML_ELEMENT_NODE:
  case XML_ENTITY_REF_NODE:
  case XML_ENTITY_NODE:
  case XML_PI_NODE:
  case XML_COMMENT_NODE:
  case XML_DOCUMENT_NODE:
#ifdef LIBXML_DOCB_ENABLED
  case XML_DOCB_DOCUMENT_NODE:
#endif
  case XML_HTML_DOCUMENT_NODE:
  case XML_DTD_NODE:
    tmp = node->node->children;
    break;
  case XML_ATTRIBUTE_NODE:
    {
      xmlAttrPtr attr = (xmlAttrPtr) node->node;
      tmp = attr->children;
      break;
    }
  default:
    tmp = NULL;
    break;
  }

  if (tmp == NULL)
    return(Qnil);
  else
    return(ruby_xml_node2_wrap(cXMLNode, tmp));
}


/*
 * call-seq:
 *    node.child? => (true|false)
 * 
 * Determine whether this node has at least one child.
 */
VALUE
ruby_xml_node_child_q(VALUE self) {
  ruby_xml_node *rxn;
  xmlNodePtr node;
  Data_Get_Struct(self, ruby_xml_node, rxn);

  node = NULL;
  switch (rxn->node->type) {
  case XML_ELEMENT_NODE:
  case XML_ENTITY_REF_NODE:
  case XML_ENTITY_NODE:
  case XML_PI_NODE:
  case XML_COMMENT_NODE:
  case XML_DOCUMENT_NODE:
#ifdef LIBXML_DOCB_ENABLED
  case XML_DOCB_DOCUMENT_NODE:
#endif
  case XML_HTML_DOCUMENT_NODE:
  case XML_DTD_NODE:
    node = rxn->node->children;
    break;
  case XML_ATTRIBUTE_NODE:
    {
      xmlAttrPtr attr = (xmlAttrPtr) rxn->node;
      node = attr->children;
      break;
    }
  default:
    node = NULL;
  }

  if (node == NULL)
    return(Qfalse);
  else
    return(Qtrue);
}


// TODO Fixes below should be applied to sibling, prev, etc ?
/*
 * call-seq:
 *    node.child = node
 * 
 * Set a child node for this node.
 */
VALUE
ruby_xml_node_child_set(VALUE self, VALUE rnode) {
  ruby_xml_node *cnode, *pnode;
  xmlNodePtr chld, ret;
  ruby_xml_document_t *pdoc, *cdoc;
  int ptr;

  if (rb_obj_is_kind_of(rnode, cXMLNode) == Qfalse)
    rb_raise(rb_eTypeError, "Must pass an XML::Node object");

  Data_Get_Struct(self,  ruby_xml_node, pnode);
  Data_Get_Struct(rnode, ruby_xml_node, cnode);
  
  chld = cnode->node;

#ifdef IGNORE  
  // Only copy if both nodes are in documents, which are different.
  if (pnode->xd && pnode->xd != Qnil) {
    Data_Get_Struct(pnode->xd, ruby_xml_document_t, pdoc);
    if (cnode->xd && cnode->xd != Qnil) {
      Data_Get_Struct(cnode->xd, ruby_xml_document_t, cdoc);
      if (cdoc->doc != pdoc->doc) {
        chld = xmlDocCopyNode(chld, pdoc->doc, 1);
        chld->_private = 0;
        ptr = 1;
      }
    } else {
      chld = xmlDocCopyNode(chld, pdoc->doc, 1);
      chld->_private = 0;
      ptr = 1;
    }
  } else {
    chld->doc = NULL;
  }
#endif
  
  ret = xmlAddChild(pnode->node, chld);
  if (ret == NULL)
    rb_raise(eXMLNodeFailedModify, "unable to add a child to the document");
    
  return ruby_xml_node2_wrap(cXMLNode,ret);
}

/*
 * call-seq:
 *    node.doc => document
 * 
 * Obtain the XML::Document this node belongs to.
 */
VALUE
ruby_xml_node_doc(VALUE self) {
  ruby_xml_document_t *rxd;
  ruby_xml_node *rxn;
  xmlDocPtr doc=NULL;
  VALUE docobj;

  Data_Get_Struct(self, ruby_xml_node, rxn);
  
  switch (rxn->node->type) {
  case XML_DOCUMENT_NODE:
#ifdef LIBXML_DOCB_ENABLED
  case XML_DOCB_DOCUMENT_NODE:
#endif
  case XML_HTML_DOCUMENT_NODE:
    doc = NULL;
    break;
  case XML_ATTRIBUTE_NODE:
    {
      xmlAttrPtr attr = (xmlAttrPtr) rxn->node;
      doc = attr->doc;
      break;
    }
  case XML_NAMESPACE_DECL:
    doc = NULL;
    break;
  default:
    doc = rxn->node->doc;
    break;
  }

  if (doc == NULL)
    return(Qnil);

  if ( doc->_private == NULL )
    rb_raise(rb_eRuntimeError,"existing document object has no ruby-instance");

  return (VALUE)doc->_private;
}


/*
 * call-seq:
 *    node.docbook? => (true|false)
 * 
 * Determine whether this is a docbook node.
 */
VALUE
ruby_xml_node_docbook_doc_q(VALUE self) {
#ifdef LIBXML_DOCB_ENABLED
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);
  if (rxn->node->type == XML_DOCB_DOCUMENT_NODE)
    return(Qtrue);
  else
    return(Qfalse);
#else
  rb_warn("libxml compiled without docbook support");
  return(Qfalse);
#endif
}


/*
 * call-seq:
 *    node.doctype? => (true|false)
 * 
 * Determine whether this is a DOCTYPE node.
 */
VALUE
ruby_xml_node_doctype_q(VALUE self) {
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);
  if (rxn->node->type == XML_DOCUMENT_TYPE_NODE)
    return(Qtrue);
  else
    return(Qfalse);
}


/*
 * call-seq:
 *    node.document? => (true|false)
 * 
 * Determine whether this is a document node.
 */
VALUE
ruby_xml_node_document_q(VALUE self) {
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);
  if (rxn->node->type == XML_DOCUMENT_NODE)
    return(Qtrue);
  else
    return(Qfalse);
}


/*
 * call-seq:
 *    node.dtd? => (true|false)
 * 
 * Determine whether this is a DTD node.
 */
VALUE
ruby_xml_node_dtd_q(VALUE self) {
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);
  if (rxn->node->type == XML_DTD_NODE)
    return(Qtrue);
  else
    return(Qfalse);
}


/*
 * call-seq:
 *    node.dump => (true|nil)
 * 
 * Dump this node to stdout.
 */
VALUE
ruby_xml_node_dump(VALUE self) {
  ruby_xml_node *rxn;
  xmlBufferPtr buf;

  Data_Get_Struct(self, ruby_xml_node, rxn);

  if (rxn->node->doc == NULL)
    return(Qnil);

  buf = xmlBufferCreate();
  xmlNodeDump(buf, rxn->node->doc, rxn->node, 0, 1);
  xmlBufferDump(stdout, buf);
  xmlBufferFree(buf);
  return(Qtrue);
}


/*
 * call-seq:
 *    node.debug_dump => (true|nil)
 * 
 * Dump this node to stdout, including any debugging
 * information.
 */
VALUE
ruby_xml_node_debug_dump(VALUE self) {
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);

  if (rxn->node->doc == NULL)
    return(Qnil);

  xmlElemDump(stdout, rxn->node->doc, rxn->node);
  return(Qtrue);
}


/*
 * call-seq:
 *    node.element? => (true|false)
 * 
 * Determine whether this is an element node.
 */
VALUE
ruby_xml_node_element_q(VALUE self) {
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);
  if (rxn->node->type == XML_ELEMENT_NODE)
    return(Qtrue);
  else
    return(Qfalse);
}


/*
 * call-seq:
 *    node.element_decl? => (true|false)
 * 
 * Determine whether this is an element declaration node.
 */
VALUE
ruby_xml_node_element_decl_q(VALUE self) {
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);
  if (rxn->node->type == XML_ELEMENT_DECL)
    return(Qtrue);
  else
    return(Qfalse);
}


/*
 * call-seq:
 *    node.empty? => (true|false)
 * 
 * Determine whether this node is empty.
 */
VALUE
ruby_xml_node_empty_q(VALUE self) {
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);
  if (rxn->node == NULL)
    return(Qnil);

  return((xmlIsBlankNode(rxn->node) == 1) ? Qtrue : Qfalse);
}


/*
 * call-seq:
 *    node.entity? => (true|false)
 * 
 * Determine whether this is an entity node.
 */
VALUE
ruby_xml_node_entity_q(VALUE self) {
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);
  if (rxn->node->type == XML_ENTITY_NODE)
    return(Qtrue);
  else
    return(Qfalse);
}


/*
 * call-seq:
 *    node.entity_ref? => (true|false)
 * 
 * Determine whether this is an entity reference node.
 */
VALUE
ruby_xml_node_entity_ref_q(VALUE self) {
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);
  if (rxn->node->type == XML_ENTITY_REF_NODE)
    return(Qtrue);
  else
    return(Qfalse);
}

VALUE ruby_xml_node_to_s(VALUE self);

/*
 * call-seq:
 *    node.eql?(other_node) => (true|false)
 * 
 * Test equality between the two nodes. Equality is determined based
 * on the XML representation of the nodes.
 */
VALUE
ruby_xml_node_eql_q(VALUE self, VALUE other) {
  // TODO this isn't the best way to handle this
  ruby_xml_node *rxn, *orxn;  
  VALUE thisxml, otherxml;
  Data_Get_Struct(self, ruby_xml_node, rxn);
  Data_Get_Struct(other, ruby_xml_node, orxn);
  thisxml = ruby_xml_node_to_s(self);
  otherxml = ruby_xml_node_to_s(other);
  
  return(rb_funcall(thisxml, rb_intern("=="), 1, otherxml));
}


/*
 * call-seq:
 *    node.find(xpath_expr, namespace = [any]) => nodeset
 * 
 * Find nodes matching the specified xpath expression, optionally
 * using the specified namespaces. Returns an XML::Node::Set.
 */
VALUE
ruby_xml_node_find(int argc, VALUE *argv, VALUE self) {
  int i, vargc;
  VALUE *vargv;

  if (argc > 2 || argc < 1)
    rb_raise(rb_eArgError, "wrong number of arguments (need 1 or 2)");

  vargc = argc + 1;
  vargv = ALLOC_N(VALUE, vargc + 1);
  vargv[0] = self;
  for (i = 0; i<argc; i++)
    vargv[i + 1] = argv[i];

  return(ruby_xml_xpath_find2(vargc, vargv));
}

/*
 * call-seq:
 *    node.find_first(xpath_expr, namespace = [any]) => nodeset
 *
 * Find the first node matching the specified xpath expression, optionally
 * using the specified namespaces. Returns an XML::Node.
 */
VALUE
ruby_xml_node_find_first(int argc, VALUE *argv, VALUE self) {
  VALUE ns = ruby_xml_node_find(argc, argv, self);
  ruby_xml_node_set *rxnset;
  VALUE nodeobj;

  Data_Get_Struct(ns, ruby_xml_node_set, rxnset);
  if (rxnset->node_set == NULL || rxnset->node_set->nodeNr < 1)
    return(Qnil);

  switch(rxnset->node_set->nodeTab[0]->type) {
    case XML_ATTRIBUTE_NODE:
      nodeobj = ruby_xml_attr_wrap(cXMLAttr, (xmlAttrPtr)rxnset->node_set->nodeTab[0]);
      break;
    default:
      nodeobj = ruby_xml_node2_wrap(cXMLNode, rxnset->node_set->nodeTab[0]);
  }

  return(nodeobj);
}


/*
 * call-seq:
 *    node.fragment? => (true|false)
 * 
 * Determine whether this node is a fragment.
 */
VALUE
ruby_xml_node_fragment_q(VALUE self) {
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);
  if (rxn->node->type == XML_DOCUMENT_FRAG_NODE)
    return(Qtrue);
  else
    return(Qfalse);
}

/*
 * call-seq:
 *    node.hash => fixnum
 * 
 * Returns the hash-code for this node. This is the hash of the XML
 * representation in order to be consistent with eql.
 */
VALUE
ruby_xml_node_hash(VALUE self) {
  ruby_xml_node *rxn;
  VALUE thisxml;
  Data_Get_Struct(self, ruby_xml_node, rxn);
  thisxml = ruby_xml_node_to_s(self);
  
  return(rb_funcall(thisxml, rb_intern("hash"), 0));
}


/*
 * call-seq:
 *    node.html_doc? => (true|false)
 * 
 * Determine whether this node is an html document node.
 */
VALUE
ruby_xml_node_html_doc_q(VALUE self) {
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);
  if (rxn->node->type == XML_HTML_DOCUMENT_NODE)
    return(Qtrue);
  else
    return(Qfalse);
}

/*
 * call-seq:
 *    XML::Node.new_cdata(content = nil) => node
 * 
 * Create a new #CDATA node, optionally setting
 * the node's content.
 */
VALUE
ruby_xml_node_new_cdata(int argc, VALUE *argv, VALUE class) {
  xmlNodePtr xnode;
  VALUE node, str=Qnil;

  switch(argc) {
  case 1:
    str = argv[0];
    Check_Type(str, T_STRING);
    if (!NIL_P(str)) {
      xnode = xmlNewCDataBlock(NULL, (xmlChar*)StringValuePtr(str), xmlStrlen((xmlChar*)StringValuePtr(str)));
    } else {
      xnode = xmlNewCDataBlock(NULL, NULL , 0);
    }

    if (xnode == NULL)
      return(Qnil);
      
    return ruby_xml_node2_wrap(class,xnode);

  default:
    rb_raise(rb_eArgError, "wrong number of arguments (1)");
  }

  // not reached
  return(Qnil);
}


/*
 * call-seq:
 *    XML::Node.new_comment(content = nil) => node
 * 
 * Create a new comment node, optionally setting
 * the node's content.
 * 
 */
VALUE
ruby_xml_node_new_comment(int argc, VALUE *argv, VALUE class) {
  xmlNodePtr xnode;
  VALUE node, str=Qnil;

  switch(argc) {
  case 1:
    str = argv[0];
    Check_Type(str, T_STRING);
// TODO  xmlNewComment wrongly? adds \n before and after the comment
    if (!NIL_P(str)) {
      xnode = xmlNewComment((xmlChar*)StringValuePtr(str));
    } else {
      xnode = xmlNewComment(NULL);
    }

    if (xnode == NULL)
      return(Qnil);

    return ruby_xml_node2_wrap(class,xnode);

  default:
    rb_raise(rb_eArgError, "wrong number of arguments (1)");
  }

  // not reached
  return(Qnil);
}

 
/*
 * call-seq:
 *    node.lang => "string"
 * 
 * Obtain the language set for this node, if any.
 * This is set in XML via the xml:lang attribute.
 */
VALUE
ruby_xml_node_lang_get(VALUE self) {
  ruby_xml_node *rxn;
  xmlChar *lang;

  Data_Get_Struct(self, ruby_xml_node, rxn);
  lang = xmlNodeGetLang(rxn->node);

  if (lang == NULL)
    return(Qnil);
  else
    return(rb_str_new2((const char*)lang));
}


// TODO node_lang_set should support setting back to nil

/*
 * call-seq:
 *    node.lang = "string"
 * 
 * Set the language for this node. This affects the value
 * of the xml:lang attribute.
 */
VALUE
ruby_xml_node_lang_set(VALUE self, VALUE lang) {
  ruby_xml_node *node;

  Check_Type(lang, T_STRING);
  Data_Get_Struct(self, ruby_xml_node, node);
  xmlNodeSetLang(node->node, (xmlChar*)StringValuePtr(lang));

  return(Qtrue);
}


/*
 * call-seq:
 *    node.last => node
 * 
 * Obtain the last child node of this node, if any.
 */
VALUE
ruby_xml_node_last_get(VALUE self) {
  ruby_xml_node *rxn;
  xmlNodePtr node;

  Data_Get_Struct(self, ruby_xml_node, rxn);

  switch (rxn->node->type) {
  case XML_ELEMENT_NODE:
  case XML_ENTITY_REF_NODE:
  case XML_ENTITY_NODE:
  case XML_PI_NODE:
  case XML_COMMENT_NODE:
  case XML_DOCUMENT_NODE:
#ifdef LIBXML_DOCB_ENABLED
  case XML_DOCB_DOCUMENT_NODE:
#endif
  case XML_HTML_DOCUMENT_NODE:
  case XML_DTD_NODE:
    node = rxn->node->last;
    break;
  case XML_ATTRIBUTE_NODE:
    {
      xmlAttrPtr attr = (xmlAttrPtr) rxn->node;
      node = attr->last;
    }
  default:
    node = NULL;
    break;
  }

  if (node == NULL)
    return(Qnil);
  else
    return(ruby_xml_node2_wrap(cXMLNode, node));
}


/*
 * call-seq:
 *    node.last? => (true|false)
 * 
 * Determine whether this node has a last child node.
 */
VALUE
ruby_xml_node_last_q(VALUE self) {
  ruby_xml_node *rxn;
  xmlNodePtr node;

  Data_Get_Struct(self, ruby_xml_node, rxn);

  switch (rxn->node->type) {
  case XML_ELEMENT_NODE:
  case XML_ENTITY_REF_NODE:
  case XML_ENTITY_NODE:
  case XML_PI_NODE:
  case XML_COMMENT_NODE:
  case XML_DOCUMENT_NODE:
#ifdef LIBXML_DOCB_ENABLED
  case XML_DOCB_DOCUMENT_NODE:
#endif
  case XML_HTML_DOCUMENT_NODE:
  case XML_DTD_NODE:
    node = rxn->node->last;
    break;
  case XML_ATTRIBUTE_NODE:
    {
      xmlAttrPtr attr = (xmlAttrPtr) rxn->node;
      node = attr->last;
    }
  default:
    node = NULL;
    break;
  }

  if (node == NULL)
    return(Qfalse);
  else
    return(Qtrue);
}


/*
 * call-seq:
 *    node.line_num => num
 * 
 * Obtain the line number (in the XML document) that this
 * node was read from. If +default_line_numbers+ is set
 * false (the default), this method returns zero.
 */
VALUE
ruby_xml_node_line_num(VALUE self) {
  ruby_xml_node *rxn;
  long line_num;
  Data_Get_Struct(self, ruby_xml_node, rxn);

  if (!xmlLineNumbersDefaultValue)
    rb_warn("Line numbers were not retained: use XML::Parser::default_line_numbers=true");

  line_num = xmlGetLineNo(rxn->node);
  if (line_num == -1)
    return(Qnil);
  else
    return(INT2NUM((long)line_num));
}


/*
 * call-seq:
 *    node.xlink? => (true|false)
 * 
 * Determine whether this node is an xlink node.
 */
VALUE
ruby_xml_node_xlink_q(VALUE self) {
  ruby_xml_node *node;
  xlinkType xlt;

  Data_Get_Struct(self, ruby_xml_node, node);
  xlt = xlinkIsLink(node->node->doc, node->node);

  if (xlt == XLINK_TYPE_NONE)
    return(Qfalse);
  else
    return(Qtrue);
}


/*
 * call-seq:
 *    node.xlink_type => num
 * 
 * Obtain the type identifier for this xlink, if applicable.
 * If this is not an xlink node (see +xlink?+), will return
 * nil.
 */
VALUE
ruby_xml_node_xlink_type(VALUE self) {
  ruby_xml_node *node;
  ruby_xml_document_t *doc;
  xlinkType xlt;

  Data_Get_Struct(self, ruby_xml_node, node);
  xlt = xlinkIsLink(node->node->doc, node->node);

  if (xlt == XLINK_TYPE_NONE)
    return(Qnil);
  else
    return(INT2NUM(xlt));
}


/*
 * call-seq:
 *    node.xlink_type_name => "string"
 * 
 * Obtain the type name for this xlink, if applicable.
 * If this is not an xlink node (see +xlink?+), will return
 * nil.
 */
VALUE
ruby_xml_node_xlink_type_name(VALUE self) {
  ruby_xml_node *node;
  ruby_xml_document_t *doc;
  xlinkType xlt;

  Data_Get_Struct(self, ruby_xml_node, node);
  xlt = xlinkIsLink(node->node->doc, node->node);

  switch(xlt) {
  case XLINK_TYPE_NONE:
    return(Qnil);
  case XLINK_TYPE_SIMPLE:
    return(rb_str_new2("simple"));
  case XLINK_TYPE_EXTENDED:
    return(rb_str_new2("extended"));
  case XLINK_TYPE_EXTENDED_SET:
    return(rb_str_new2("extended_set"));
  default:
    rb_fatal("Unknowng xlink type, %d", xlt);
  }
}

/*
 * call-seq:
 *    node.name => "string"
 * 
 * Obtain this node's name.
 */
VALUE
ruby_xml_node_name_get(VALUE self) {
  ruby_xml_node *rxn;
  const xmlChar *name;

  Data_Get_Struct(self, ruby_xml_node, rxn);

  switch (rxn->node->type) {
  case XML_DOCUMENT_NODE:
#ifdef LIBXML_DOCB_ENABLED
  case XML_DOCB_DOCUMENT_NODE:
#endif
  case XML_HTML_DOCUMENT_NODE:
    {
      xmlDocPtr doc = (xmlDocPtr) rxn->node;
      name = doc->URL;
      break;
    }
  case XML_ATTRIBUTE_NODE:
    {
      xmlAttrPtr attr = (xmlAttrPtr) rxn->node;
      name = attr->name;
      break;
    }
  case XML_NAMESPACE_DECL:
    {
      xmlNsPtr ns = (xmlNsPtr) rxn->node;
      name = ns->prefix;
      break;
    }
  default:
    name = rxn->node->name;
    break;
  }

  if (rxn->node->name == NULL)
    return(Qnil);
  else
    return(rb_str_new2((const char*)name));
}


/*
 * call-seq:
 *    node.name = "string"
 * 
 * Set this node's name.
 */
VALUE
ruby_xml_node_name_set(VALUE self, VALUE name) {
  ruby_xml_node *node;

  Check_Type(name, T_STRING);
  Data_Get_Struct(self, ruby_xml_node, node);
  xmlNodeSetName(node->node, (xmlChar*)StringValuePtr(name));
  return(Qtrue);
}


/*
 * call-seq:
 *    node.namespace => [namespace, ..., namespace]
 * 
 * Obtain an array of +XML::NS+ objects representing
 * this node's xmlns attributes
 */
VALUE
ruby_xml_node_namespace_get(VALUE self) {
  ruby_xml_node *node;
  xmlNsPtr *nsList, *cur;
  VALUE arr, ns;

  Data_Get_Struct(self, ruby_xml_node, node);
  if (node->node == NULL)
    return(Qnil);

  nsList = xmlGetNsList(node->node->doc, node->node);

  if (nsList == NULL)
    return(Qnil);

  arr = rb_ary_new();
  for (cur = nsList; *cur != NULL; cur++) {
    ns = ruby_xml_ns_new2(cXMLNS, ruby_xml_document_wrap(cXMLDocument,node->node->doc), *cur);
    if (ns == Qnil)
      continue;
    else
      rb_ary_push(arr, ns);
  }
  xmlFree(nsList);

  return(arr);
}


/*
 * call-seq:
 *    node.namespace_node => namespace.
 * 
 * Obtain this node's namespace node.
 */
VALUE
ruby_xml_node_namespace_get_node(VALUE self) {
  ruby_xml_node *node;

  Data_Get_Struct(self, ruby_xml_node, node);
  if (node->node->ns == NULL)
    return(Qnil);
  else
    return ruby_xml_ns_new2(cXMLNS,
			    ruby_xml_document_wrap(cXMLDocument,node->node->doc),
			    node->node->ns);
}

// TODO namespace_set can take varargs (in fact, must if used
//      with strings), but I cannot see how you can call 
//      that version, apart from with 'send'
//
//      Would sure be nice to support foo.namespace['foo'] = 'bar'
//      but maybe that's not practical...

/*
 * call-seq:
 *    node.namespace = namespace
 * 
 * Add the specified XML::NS object to this node's xmlns attributes.
 */
VALUE
ruby_xml_node_namespace_set(int argc, VALUE *argv, VALUE self) {
  VALUE rns, rprefix;
  ruby_xml_node *rxn;
  ruby_xml_ns *rxns;
  xmlNsPtr ns;
  char *cp, *href;

  Data_Get_Struct(self, ruby_xml_node, rxn);
  switch (argc) {
  case 1:
    rns = argv[0];
    if (TYPE(rns) == T_STRING) {
      cp = strchr(StringValuePtr(rns), (int)':');
      if (cp == NULL) {
	rprefix = rns;
	href = NULL;
      } else {
	rprefix = rb_str_new(StringValuePtr(rns), (int)((long)cp - (long)StringValuePtr(rns)));
	href = &cp[1]; /* skip the : */
      }
    } else if (rb_obj_is_kind_of(rns, cXMLNS) == Qtrue) {
      Data_Get_Struct(self, ruby_xml_ns, rxns);
      xmlSetNs(rxn->node, rxns->ns);
      return(rns);
    } else
      rb_raise(rb_eTypeError, "must pass a string or an XML::Ns object");

    /* Fall through to next case because when argc == 1, we need to
     * manually setup the additional args unless the arg passed is of
     * cXMLNS type */
  case 2:
    /* Don't want this code run in the fall through case */
    if (argc == 2) {
      rprefix = argv[0];
      href = StringValuePtr(argv[1]);
    }

    ns = xmlNewNs(rxn->node, (xmlChar*)href, (xmlChar*)StringValuePtr(rprefix));
    if (ns == NULL)
      rb_raise(eXMLNodeSetNamespace, "unable to set the namespace");
    else
      return ruby_xml_ns_new2(cXMLNS, ruby_xml_document_wrap(cXMLDocument,rxn->node->doc), ns);
    break;

  default:
    rb_raise(rb_eArgError, "wrong number of arguments (1 or 2)");
  }

  /* can't get here */
  return(Qnil);
}


/*
 * call-seq:
 *    node.namespace? => (true|false)
 * 
 * Determine whether this node *is* (not has) a namespace
 * node.
 */
VALUE
ruby_xml_node_namespace_q(VALUE self) {
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);
  if (rxn->node->type == XML_NAMESPACE_DECL)
    return(Qtrue);
  else
    return(Qfalse);
}

/*
 * memory2 implementation: xmlNode->_private holds a reference
 * to the wrapping ruby object VALUE when there is one.
 * traversal for marking is upward, and top levels are marked
 * through and lower level mark entry.
 *
 * All ruby retrieval for an xml
 * node will result in the same ruby instance. When all handles to them
 * go out of scope, then free gets called and _private is set to NULL.
 * If the xmlNode has no parent or document, then call xmlFree.
 */
void
ruby_xml_node2_free(ruby_xml_node *rxn) {

  if (rxn->node == NULL ) return;

  if (rxn->node->parent == NULL && rxn->node->doc == NULL ) {
#ifdef NODE_DEBUG
    fprintf(stderr,"free rxn=0x%x xn=0x%x o=0x%x\n",(long)rxn,(long)rxn->node,(long)rxn->node->_private);
#endif
    rxn->node->_private=NULL;
    xmlFreeNode(rxn->node);
  }

  rxn->node=NULL;
  //  fprintf(stderr,"%0x ",(long)rxn);
  free(rxn);
}

void
ruby_xml_node2_mark(ruby_xml_node *rxn) {
  xmlNodePtr node;
  if (rxn->node == NULL ) return;

  if (rxn->node->_private == NULL ) {
    rb_warning("XmlNode is not bound! (%s:%d)",
	       __FILE__,__LINE__);
    return;
  }

  if (rxn->node->doc != NULL ) {
    if (rxn->node->doc->_private == NULL )
      rb_warning("XmlNode Doc is not bound! (%s:%d)",
		 __FILE__,__LINE__);
    else {
      rb_gc_mark((VALUE)rxn->node->doc->_private);
#ifdef NODE_DEBUG
      fprintf(stderr,"mark rxn=0x%x xn=0x%x o=0x%x\n",(long)rxn,(long)rxn->node,(long)rxn->node->_private);
#endif
    }
  } else if (rxn->node->parent != NULL ) {
    if (rxn->node->parent->_private == NULL )
      rb_warning("XmlNode Parent is not bound! (%s:%d)",
		 __FILE__,__LINE__);
    node=rxn->node;
    while (node->parent != NULL )
      node=node->parent;
    if (node->_private != NULL) {
      rb_gc_mark((VALUE)node->_private);
#ifdef NODE_DEBUG
      fprintf(stderr,"mark rxn=0x%x xn=0x%x o=0x%x\n",(long)0,(long)node,(long)node->_private);
#endif
    }
  }
}

VALUE
ruby_xml_node2_wrap(VALUE class, xmlNodePtr xnode)
{
  VALUE obj;
  ruby_xml_node *rxn;

  // This node is already wrapped
  if (xnode->_private != NULL)
    return (VALUE)xnode->_private;

  obj=Data_Make_Struct(class,ruby_xml_node,ruby_xml_node2_mark,
		       ruby_xml_node2_free,rxn);

  rxn->node=xnode;
  xnode->_private=(void*)obj;
#ifdef NODE_DEBUG
  fprintf(stderr,"wrap rxn=0x%x xn=0x%x o=0x%x\n",(long)rxn,(long)xnode,(long)obj);
#endif
  return obj;
}

VALUE
ruby_xml_node2_new_native(VALUE class, VALUE ns, VALUE name)
{
  VALUE obj;
  xmlNodePtr xnode;
  xmlNsPtr xns=NULL;
  ruby_xml_node *rxn;

  if ( ! NIL_P(ns) ) {
    Data_Get_Struct(ns,xmlNs,xns);
  }
  xnode=xmlNewNode(xns,(xmlChar*)StringValuePtr(name));
  xnode->_private=NULL;

  obj=
    ruby_xml_node2_wrap(class,xnode);
  
  rb_obj_call_init(obj,0,NULL);
  return obj;
}

VALUE
ruby_xml_node2_new_string(VALUE class, VALUE ns, VALUE name, VALUE val)
{
  VALUE obj;
  char* value;
  obj=ruby_xml_node2_new_native(class,ns,name);
  if ( ! NIL_P(val) ) {
    if ( TYPE(val) != T_STRING )
      val=rb_obj_as_string(val);
    ruby_xml_node_content_set(obj,val);
  }
  return obj;
}
/*
 * call-seq:
 *    XML::Node.new(name, content = nil) => node
 *    XML::Node.new_element(name, content = nil) => node
 * 
 * Create a new element node with the specified name, optionally setting
 * the node's content.
 * backward compatibility for <.5 new
 */
VALUE
ruby_xml_node2_new_string_bc(int argc, VALUE *argv, VALUE class)
{
  VALUE content=Qnil,name=Qnil,rxnode;
  switch(argc) {
  case 2:
    content=argv[1];
    if ( TYPE(content) != T_STRING)
      content=rb_obj_as_string(content);
    
  case 1:
    name=check_string_or_symbol( argv[0] );
    return ruby_xml_node2_new_string(class,Qnil,name,content);

  default:
    rb_raise(rb_eArgError, "wrong number of arguments (1 or 2) given %d",argc);
  }
}

/*
 * call-seq:
 *    node.next => node
 * 
 * Obtain the next sibling node, if any.
 */
VALUE
ruby_xml_node_next_get(VALUE self) {
  ruby_xml_node *rxn;
  xmlNodePtr node;
  Data_Get_Struct(self, ruby_xml_node, rxn);

  switch (rxn->node->type) {
  case XML_DOCUMENT_NODE:
#ifdef LIBXML_DOCB_ENABLED
  case XML_DOCB_DOCUMENT_NODE:
#endif
  case XML_HTML_DOCUMENT_NODE:
    node = NULL;
    break;
  case XML_ATTRIBUTE_NODE:
    {
      xmlAttrPtr attr = (xmlAttrPtr) rxn->node;
      node = (xmlNodePtr) attr->next;
      break;
    }
  case XML_NAMESPACE_DECL:
    {
      xmlNsPtr ns = (xmlNsPtr) rxn->node;
      node = (xmlNodePtr) ns->next;
      break;
    }
  default:
    node = rxn->node->next;
    break;
  }

  if (node == NULL) {
    return(Qnil);
  } else {
    return(ruby_xml_node2_wrap(cXMLNode, node));
  }
}


/*
 * call-seq:
 *    node.next? => (true|false)
 * 
 * Determine whether this node has a next sibling.
 */
VALUE
ruby_xml_node_next_q(VALUE self) {
  ruby_xml_node *rxn;
  xmlNodePtr node;
  Data_Get_Struct(self, ruby_xml_node, rxn);

  switch (rxn->node->type) {
  case XML_DOCUMENT_NODE:
#ifdef LIBXML_DOCB_ENABLED
  case XML_DOCB_DOCUMENT_NODE:
#endif
  case XML_HTML_DOCUMENT_NODE:
    node = NULL;
    break;
  case XML_ATTRIBUTE_NODE:
    {
      xmlAttrPtr attr = (xmlAttrPtr) rxn->node;
      node = (xmlNodePtr) attr->next;
      break;
    }
  case XML_NAMESPACE_DECL:
    {
      xmlNsPtr ns = (xmlNsPtr) rxn->node;
      node = (xmlNodePtr) ns->next;
      break;
    }
  default:
    node = rxn->node->next;
    break;
  }

  if (node == NULL)
    return(Qfalse);
  else
    return(Qtrue);
}


/*
 * call-seq:
 *    node.next = node
 * 
 * Insert the specified node as this node's next sibling.
 */
VALUE
ruby_xml_node_next_set(VALUE self, VALUE rnode) {
  ruby_xml_node *cnode, *pnode;
  xmlNodePtr ret;

  if (rb_obj_is_kind_of(rnode, cXMLNode) == Qfalse)
    rb_raise(rb_eTypeError, "Must pass an XML::Node object");

  Data_Get_Struct(self,  ruby_xml_node, pnode);
  Data_Get_Struct(rnode, ruby_xml_node, cnode);

  ret = xmlAddNextSibling(pnode->node, cnode->node);
  if (ret == NULL)
    rb_raise(eXMLNodeFailedModify, "unable to add a sibling to the document");

  return(ruby_xml_node2_wrap(cXMLNode, ret));
}


/*
 * call-seq:
 *    node.notation? => (true|false)
 * 
 * Determine whether this is a notation node
 */
VALUE
ruby_xml_node_notation_q(VALUE self) {
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);
  if (rxn->node->type == XML_NOTATION_NODE)
    return(Qtrue);
  else
    return(Qfalse);
}


/*
 * call-seq:
 *    node.ns? => (true|false)
 * 
 * Determine whether this node is a namespace node.
 */
VALUE
ruby_xml_node_ns_q(VALUE self) {
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);
  if (rxn->node->ns == NULL)
    return(Qfalse);
  else
    return(Qtrue);
}


/*
 * call-seq:
 *    node.ns_def => namespace
 * 
 * Obtain this node's default namespace.
 */
VALUE
ruby_xml_node_ns_def_get(VALUE self) {
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);
  if (rxn->node->nsDef == NULL)
    return(Qnil);
  else
    return(ruby_xml_ns_new2(cXMLNS, ruby_xml_document_wrap(cXMLDocument,rxn->node->doc), rxn->node->nsDef));
}


/*
 * call-seq:
 *    node.ns_def? => (true|false)
 * 
 * Obtain an array of +XML::NS+ objects representing
 * this node's xmlns attributes
 */
VALUE
ruby_xml_node_ns_def_q(VALUE self) {
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);
  if (rxn->node->nsDef == NULL)
    return(Qfalse);
  else
    return(Qtrue);
}


/*
 * call-seq:
 *    node.parent => node
 * 
 * Obtain this node's parent node, if any.
 */
VALUE
ruby_xml_node_parent_get(VALUE self) {
  ruby_xml_node *rxn;
  xmlNodePtr node;

  Data_Get_Struct(self, ruby_xml_node, rxn);

  switch (rxn->node->type) {
  case XML_DOCUMENT_NODE:
  case XML_HTML_DOCUMENT_NODE:
#ifdef LIBXML_DOCB_ENABLED
  case XML_DOCB_DOCUMENT_NODE:
#endif
    node = NULL;
    break;
  case XML_ATTRIBUTE_NODE:
    {
      xmlAttrPtr attr = (xmlAttrPtr) rxn->node;
      node = attr->parent;
    }
  case XML_ENTITY_DECL:
  case XML_NAMESPACE_DECL:
  case XML_XINCLUDE_START:
  case XML_XINCLUDE_END:
    node = NULL;
    break;
  default:
    node = rxn->node->parent;
    break;
  }

  if (node == NULL)
    return(Qnil);
  else
    return(ruby_xml_node2_wrap(cXMLNode, node));
}


/*
 * call-seq:
 *    node.parent? => (true|false)
 * 
 * Determine whether this node has a parent node.
 */
VALUE
ruby_xml_node_parent_q(VALUE self) {
  ruby_xml_node *rxn;
  xmlNodePtr node;

  Data_Get_Struct(self, ruby_xml_node, rxn);

  switch (rxn->node->type) {
  case XML_DOCUMENT_NODE:
  case XML_HTML_DOCUMENT_NODE:
#ifdef LIBXML_DOCB_ENABLED
  case XML_DOCB_DOCUMENT_NODE:
#endif
    node = NULL;
    break;
  case XML_ATTRIBUTE_NODE:
    {
      xmlAttrPtr attr = (xmlAttrPtr) rxn->node;
      node = attr->parent;
    }
  case XML_ENTITY_DECL:
  case XML_NAMESPACE_DECL:
  case XML_XINCLUDE_START:
  case XML_XINCLUDE_END:
    node = NULL;
    break;
  default:
    node = rxn->node->parent;
    break;
  }

  if (node == NULL)
    return(Qfalse);
  else
    return(Qtrue);
}


/*
 * call-seq:
 *    node.path => path
 * 
 * Obtain this node's path.
 */
VALUE
ruby_xml_node_path(VALUE self) {
  ruby_xml_node *rxn;
  xmlChar *path;

  Data_Get_Struct(self, ruby_xml_node, rxn);
  path = xmlGetNodePath(rxn->node);

  if (path == NULL)
    return(Qnil);
  else
    return(rb_str_new2((const char*)path));
}


/*
 * call-seq:
 *    node.pi? => (true|false)
 * 
 * Determine whether this is a processing instruction node.
 */
VALUE
ruby_xml_node_pi_q(VALUE self) {
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);
  if (rxn->node->type == XML_PI_NODE)
    return(Qtrue);
  else
    return(Qfalse);
}


/*
 * call-seq:
 *    node.pointer => node_set
 * 
 * Evaluates an XPointer expression relative to this node.
 */
VALUE
ruby_xml_node_pointer(VALUE self, VALUE xptr_str) {
  return(ruby_xml_xpointer_point2(self, xptr_str));
}


/*
 * call-seq:
 *    node.prev => node
 * 
 * Obtain the previous sibling, if any.
 */
VALUE
ruby_xml_node_prev_get(VALUE self) {
  ruby_xml_node *rxn;
  xmlNodePtr node;
  Data_Get_Struct(self, ruby_xml_node, rxn);

  switch (rxn->node->type) {
  case XML_DOCUMENT_NODE:
#ifdef LIBXML_DOCB_ENABLED
  case XML_DOCB_DOCUMENT_NODE:
#endif
  case XML_HTML_DOCUMENT_NODE:
  case XML_NAMESPACE_DECL:
    node = NULL;
    break;
  case XML_ATTRIBUTE_NODE:
    {
      xmlAttrPtr attr = (xmlAttrPtr) rxn->node;
      node = (xmlNodePtr) attr->prev;
    }
    break;
  default:
    node = rxn->node->prev;
    break;
  }

  if (node == NULL)
    return(Qnil);
  else
    return(ruby_xml_node2_wrap(cXMLNode, node));
}


/*
 * call-seq:
 *    node.prev? => (true|false)
 * 
 * Determines whether this node has a previous sibling node.
 */
VALUE
ruby_xml_node_prev_q(VALUE self) {
  ruby_xml_node *rxn;
  xmlNodePtr node;

  Data_Get_Struct(self, ruby_xml_node, rxn);

  switch (rxn->node->type) {
  case XML_DOCUMENT_NODE:
#ifdef LIBXML_DOCB_ENABLED
  case XML_DOCB_DOCUMENT_NODE:
#endif
  case XML_HTML_DOCUMENT_NODE:
  case XML_NAMESPACE_DECL:
    node = NULL;
    break;
  case XML_ATTRIBUTE_NODE:
    {
      xmlAttrPtr attr = (xmlAttrPtr) rxn->node;
      node = (xmlNodePtr) attr->prev;
    }
    break;
  default:
    node = rxn->node->prev;
    break;
  }

  if (node == NULL)
    return(Qfalse);
  else
    return(Qtrue);
}


/*
 * call-seq:
 *    node.prev = node
 * 
 * Insert the specified node as this node's previous sibling.
 */
VALUE
ruby_xml_node_prev_set(VALUE self, VALUE rnode) {
  ruby_xml_node *cnode, *pnode;
  xmlNodePtr ret;

  if (rb_obj_is_kind_of(rnode, cXMLNode) == Qfalse)
    rb_raise(rb_eTypeError, "Must pass an XML::Node object");

  Data_Get_Struct(self,  ruby_xml_node, pnode);
  Data_Get_Struct(rnode, ruby_xml_node, cnode);

  ret = xmlAddPrevSibling(pnode->node, cnode->node);
  if (ret == NULL)
    rb_raise(eXMLNodeFailedModify, "unable to add a sibling to the document");

  return(ruby_xml_node2_wrap(cXMLNode, ret));
}


/*
 * call-seq:
 *    node.property("name") => "string"
 *    node["name"]          => "string"
 * 
 * Obtain the named property.
 */
VALUE
ruby_xml_node_property_get(VALUE self, VALUE prop) {
  ruby_xml_node *rxn;
  xmlChar *p;
  VALUE r;
  
  prop = check_string_or_symbol( prop );
  
  Data_Get_Struct(self, ruby_xml_node, rxn);
  p = xmlGetProp(rxn->node, (xmlChar*)StringValuePtr(prop));

  if (p == NULL)
    r = Qnil;
  else {
    r = rb_str_new2((const char*)p);
    xmlFree(p);
  }
  
  return r;
}


/*
 * call-seq:
 *    node["name"] = "string"
 * 
 * Set the named property.
 */
VALUE
ruby_xml_node_property_set(VALUE self, VALUE key, VALUE val) {
  ruby_xml_node *node;
  xmlAttrPtr attr;

  key = check_string_or_symbol( key );
  Data_Get_Struct(self, ruby_xml_node, node); 
  
  if( val == Qnil ) {
    attr = xmlSetProp(node->node, (xmlChar*)StringValuePtr(key), NULL);
    xmlRemoveProp( attr );
    return Qnil;
  } else {
    Check_Type(val, T_STRING);
  }
  
  attr = xmlSetProp(node->node, (xmlChar*)StringValuePtr(key), (xmlChar*)StringValuePtr(val));
  if (attr == NULL) {
    attr = xmlNewProp(node->node, (xmlChar*)StringValuePtr(key), (xmlChar*)StringValuePtr(val));
    if (attr == NULL)
      return(Qnil);
  }
  return(ruby_xml_attr_new(cXMLAttr, attr));
}


/*
 * call-seq:
 *    node.properties => attributes
 * 
 * Returns the +XML::Attr+ for this node. 
 */
VALUE
ruby_xml_node_properties_get(VALUE self) {
  ruby_xml_node *node;
  xmlAttrPtr attr;

  Data_Get_Struct(self, ruby_xml_node, node);

  if (node->node->type == XML_ELEMENT_NODE) {
    attr = node->node->properties;
    
    if (attr == NULL) {
      return(Qnil);
    } else {
      return(ruby_xml_attr_wrap(cXMLAttr, attr));
    }
  } else {
    return(Qnil);
  }
}


/*
 * call-seq:
 *    node.properties? => (true|false)
 * 
 * Determine whether this node has properties
 * (attributes).
 */
VALUE
ruby_xml_node_properties_q(VALUE self) {
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);
  if (rxn->node->type == XML_ELEMENT_NODE && rxn->node->properties != NULL)
    return(Qtrue);
  else
    return(Qfalse);
}


/*
 * call-seq:
*    node.remove! => nil
*
* Removes this node from it's parent.
*/
VALUE
ruby_xml_node_remove_ex(VALUE self) {
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);
  xmlUnlinkNode(rxn->node);
    return(Qnil);
}


/*
 * call-seq:
 *    node.search_href => namespace
 * 
 * Search for a namespace by href.
 */
VALUE
ruby_xml_node_search_href(VALUE self, VALUE href) {
  ruby_xml_node *node;

  Check_Type(href, T_STRING);
  Data_Get_Struct(self, ruby_xml_node, node);
  return(ruby_xml_ns_new2(cXMLNS, ruby_xml_document_wrap(cXMLDocument,node->node->doc),
			  xmlSearchNsByHref(node->node->doc, node->node,
					    (xmlChar*)StringValuePtr(href))));
}


/*
 * call-seq:
 *    node.search_ns => namespace
 * 
 * Search for a namespace by namespace.
 */
VALUE
ruby_xml_node_search_ns(VALUE self, VALUE ns) {
  ruby_xml_node *node;

  Check_Type(ns, T_STRING);
  Data_Get_Struct(self, ruby_xml_node, node);
  return(ruby_xml_ns_new2(cXMLNS,
			  ruby_xml_document_wrap(cXMLDocument,node->node->doc),
			  xmlSearchNs(node->node->doc, node->node,
				      (xmlChar*)StringValuePtr(ns))));
}


/*
 * call-seq:
 *    node.sibling(node) => node
 * 
 * Add the specified node as a sibling of this node.
 */
VALUE
ruby_xml_node_sibling_set(VALUE self, VALUE rnode) {
  ruby_xml_node *cnode, *pnode;
  xmlNodePtr ret;
  VALUE obj;

  if (rb_obj_is_kind_of(rnode, cXMLNode) == Qfalse)
    rb_raise(rb_eTypeError, "Must pass an XML::Node object");

  Data_Get_Struct(self,  ruby_xml_node, pnode);
  Data_Get_Struct(rnode, ruby_xml_node, cnode);

  ret = xmlAddSibling(pnode->node, cnode->node);
  if (ret == NULL)
    rb_raise(eXMLNodeFailedModify, "unable to add a sibling to the document");
  if (ret->_private==NULL)
    obj=ruby_xml_node2_wrap(cXMLNode,ret);
  else
    obj=(VALUE)ret->_private;

  return obj;
}


/*
 * call-seq:
 *    node.space_preserve => (true|false)
 * 
 * Determine whether this node preserves whitespace.
 */
VALUE
ruby_xml_node_space_preserve_get(VALUE self) {
  ruby_xml_node *rxn;

  Data_Get_Struct(self, ruby_xml_node, rxn);
  return(INT2NUM(xmlNodeGetSpacePreserve(rxn->node)));
}


/*
 * call-seq:
 *    node.space_preserve = true|false
 * 
 * Control whether this node preserves whitespace.
 */
VALUE
ruby_xml_node_space_preserve_set(VALUE self, VALUE bool) {
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);

  if (TYPE(bool) == T_FALSE)
    xmlNodeSetSpacePreserve(rxn->node, 1);
  else
    xmlNodeSetSpacePreserve(rxn->node, 0);

  return(Qnil);
}


/*
 * call-seq:
 *    node.text? => (true|false)
 * 
 * Determine whether this node has text.
 */
VALUE
ruby_xml_node_text_q(VALUE self) {
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);
  if (rxn->node == NULL)
    return(Qnil);

  return((xmlNodeIsText(rxn->node) == 1) ? Qtrue : Qfalse);
}


/*
 * call-seq:
 *    node.to_s => "string"
 * 
 * Coerce this node to a string representation of
 * it's XML.
 */
VALUE
ruby_xml_node_to_s(VALUE self) {
  ruby_xml_node *rxn;
  xmlBufferPtr buf;
  VALUE result;

  Data_Get_Struct(self, ruby_xml_node, rxn);
  buf = xmlBufferCreate();
  xmlNodeDump(buf, rxn->node->doc, rxn->node, 0, 1);
  result = rb_str_new2((const char*)buf->content);
  
  xmlBufferFree(buf);
  return result;
}


/*
 * call-seq:
 *    node.type => num
 * 
 * Obtain this node's type identifier.
 */
VALUE
ruby_xml_node_type(VALUE self) {
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);
  return(INT2NUM(rxn->node->type));
}


/*
 * call-seq:
 *    node.type_name => num
 * 
 * Obtain this node's type name.
 */
VALUE
ruby_xml_node_type_name(VALUE self) {
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);

  switch(rxn->node->type) {
  case XML_ELEMENT_NODE:
    return(rb_str_new2("element"));
  case XML_ATTRIBUTE_NODE:
    return(rb_str_new2("attribute"));
  case XML_TEXT_NODE:
    return(rb_str_new2("text"));
  case XML_CDATA_SECTION_NODE:
    return(rb_str_new2("cdata"));
  case XML_ENTITY_REF_NODE:
    return(rb_str_new2("entity_ref"));
  case XML_ENTITY_NODE:
    return(rb_str_new2("entity"));
  case XML_PI_NODE:
    return(rb_str_new2("pi"));
  case XML_COMMENT_NODE:
    return(rb_str_new2("comment"));
  case XML_DOCUMENT_NODE:
    return(rb_str_new2("document_xml"));
  case XML_DOCUMENT_TYPE_NODE:
    return(rb_str_new2("doctype"));
  case XML_DOCUMENT_FRAG_NODE:
    return(rb_str_new2("fragment"));
  case XML_NOTATION_NODE:
    return(rb_str_new2("notation"));
  case XML_HTML_DOCUMENT_NODE:
    return(rb_str_new2("document_html"));
  case XML_DTD_NODE:
    return(rb_str_new2("dtd"));
  case XML_ELEMENT_DECL:
    return(rb_str_new2("elem_decl"));
  case XML_ATTRIBUTE_DECL:
    return(rb_str_new2("attribute_decl"));
  case XML_ENTITY_DECL:
    return(rb_str_new2("entity_decl"));
  case XML_NAMESPACE_DECL:
    return(rb_str_new2("namespace"));
  case XML_XINCLUDE_START:
    return(rb_str_new2("xinclude_start"));
  case XML_XINCLUDE_END:
    return(rb_str_new2("xinclude_end"));
#ifdef LIBXML_DOCB_ENABLED
  case XML_DOCB_DOCUMENT_NODE:
    return(rb_str_new2("document_docbook"));
#endif
  default:
    rb_raise(eXMLNodeUnknownType, "Unknown node type: %n", rxn->node->type);
    return(Qfalse);
  }
}


/*
 * call-seq:
 *    node.xinclude_end? => num
 * 
 * Determine whether this node is an xinclude end node.
 */
VALUE
ruby_xml_node_xinclude_end_q(VALUE self) {
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);
  if (rxn->node->type == XML_XINCLUDE_END)
    return(Qtrue);
  else
    return(Qfalse);
}


/*
 * call-seq:
 *    node.xinclude_start? => num
 * 
 * Determine whether this node is an xinclude start node.
 */
VALUE
ruby_xml_node_xinclude_start_q(VALUE self) {
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);
  if (rxn->node->type == XML_XINCLUDE_START)
    return(Qtrue);
  else
    return(Qfalse);
}


/*
 * call-seq:
 *    node.copy => node
 * 
 * Create a copy of this node.
 */
VALUE
ruby_xml_node_copy(VALUE self, VALUE deep) {
  ruby_xml_node *rxn;
  xmlNode *copy;
  VALUE obj;

  Data_Get_Struct(self, ruby_xml_node, rxn);
  copy = xmlCopyNode( rxn->node, ((deep==Qnil)||(deep==Qfalse))?0:1 );

  if (copy == NULL)
    return Qnil;

  obj=ruby_xml_node2_wrap(cXMLNode,copy);
  copy->_private = (void*) obj;
  return obj;
}


// Rdoc needs to know 
#ifdef RDOC_NEVER_DEFINED
  mXML = rb_define_module("XML");
#endif

void
ruby_init_xml_node(void) {
  VALUE singleton;
  
  cXMLNode = rb_define_class_under(mXML, "Node", rb_cObject);
  eXMLNodeSetNamespace = rb_define_class_under(cXMLNode, "SetNamespace", eXMLError);
  eXMLNodeFailedModify = rb_define_class_under(cXMLNode, "FailedModify", eXMLError);
  eXMLNodeUnknownType = rb_define_class_under(cXMLNode, "UnknownType", eXMLError);
  
  singleton = rb_singleton_class(cXMLNode);

  rb_define_const(cXMLNode, "SPACE_DEFAULT", INT2NUM(0));
  rb_define_const(cXMLNode, "SPACE_PRESERVE", INT2NUM(1));
  rb_define_const(cXMLNode, "SPACE_NOT_INHERIT", INT2NUM(-1));
  rb_define_const(cXMLNode, "XLINK_ACTUATE_AUTO", INT2NUM(1));
  rb_define_const(cXMLNode, "XLINK_ACTUATE_NONE", INT2NUM(0));
  rb_define_const(cXMLNode, "XLINK_ACTUATE_ONREQUEST", INT2NUM(2));
  rb_define_const(cXMLNode, "XLINK_SHOW_EMBED", INT2NUM(2));
  rb_define_const(cXMLNode, "XLINK_SHOW_NEW", INT2NUM(1));
  rb_define_const(cXMLNode, "XLINK_SHOW_NONE", INT2NUM(0));
  rb_define_const(cXMLNode, "XLINK_SHOW_REPLACE", INT2NUM(3));
  rb_define_const(cXMLNode, "XLINK_TYPE_EXTENDED", INT2NUM(2));
  rb_define_const(cXMLNode, "XLINK_TYPE_EXTENDED_SET", INT2NUM(3));
  rb_define_const(cXMLNode, "XLINK_TYPE_NONE", INT2NUM(0));
  rb_define_const(cXMLNode, "XLINK_TYPE_SIMPLE", INT2NUM(1));

  rb_define_singleton_method(cXMLNode, "new2", ruby_xml_node2_new_native, 2);
  rb_define_singleton_method(cXMLNode, "new", ruby_xml_node2_new_string_bc, -1);
  rb_define_singleton_method(cXMLNode, "new_cdata", ruby_xml_node_new_cdata, -1);
  rb_define_singleton_method(cXMLNode, "new_comment", ruby_xml_node_new_comment, -1); 
  
  rb_define_alias(singleton, "new_element", "new");
  
  rb_define_method(cXMLNode, "<<", ruby_xml_node_content_add, 1);
  rb_define_method(cXMLNode, "[]", ruby_xml_node_property_get, 1);
  rb_define_method(cXMLNode, "[]=", ruby_xml_node_property_set, 2);
  rb_define_method(cXMLNode, "attribute?", ruby_xml_node_attribute_q, 0);
  rb_define_method(cXMLNode, "attribute_decl?", ruby_xml_node_attribute_decl_q, 0);
  rb_define_method(cXMLNode, "base", ruby_xml_node_base_get, 0);
  rb_define_method(cXMLNode, "base=", ruby_xml_node_base_set, 1);
  rb_define_method(cXMLNode, "blank?", ruby_xml_node_empty_q, 0);
  rb_define_method(cXMLNode, "cdata?", ruby_xml_node_cdata_q, 0);
  rb_define_method(cXMLNode, "comment?", ruby_xml_node_comment_q, 0);
  rb_define_method(cXMLNode, "copy", ruby_xml_node_copy, 1);
  rb_define_method(cXMLNode, "child", ruby_xml_node_child_get, 0);
  rb_define_method(cXMLNode, "child?", ruby_xml_node_child_q, 0);
  rb_define_method(cXMLNode, "child=", ruby_xml_node_child_set, 1);
  rb_define_method(cXMLNode, "children", ruby_xml_node_child_get, 0);
  rb_define_method(cXMLNode, "children?", ruby_xml_node_child_q, 0);
  rb_define_method(cXMLNode, "content", ruby_xml_node_content_get, 0);
  rb_define_method(cXMLNode, "content=", ruby_xml_node_content_set, 1);
  rb_define_method(cXMLNode, "content_stripped", ruby_xml_node_content_stripped_get, 0);
  rb_define_method(cXMLNode, "doc", ruby_xml_node_doc, 0);
  rb_define_method(cXMLNode, "docbook_doc?", ruby_xml_node_docbook_doc_q, 0);
  rb_define_method(cXMLNode, "doctype?", ruby_xml_node_doctype_q, 0);
  rb_define_method(cXMLNode, "document?", ruby_xml_node_document_q, 0);
  rb_define_method(cXMLNode, "dtd?", ruby_xml_node_dtd_q, 0);
  rb_define_method(cXMLNode, "dump", ruby_xml_node_dump, 0);
  rb_define_method(cXMLNode, "debug_dump", ruby_xml_node_debug_dump, 0);
  rb_define_method(cXMLNode, "element?", ruby_xml_node_element_q, 0);
  rb_define_method(cXMLNode, "element_decl?", ruby_xml_node_element_decl_q, 0);
  rb_define_method(cXMLNode, "empty?", ruby_xml_node_empty_q, 0);
  rb_define_method(cXMLNode, "entity?", ruby_xml_node_entity_q, 0);
  rb_define_method(cXMLNode, "entity_ref?", ruby_xml_node_entity_ref_q, 0);
  rb_define_method(cXMLNode, "eql?", ruby_xml_node_eql_q, 1);
  rb_define_method(cXMLNode, "find", ruby_xml_node_find, -1);
  rb_define_method(cXMLNode, "find_first", ruby_xml_node_find_first, -1);
  rb_define_method(cXMLNode, "fragment?", ruby_xml_node_fragment_q, 0);
  rb_define_method(cXMLNode, "hash", ruby_xml_node_hash, 0);
  rb_define_method(cXMLNode, "html_doc?", ruby_xml_node_html_doc_q, 0);
  rb_define_method(cXMLNode, "lang", ruby_xml_node_lang_get, 0);
  rb_define_method(cXMLNode, "lang=", ruby_xml_node_lang_set, 1);
  rb_define_method(cXMLNode, "last", ruby_xml_node_last_get, 0);
  rb_define_method(cXMLNode, "last?", ruby_xml_node_last_q, 0);
  rb_define_method(cXMLNode, "line_num", ruby_xml_node_line_num, 0);
  rb_define_method(cXMLNode, "name", ruby_xml_node_name_get, 0);
  rb_define_method(cXMLNode, "name=", ruby_xml_node_name_set, 1);
  rb_define_method(cXMLNode, "namespace", ruby_xml_node_namespace_get, 0);
  rb_define_method(cXMLNode, "namespace_node", ruby_xml_node_namespace_get_node, 0);
  rb_define_method(cXMLNode, "namespace?", ruby_xml_node_namespace_q, 0);  
  rb_define_method(cXMLNode, "namespace=", ruby_xml_node_namespace_set, -1);  
  rb_define_method(cXMLNode, "next", ruby_xml_node_next_get, 0);
  rb_define_method(cXMLNode, "next?", ruby_xml_node_next_q, 0);
  rb_define_method(cXMLNode, "next=", ruby_xml_node_next_set, 1);
  rb_define_method(cXMLNode, "node_type", ruby_xml_node_type, 0);
  rb_define_method(cXMLNode, "node_type_name", ruby_xml_node_type_name, 0);
  rb_define_method(cXMLNode, "notation?", ruby_xml_node_notation_q, 0);
  rb_define_method(cXMLNode, "ns", ruby_xml_node_namespace_get, 0);
  rb_define_method(cXMLNode, "ns?", ruby_xml_node_ns_q, 0);
  rb_define_method(cXMLNode, "ns_def", ruby_xml_node_ns_def_get, 0);
  rb_define_method(cXMLNode, "ns_def?", ruby_xml_node_ns_def_q, 0);
  rb_define_method(cXMLNode, "parent", ruby_xml_node_parent_get, 0);
  rb_define_method(cXMLNode, "parent?", ruby_xml_node_parent_q, 0);
  rb_define_method(cXMLNode, "path", ruby_xml_node_path, 0);
  rb_define_method(cXMLNode, "pi?", ruby_xml_node_pi_q, 0);
  rb_define_method(cXMLNode, "pointer", ruby_xml_node_pointer, 1);
  rb_define_method(cXMLNode, "prev", ruby_xml_node_prev_get, 0);
  rb_define_method(cXMLNode, "prev?", ruby_xml_node_prev_q, 0);
  rb_define_method(cXMLNode, "prev=", ruby_xml_node_prev_set, 1);
  rb_define_method(cXMLNode, "property", ruby_xml_node_property_get, 1);
  rb_define_method(cXMLNode, "properties", ruby_xml_node_properties_get, 0);
  rb_define_method(cXMLNode, "properties?", ruby_xml_node_properties_q, 0);
  rb_define_method(cXMLNode, "remove!", ruby_xml_node_remove_ex, 0);
  rb_define_method(cXMLNode, "search_ns", ruby_xml_node_search_ns, 1);
  rb_define_method(cXMLNode, "search_href", ruby_xml_node_search_href, 1);
  rb_define_method(cXMLNode, "sibling=", ruby_xml_node_sibling_set, 1);
  rb_define_method(cXMLNode, "space_preserve", ruby_xml_node_space_preserve_get, 0);
  rb_define_method(cXMLNode, "space_preserve=", ruby_xml_node_space_preserve_set, 1);
  rb_define_method(cXMLNode, "text?", ruby_xml_node_text_q, 0);
  rb_define_method(cXMLNode, "to_s", ruby_xml_node_to_s, 0);
  rb_define_method(cXMLNode, "xinclude_end?", ruby_xml_node_xinclude_end_q, 0);
  rb_define_method(cXMLNode, "xinclude_start?", ruby_xml_node_xinclude_start_q, 0);
  rb_define_method(cXMLNode, "xlink?", ruby_xml_node_xlink_q, 0);
  rb_define_method(cXMLNode, "xlink_type", ruby_xml_node_xlink_type, 0);
  rb_define_method(cXMLNode, "xlink_type_name", ruby_xml_node_xlink_type_name, 0);

  rb_define_alias(cXMLNode, "==", "eql?");
}
