/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

#include "ruby_libxml.h"
#include "ruby_xml_node.h"

VALUE cXMLNode;

/* Document-class: LibXML::XML::Node
 *
 * Nodes are the primary objects that make up an XML document. 
 * The node class represents most node types that are found in
 * an XML document (but not Attributes, see LibXML::XML::Attribute).
 * It exposes libxml's full API for creating, querying
 * moving and deleting node objects.  Many of these methods are
 * documented in the DOM Level 3 specification found at:
 * http://www.w3.org/TR/DOM-Level-3-Core/. */


VALUE
check_string_or_symbol( VALUE val ) {
  if( TYPE(val) != T_STRING && TYPE(val) != T_SYMBOL ) {
    rb_raise(rb_eTypeError, "wrong argument type %s (expected String or Symbol)", 
    rb_obj_classname(val) );
  }
  return rb_obj_as_string( val );
}


/*
 * call-seq:
 *    node.base -> "uri"
 * 
 * Obtain this node's base URI.
 */
static VALUE
rxml_node_base_get(VALUE self) {
  xmlNodePtr xnode;
  xmlChar* base_uri;            
  VALUE result = Qnil;
  
  Data_Get_Struct(self, xmlNode, xnode);
  
  if (xnode->doc == NULL)
    return(result);
  
  base_uri = xmlNodeGetBase(xnode->doc, xnode);
  if (base_uri) {
    result = rb_str_new2((const char*)base_uri);
    xmlFree(base_uri);
  }
  
  return(result);
}


// TODO node_base_set should support setting back to nil

/*
 * call-seq:
 *    node.base = "uri"
 * 
 * Set this node's base URI.
 */
static VALUE
rxml_node_base_set(VALUE self, VALUE uri) {
  xmlNodePtr xnode;

  Check_Type(uri, T_STRING);
  Data_Get_Struct(self, xmlNode, xnode);
  if (xnode->doc == NULL)
    return(Qnil);

  xmlNodeSetBase(xnode, (xmlChar*)StringValuePtr(uri));
  return(Qtrue);
}


/*
 * call-seq:
 *    node.content -> "string"
 * 
 * Obtain this node's content as a string.
 */
static VALUE
rxml_node_content_get(VALUE self) {
  xmlNodePtr xnode;
  xmlChar *content;
  VALUE result = Qnil;
  
  Data_Get_Struct(self, xmlNode, xnode);
  content = xmlNodeGetContent(xnode);
  if (content) {
    result = rb_str_new2((const char *) content);
    xmlFree(content);
  }
 
  return result;
}

/*
 * call-seq:
 *    node.content = "string"
 * 
 * Set this node's content to the specified string.
 */ 
static VALUE
rxml_node_content_set(VALUE self, VALUE content) {
  xmlNodePtr xnode;

  Check_Type(content, T_STRING);
  Data_Get_Struct(self, xmlNode, xnode);
  // XXX docs indicate need for escaping entites, need to be done? danj
  xmlNodeSetContent(xnode, (xmlChar*)StringValuePtr(content));
  return(Qtrue);
}


/*
 * call-seq:
 *    node.content_stripped -> "string"
 * 
 * Obtain this node's stripped content.
 * 
 * *Deprecated*: Stripped content can be obtained via the
 * +content+ method.
 */
static VALUE
rxml_node_content_stripped_get(VALUE self) {
  xmlNodePtr xnode;
  xmlChar* content;
  VALUE result = Qnil;

  Data_Get_Struct(self, xmlNode, xnode);
  
  if (!xnode->content) 
    return result;
  
  content = xmlNodeGetContent(xnode);
  if (content) { 
    result = rb_str_new2((const char*)content);
    xmlFree(content);
  }
  return(result);
}

/*
 * call-seq:
 *    node.first -> XML::Node
 * 
 * Returns this node's first child node if any.
 */
static VALUE
rxml_node_first_get(VALUE self) {
  xmlNodePtr xnode;

  Data_Get_Struct(self, xmlNode, xnode);

  if (xnode->children)
    return(rxml_node2_wrap(cXMLNode, xnode->children));
  else
    return(Qnil);
}


/*
 * underlying for child_set and child_add, difference being
 * former raises on implicit copy, latter does not.
 */
static VALUE
rxml_node_child_set_aux(VALUE self, VALUE rnode) {
  xmlNodePtr pnode, chld, ret;

  if (rb_obj_is_kind_of(rnode, cXMLNode) == Qfalse)
    rb_raise(rb_eTypeError, "Must pass an XML::Node object");

  Data_Get_Struct(self,  xmlNode, pnode);
  Data_Get_Struct(rnode, xmlNode, chld);
  
  if ( chld->parent != NULL || chld->doc != NULL )
    rb_raise(rb_eRuntimeError, "Cannot move a node from one document to another with child= or <<.  First copy the node before moving it.");

  ret = xmlAddChild(pnode, chld);
  if (ret == NULL) {
    rxml_raise(&xmlLastError);
  } else if ( ret==chld ) {
    /* child was added whole to parent and we need to return it as a new object */
    return rxml_node2_wrap(cXMLNode,chld);
  }
  /* else */
  /* If it was a text node, then ret should be parent->last, so we will just return ret. */
  return rxml_node2_wrap(cXMLNode,ret);
}

/*
 * call-seq:
 *    node.child = node
 * 
 * Set a child node for this node. Also called for <<
 */
static VALUE
rxml_node_child_set(VALUE self, VALUE rnode) {
  return rxml_node_child_set_aux(self, rnode);
}


/*
 * call-seq:
 *    node << ("string" | node) -> XML::Node
 * 
 * Add the specified string or XML::Node to this node's
 * content.  The returned node is the node that was
 * added and not self, thereby allowing << calls to
 * be chained.
 */
static VALUE
rxml_node_content_add(VALUE self, VALUE obj) {
  xmlNodePtr xnode;
  VALUE str;

  Data_Get_Struct(self, xmlNode, xnode);
  /* XXX This should only be legal for a CDATA type node, I think,
   * resulting in a merge of content, as if a string were passed
   * danj 070827
   */
  if (rb_obj_is_kind_of(obj, cXMLNode)) {
    rxml_node_child_set(self, obj);
  } else {
    str = rb_obj_as_string(obj);
    if (NIL_P(str) || TYPE(str) != T_STRING)
      rb_raise(rb_eTypeError, "invalid argument: must be string or XML::Node");

		xmlNodeAddContent(xnode, (xmlChar*)StringValuePtr(str));
  }
  return(self);
}

/*
 * call-seq:
 *    node.child_add(node)
 * 
 * Set a child node for this node.
 */
static VALUE
rxml_node_child_add(VALUE self, VALUE rnode) {
  return rxml_node_child_set_aux(self, rnode);
}

/*
 * call-seq:
 *    node.doc -> document
 * 
 * Obtain the XML::Document this node belongs to.
 */
static VALUE
rxml_node_doc(VALUE self) {
  xmlNodePtr xnode;
  xmlDocPtr doc=NULL;

  Data_Get_Struct(self, xmlNode, xnode);
  
  switch (xnode->type) {
  case XML_DOCUMENT_NODE:
#ifdef LIBXML_DOCB_ENABLED
  case XML_DOCB_DOCUMENT_NODE:
#endif
  case XML_HTML_DOCUMENT_NODE:
    doc = NULL;
    break;
  case XML_ATTRIBUTE_NODE:
    {
      xmlAttrPtr attr = (xmlAttrPtr) xnode;
      doc = attr->doc;
      break;
    }
  case XML_NAMESPACE_DECL:
    doc = NULL;
    break;
  default:
    doc = xnode->doc;
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
 *    node.dump -> (true|nil)
 * 
 * Dump this node to stdout.
 */
static VALUE
rxml_node_dump(VALUE self) {
  xmlNodePtr xnode;
  xmlBufferPtr buf;

  Data_Get_Struct(self, xmlNode, xnode);

  if (xnode->doc == NULL)
    return(Qnil);

  buf = xmlBufferCreate();
  xmlNodeDump(buf, xnode->doc, xnode, 0, 1);
  xmlBufferDump(stdout, buf);
  xmlBufferFree(buf);
  return(Qtrue);
}


/*
 * call-seq:
 *    node.debug_dump -> (true|nil)
 * 
 * Dump this node to stdout, including any debugging
 * information.
 */
static VALUE
rxml_node_debug_dump(VALUE self) {
  xmlNodePtr xnode;
  Data_Get_Struct(self, xmlNode, xnode);

  if (xnode->doc == NULL)
    return(Qnil);

  xmlElemDump(stdout, xnode->doc, xnode);
  return(Qtrue);
}

/*
 * call-seq:
 *    node.each -> XML::Node
 * 
 * Iterates over this node's children, including text 
 * nodes, element nodes, etc.  If you wish to iterate
 * only over child elements, use XML::Node#each_element.
 *
 *  doc = XML::Document.new('model/books.xml')
 *  doc.root.each {|node| puts node}
 */
static VALUE
rxml_node_each(VALUE self) {
  xmlNodePtr xnode;
  xmlNodePtr xchild;
  Data_Get_Struct(self, xmlNode, xnode);

  xchild = xnode->children;
  
  while (xchild) {
    rb_yield(rxml_node2_wrap(cXMLNode, xchild));
    xchild = xchild->next;
  }
  return Qnil;
}


/*
 * call-seq:
 *    node.empty? -> (true|false)
 * 
 * Determine whether this node is empty.
 */
static VALUE
rxml_node_empty_q(VALUE self) {
  xmlNodePtr xnode;
  Data_Get_Struct(self, xmlNode, xnode);
  if (xnode == NULL)
    return(Qnil);

  return((xmlIsBlankNode(xnode) == 1) ? Qtrue : Qfalse);
}


static VALUE rxml_node_to_s(VALUE self);

/*
 * call-seq:
 *    node.eql?(other_node) => (true|false)
 * 
 * Test equality between the two nodes. Two nodes are equal
 * if they are the same node or have the same XML representation.*/
static VALUE
rxml_node_eql_q(VALUE self, VALUE other) {
  if (self == other)
  {
    return Qtrue;
  }
  else if NIL_P(other)
  {
    return Qfalse;
  }
  else      
  {
    VALUE self_xml;
    VALUE other_xml;
    
    if (rb_obj_is_kind_of(other, cXMLNode) == Qfalse)
      rb_raise(rb_eTypeError, "Nodes can only be compared against other nodes");
    
    self_xml = rxml_node_to_s(self);
    other_xml = rxml_node_to_s(other);
    return(rb_funcall(self_xml, rb_intern("=="), 1, other_xml));
  }    
}

/*
 * call-seq:
 *    XML::Node.new_cdata(content = nil) -> XML::Node
 * 
 * Create a new #CDATA node, optionally setting
 * the node's content.
 */
static VALUE
rxml_node_new_cdata(int argc, VALUE *argv, VALUE class) {
  xmlNodePtr xnode;
  VALUE str=Qnil;

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
      
    return rxml_node2_wrap(class,xnode);

  default:
    rb_raise(rb_eArgError, "wrong number of arguments (1)");
  }

  // not reached
  return(Qnil);
}


/*
 * call-seq:
 *    XML::Node.new_comment(content = nil) -> XML::Node
 * 
 * Create a new comment node, optionally setting
 * the node's content.
 * 
 */
static VALUE
rxml_node_new_comment(int argc, VALUE *argv, VALUE class) {
  xmlNodePtr xnode;
  VALUE str=Qnil;

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

    return rxml_node2_wrap(class,xnode);

  default:
    rb_raise(rb_eArgError, "wrong number of arguments (1)");
  }

  // not reached
  return(Qnil);
}

 
/*
 * call-seq:
 *    node.lang -> "string"
 * 
 * Obtain the language set for this node, if any.
 * This is set in XML via the xml:lang attribute.
 */
static VALUE
rxml_node_lang_get(VALUE self) {
  xmlNodePtr xnode;
  xmlChar *lang;
  VALUE result = Qnil;

  Data_Get_Struct(self, xmlNode, xnode);
  lang = xmlNodeGetLang(xnode);

  if (lang) {
    result = rb_str_new2((const char*)lang);
    xmlFree(lang);
  }
    
  return(result);
}


// TODO node_lang_set should support setting back to nil

/*
 * call-seq:
 *    node.lang = "string"
 * 
 * Set the language for this node. This affects the value
 * of the xml:lang attribute.
 */
static VALUE
rxml_node_lang_set(VALUE self, VALUE lang) {
  xmlNodePtr xnode;

  Check_Type(lang, T_STRING);
  Data_Get_Struct(self, xmlNode, xnode);
  xmlNodeSetLang(xnode, (xmlChar*)StringValuePtr(lang));

  return(Qtrue);
}


/*
 * call-seq:
 *    node.last -> XML::Node
 * 
 * Obtain the last child node of this node, if any.
 */
static VALUE
rxml_node_last_get(VALUE self) {
  xmlNodePtr xnode;

  Data_Get_Struct(self, xmlNode, xnode);

  if (xnode->last)
    return(rxml_node2_wrap(cXMLNode, xnode->last));
  else
    return(Qnil);
}

/*
 * call-seq:
 *    node.line_num -> num
 * 
 * Obtain the line number (in the XML document) that this
 * node was read from. If +default_line_numbers+ is set
 * false (the default), this method returns zero.
 */
static VALUE
rxml_node_line_num(VALUE self) {
  xmlNodePtr xnode;
  long line_num;
  Data_Get_Struct(self, xmlNode, xnode);

  if (!xmlLineNumbersDefaultValue)
    rb_warn("Line numbers were not retained: use XML::Parser::default_line_numbers=true");

  line_num = xmlGetLineNo(xnode);
  if (line_num == -1)
    return(Qnil);
  else
    return(INT2NUM((long)line_num));
}


/*
 * call-seq:
 *    node.xlink? -> (true|false)
 * 
 * Determine whether this node is an xlink node.
 */
static VALUE
rxml_node_xlink_q(VALUE self) {
  xmlNodePtr xnode;
  xlinkType xlt;

  Data_Get_Struct(self, xmlNode, xnode);
  xlt = xlinkIsLink(xnode->doc, xnode);

  if (xlt == XLINK_TYPE_NONE)
    return(Qfalse);
  else
    return(Qtrue);
}


/*
 * call-seq:
 *    node.xlink_type -> num
 * 
 * Obtain the type identifier for this xlink, if applicable.
 * If this is not an xlink node (see +xlink?+), will return
 * nil.
 */
static VALUE
rxml_node_xlink_type(VALUE self) {
  xmlNodePtr xnode;
  xlinkType xlt;

  Data_Get_Struct(self, xmlNode, xnode);
  xlt = xlinkIsLink(xnode->doc, xnode);

  if (xlt == XLINK_TYPE_NONE)
    return(Qnil);
  else
    return(INT2NUM(xlt));
}


/*
 * call-seq:
 *    node.xlink_type_name -> "string"
 * 
 * Obtain the type name for this xlink, if applicable.
 * If this is not an xlink node (see +xlink?+), will return
 * nil.
 */
static VALUE
rxml_node_xlink_type_name(VALUE self) {
  xmlNodePtr xnode;
  xlinkType xlt;

  Data_Get_Struct(self, xmlNode, xnode);
  xlt = xlinkIsLink(xnode->doc, xnode);

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
 *    node.name -> "string"
 * 
 * Obtain this node's name.
 */
static VALUE
rxml_node_name_get(VALUE self) {
  xmlNodePtr xnode;
  const xmlChar *name;

  Data_Get_Struct(self, xmlNode, xnode);

  switch (xnode->type) {
  case XML_DOCUMENT_NODE:
#ifdef LIBXML_DOCB_ENABLED
  case XML_DOCB_DOCUMENT_NODE:
#endif
  case XML_HTML_DOCUMENT_NODE:
    {
      xmlDocPtr doc = (xmlDocPtr) xnode;
      name = doc->URL;
      break;
    }
  case XML_ATTRIBUTE_NODE:
    {
      xmlAttrPtr attr = (xmlAttrPtr) xnode;
      name = attr->name;
      break;
    }
  case XML_NAMESPACE_DECL:
    {
      xmlNsPtr ns = (xmlNsPtr) xnode;
      name = ns->prefix;
      break;
    }
  default:
    name = xnode->name;
    break;
  }

  if (xnode->name == NULL)
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
static VALUE
rxml_node_name_set(VALUE self, VALUE name) {
  xmlNodePtr xnode;

  Check_Type(name, T_STRING);
  Data_Get_Struct(self, xmlNode, xnode);
  xmlNodeSetName(xnode, (xmlChar*)StringValuePtr(name));
  return(Qtrue);
}


/*
 * call-seq:
 *    node.namespace -> [namespace, ..., namespace]
 * 
 * Obtain an array of +XML::NS+ objects representing
 * this node's xmlns attributes
 */
static VALUE
rxml_node_namespace_get(VALUE self) {
  xmlNodePtr xnode;
  xmlNsPtr *nsList, *cur;
  VALUE arr, ns;

  Data_Get_Struct(self, xmlNode, xnode);
  if (xnode == NULL)
    return(Qnil);

  nsList = xmlGetNsList(xnode->doc, xnode);

  if (nsList == NULL)
    return(Qnil);

  arr = rb_ary_new();
  for (cur = nsList; *cur != NULL; cur++) {
    ns = rxml_ns_wrap(*cur);
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
 *    node.namespace_node -> namespace.
 * 
 * Obtain this node's namespace node.
 */
static VALUE
rxml_node_namespace_get_node(VALUE self) {
  xmlNodePtr xnode;

  Data_Get_Struct(self, xmlNode, xnode);
  if (xnode->ns == NULL)
    return(Qnil);
  else
    return rxml_ns_wrap(xnode->ns);
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
static VALUE
rxml_node_namespace_set(int argc, VALUE *argv, VALUE self) {
  VALUE rns, rprefix;
  xmlNodePtr xnode;
  xmlNsPtr xns;
  char *cp, *href;

  Data_Get_Struct(self, xmlNode, xnode);
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
      Data_Get_Struct(self, xmlNs, xns);
      xmlSetNs(xnode, xns);
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

    xns = xmlNewNs(xnode, (xmlChar*)href, (xmlChar*)StringValuePtr(rprefix));
    if (xns == NULL)
      rxml_raise(&xmlLastError);
    else
      return rxml_ns_wrap(xns);
    break;

  default:
    rb_raise(rb_eArgError, "wrong number of arguments (1 or 2)");
  }

  /* can't get here */
  return(Qnil);
}


/*
 * memory2 implementation: xmlNode->_private holds a reference
 * to the wrapping ruby object VALUE when there is one.
 * traversal for marking is upward, and top levels are marked
 * through and lower level mark entry.
 *
 * All ruby retrieval for an xml
 * node will result in the same ruby instance. When all handles to them
 * go out of scope, then ruby_xfree gets called and _private is set to NULL.
 * If the xmlNode has no parent or document, then call xmlFree.
 */
void
rxml_node2_free(xmlNodePtr xnode) {

  if (xnode != NULL) {
    xnode->_private=NULL;

    if (xnode->doc==NULL && xnode->parent==NULL) {
      xmlFreeNode(xnode);
    }
  }
}

void
rxml_node_mark_common(xmlNodePtr xnode) {
  if (xnode->parent == NULL ) {
#ifdef NODE_DEBUG
    fprintf(stderr,"mark no parent r=0x%x *n=0x%x\n",rxn,node);
#endif
  } else if (xnode->doc != NULL ) {
    if (xnode->doc->_private == NULL) {
      rb_bug("XmlNode Doc is not bound! (%s:%d)",
	     __FILE__,__LINE__);
    }
    rb_gc_mark((VALUE)xnode->doc->_private);
  } else {
    while (xnode->parent != NULL )
      xnode = xnode->parent;
    if (xnode->_private == NULL )
      rb_warning("XmlNode Root Parent is not bound! (%s:%d)",
		 __FILE__,__LINE__);
    else {
      rb_gc_mark((VALUE)xnode->_private);
    }
  }
}

void
rxml_node2_mark(xmlNodePtr xnode) {
  if (xnode == NULL ) return;

  if (xnode->_private == NULL ) {
    rb_warning("XmlNode is not bound! (%s:%d)",
	       __FILE__,__LINE__);
    return;
  }

  rxml_node_mark_common(xnode);
}

VALUE
rxml_node2_wrap(VALUE class, xmlNodePtr xnode)
{
  VALUE obj;

  // This node is already wrapped
  if (xnode->_private != NULL) {
    return (VALUE)xnode->_private;
  }

  obj=Data_Wrap_Struct(class,
                       rxml_node2_mark, rxml_node2_free,
                       xnode);

  xnode->_private=(void*)obj;
  return obj;
}

/*
 * call-seq:
 *    XML::Node.new_string(namespace, name, content) -> XML::Node
 * 
 * Create a new element node in the specified namespace with the 
 * supplied name and content.
 */
static VALUE
rxml_node_new_string(VALUE klass, VALUE ns, VALUE name, VALUE val)
{
  xmlNodePtr xnode;
  xmlNsPtr xns = NULL;
  VALUE node;

  if (!NIL_P(ns)) {
    Data_Get_Struct(ns, xmlNs, xns);
  }
  
  xnode = xmlNewNode(xns,(xmlChar*)StringValuePtr(name));
  xnode->_private=NULL;

  node = rxml_node2_wrap(klass, xnode);
  rb_obj_call_init(node, 0, NULL);
  
  if (!NIL_P(val))
  {
    if (TYPE(val) != T_STRING)
      val = rb_obj_as_string(val);

    rxml_node_content_set(node, val);
  }
  return node;
}

/*
 * call-seq:
 *    XML::Node.new(name, content = nil) -> XML::Node
 *    XML::Node.new_element(name, content = nil) -> XML::Node
 * 
 * Create a new element node with the specified name, optionally setting
 * the node's content.
 * backward compatibility for <.5 new
 */
static VALUE
rxml_node_new_string_bc(int argc, VALUE *argv, VALUE class)
{
  VALUE content=Qnil;
  VALUE name=Qnil;
  switch(argc) {
  case 2:
    content=argv[1];
    if ( TYPE(content) != T_STRING)
      content=rb_obj_as_string(content);
    
  case 1:
    name=check_string_or_symbol( argv[0] );
    return rxml_node_new_string(class,Qnil,name,content);

  default:
    rb_raise(rb_eArgError, "wrong number of arguments (1 or 2) given %d",argc);
  }
}

/*
 * call-seq:
 *    node.next -> XML::Node
 * 
 * Obtain the next sibling node, if any.
 */
static VALUE
rxml_node_next_get(VALUE self) {
  xmlNodePtr xnode;

  Data_Get_Struct(self, xmlNode, xnode);

  if (xnode->next)
    return(rxml_node2_wrap(cXMLNode, xnode->next));
  else
    return(Qnil);
}


/*
 * call-seq:
 *    node.next = node
 * 
 * Insert the specified node as this node's next sibling.
 */
static VALUE
rxml_node_next_set(VALUE self, VALUE rnode) {
  xmlNodePtr cnode, pnode, ret;

  if (rb_obj_is_kind_of(rnode, cXMLNode) == Qfalse)
    rb_raise(rb_eTypeError, "Must pass an XML::Node object");

  Data_Get_Struct(self,  xmlNode, pnode);
  Data_Get_Struct(rnode, xmlNode, cnode);

  ret = xmlAddNextSibling(pnode, cnode);
  if (ret == NULL)
    rxml_raise(&xmlLastError);

  return(rxml_node2_wrap(cXMLNode, ret));
}


/*
 * call-seq:
 *    node.ns? -> (true|false)
 * 
 * Determine whether this node has a namespace.
 */
static VALUE
rxml_node_ns_q(VALUE self) {
  xmlNodePtr xnode;
  Data_Get_Struct(self, xmlNode, xnode);
  if (xnode->ns == NULL)
    return(Qfalse);
  else
    return(Qtrue);
}

/*
 * call-seq:
 *    node.ns_def? -> (true|false)
 * 
 * Obtain an array of +XML::NS+ objects representing
 * this node's xmlns attributes
 */
static VALUE
rxml_node_ns_def_q(VALUE self) {
  xmlNodePtr xnode;
  Data_Get_Struct(self, xmlNode, xnode);
  if (xnode->nsDef == NULL)
    return(Qfalse);
  else
    return(Qtrue);
}


/*
 * call-seq:
 *    node.ns_def -> namespace
 * 
 * Obtain this node's default namespace.
 */
static VALUE
rxml_node_ns_def_get(VALUE self) {
  xmlNodePtr xnode;
  Data_Get_Struct(self, xmlNode, xnode);
  if (xnode->nsDef == NULL)
    return(Qnil);
  else
    return(rxml_ns_wrap(xnode->nsDef));
}


/*
 * call-seq:
 *    node.parent -> XML::Node
 * 
 * Obtain this node's parent node, if any.
 */
static VALUE
rxml_node_parent_get(VALUE self) {
  xmlNodePtr xnode;

  Data_Get_Struct(self, xmlNode, xnode);

  if (xnode->parent)
    return(rxml_node2_wrap(cXMLNode, xnode->parent));
  else
    return(Qnil);
}


/*
 * call-seq:
 *    node.path -> path
 * 
 * Obtain this node's path.
 */
static VALUE
rxml_node_path(VALUE self) {
  xmlNodePtr xnode;
  xmlChar *path;

  Data_Get_Struct(self, xmlNode, xnode);
  path = xmlGetNodePath(xnode);

  if (path == NULL)
    return(Qnil);
  else
    return(rb_str_new2((const char*)path));
}


/*
 * call-seq:
 *    node.pointer -> XML::NodeSet
 * 
 * Evaluates an XPointer expression relative to this node.
 */
static VALUE
rxml_node_pointer(VALUE self, VALUE xptr_str) {
  return(rxml_xpointer_point2(self, xptr_str));
}


/*
 * call-seq:
 *    node.prev -> XML::Node
 * 
 * Obtain the previous sibling, if any.
 */
static VALUE
rxml_node_prev_get(VALUE self) {
  xmlNodePtr xnode;
  xmlNodePtr node;
  Data_Get_Struct(self, xmlNode, xnode);

  switch (xnode->type) {
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
      xmlAttrPtr attr = (xmlAttrPtr) xnode;
      node = (xmlNodePtr) attr->prev;
    }
    break;
  default:
    node = xnode->prev;
    break;
  }

  if (node == NULL)
    return(Qnil);
  else
    return(rxml_node2_wrap(cXMLNode, node));
}


/*
 * call-seq:
 *    node.prev = node
 * 
 * Insert the specified node as this node's previous sibling.
 */
static VALUE
rxml_node_prev_set(VALUE self, VALUE rnode) {
  xmlNodePtr cnode, pnode, ret;

  if (rb_obj_is_kind_of(rnode, cXMLNode) == Qfalse)
    rb_raise(rb_eTypeError, "Must pass an XML::Node object");

  Data_Get_Struct(self,  xmlNode, pnode);
  Data_Get_Struct(rnode, xmlNode, cnode);

  ret = xmlAddPrevSibling(pnode, cnode);
  if (ret == NULL)
    rxml_raise(&xmlLastError);

  return(rxml_node2_wrap(cXMLNode, ret));
}

/*
 * call-seq:
 *    node.attributes -> attributes
 * 
 * Returns the XML::Attributes for this node. 
 */
static VALUE
rxml_node_attributes_get(VALUE self) {
  xmlNodePtr xnode;

  Data_Get_Struct(self, xmlNode, xnode);
  return rxml_attributes_new(xnode);
}

/*
 * call-seq:
 *    node.property("name") -> "string"
 *    node["name"]          -> "string"
 * 
 * Obtain the named pyroperty.
 */
static VALUE
rxml_node_attribute_get(VALUE self, VALUE name) {
  VALUE attributes = rxml_node_attributes_get(self);
  return rxml_attributes_attribute_get(attributes, name);
}

/*
 * call-seq:
 *    node["name"] = "string"
 * 
 * Set the named property.
 */
static VALUE
rxml_node_property_set(VALUE self, VALUE name, VALUE value) {
  VALUE attributes = rxml_node_attributes_get(self);
  return rxml_attributes_attribute_set(attributes, name, value);
}


/*
 * call-seq:
*    node.remove! -> nil
*
* Removes this node from it's parent.
*/
static VALUE
rxml_node_remove_ex(VALUE self) {
  xmlNodePtr xnode;
  Data_Get_Struct(self, xmlNode, xnode);
  xmlUnlinkNode(xnode);
  return(Qnil);
}


/*
 * call-seq:
 *    node.search_href -> namespace
 * 
 * Search for a namespace by href.
 */
static VALUE
rxml_node_search_href(VALUE self, VALUE href) {
  xmlNodePtr xnode;

  Check_Type(href, T_STRING);
  Data_Get_Struct(self, xmlNode, xnode);
  return(rxml_ns_wrap(xmlSearchNsByHref(xnode->doc, xnode,
					    (xmlChar*)StringValuePtr(href))));
}


/*
 * call-seq:
 *    node.search_ns -> namespace
 * 
 * Search for a namespace by namespace.
 */
static VALUE
rxml_node_search_ns(VALUE self, VALUE ns) {
  xmlNodePtr xnode;

  Check_Type(ns, T_STRING);
  Data_Get_Struct(self, xmlNode, xnode);
  return(rxml_ns_wrap(xmlSearchNs(xnode->doc, xnode,
			              (xmlChar*)StringValuePtr(ns))));
}


/*
 * call-seq:
 *    node.sibling(node) -> XML::Node
 * 
 * Add the specified node as a sibling of this node.
 */
static VALUE
rxml_node_sibling_set(VALUE self, VALUE rnode) {
  xmlNodePtr cnode, pnode, ret;
  VALUE obj;

  if (rb_obj_is_kind_of(rnode, cXMLNode) == Qfalse)
    rb_raise(rb_eTypeError, "Must pass an XML::Node object");

  Data_Get_Struct(self,  xmlNode, pnode);
  Data_Get_Struct(rnode, xmlNode, cnode);

  ret = xmlAddSibling(pnode, cnode);
  if (ret == NULL)
    rxml_raise(&xmlLastError);

  if (ret->_private==NULL)
    obj=rxml_node2_wrap(cXMLNode,ret);
  else
    obj=(VALUE)ret->_private;

  return obj;
}


/*
 * call-seq:
 *    node.space_preserve -> (true|false)
 * 
 * Determine whether this node preserves whitespace.
 */
static VALUE
rxml_node_space_preserve_get(VALUE self) {
  xmlNodePtr xnode;

  Data_Get_Struct(self, xmlNode, xnode);
  return(INT2NUM(xmlNodeGetSpacePreserve(xnode)));
}


/*
 * call-seq:
 *    node.space_preserve = true|false
 * 
 * Control whether this node preserves whitespace.
 */
static VALUE
rxml_node_space_preserve_set(VALUE self, VALUE bool) {
  xmlNodePtr xnode;
  Data_Get_Struct(self, xmlNode, xnode);

  if (TYPE(bool) == T_FALSE)
    xmlNodeSetSpacePreserve(xnode, 1);
  else
    xmlNodeSetSpacePreserve(xnode, 0);

  return(Qnil);
}

/*
 * call-seq:
 *    node.to_s -> "string"
 * 
 * Coerce this node to a string representation of
 * it's XML.
 */
static VALUE
rxml_node_to_s(VALUE self) {
  xmlNodePtr xnode;
  xmlBufferPtr buf;
  VALUE result;

  Data_Get_Struct(self, xmlNode, xnode);
  buf = xmlBufferCreate();
  xmlNodeDump(buf, xnode->doc, xnode, 0, 1);
  result = rb_str_new2((const char*)buf->content);
  
  xmlBufferFree(buf);
  return result;
}


/*
 * call-seq:
 *    node.type -> num
 * 
 * Obtain this node's type identifier.
 */
static VALUE
rxml_node_type(VALUE self) {
  xmlNodePtr xnode;
  Data_Get_Struct(self, xmlNode, xnode);
  return(INT2NUM(xnode->type));
}




/*
 * call-seq:
 *    node.copy -> XML::Node
 * 
 * Creates a copy of this node.  To create a 
 * shallow copy set the deep parameter to false.
 * To create a deep copy set the deep parameter
 * to true. 
 *
 */
static VALUE
rxml_node_copy(VALUE self, VALUE deep) {
  xmlNodePtr xnode;
  xmlNodePtr xcopy;
  int recursive = (deep==Qnil || deep==Qfalse) ? 0 : 1;
  Data_Get_Struct(self, xmlNode, xnode);
  
  xcopy = xmlCopyNode(xnode, recursive);

  if (xcopy)
    return rxml_node2_wrap(cXMLNode, xcopy);
  else
    return Qnil;
}

 /*
 * call-seq: 
 *    XML::Node.new_text(content = nil) -> XML::Node
 * 
 * Create a new text node, optionally setting
 * the node's content.
 * 
 */
static VALUE
rxml_node_new_text(VALUE class, VALUE text)
{
  VALUE obj;
  xmlNodePtr xnode;
  
  if ( NIL_P(text) )
    return Qnil;

  if (TYPE(text) != T_STRING )
    rb_raise(rb_eTypeError, "requires string argument");
  
  xnode=xmlNewText((xmlChar*)STR2CSTR(text));
  if ( xnode == NULL )
    return Qnil;
  
  obj=rxml_node2_wrap(class,xnode);
  rb_obj_call_init(obj,0,NULL);
  return obj;
}

void
rxml_node_registerNode(xmlNodePtr node)
{
  node->_private=NULL;
}

void
rxml_node_deregisterNode(xmlNodePtr xnode)
{
  VALUE node;
  
  if (xnode->_private==NULL ) return;
  node = (VALUE)xnode->_private;
  DATA_PTR(node) = NULL;
}

// Rdoc needs to know 
#ifdef RDOC_NEVER_DEFINED
  mLibXML = rb_define_module("LibXML");
  mXML = rb_define_module_under(mLibXML, "XML");
#endif

void
ruby_init_xml_node(void) {
  xmlRegisterNodeDefault(rxml_node_registerNode);
  xmlDeregisterNodeDefault(rxml_node_deregisterNode);

  cXMLNode = rb_define_class_under(mXML, "Node", rb_cObject);
  
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
  
  rb_define_const(cXMLNode, "ELEMENT_NODE", INT2FIX(XML_ELEMENT_NODE));
  rb_define_const(cXMLNode, "ATTRIBUTE_NODE", INT2FIX(XML_ATTRIBUTE_NODE));
  rb_define_const(cXMLNode, "TEXT_NODE", INT2FIX(XML_TEXT_NODE));
  rb_define_const(cXMLNode, "CDATA_SECTION_NODE", INT2FIX(XML_CDATA_SECTION_NODE));
  rb_define_const(cXMLNode, "ENTITY_REF_NODE", INT2FIX(XML_ENTITY_REF_NODE));
  rb_define_const(cXMLNode, "ENTITY_NODE", INT2FIX(XML_ENTITY_NODE));
  rb_define_const(cXMLNode, "PI_NODE", INT2FIX(XML_PI_NODE));
  rb_define_const(cXMLNode, "COMMENT_NODE", INT2FIX(XML_COMMENT_NODE));
  rb_define_const(cXMLNode, "DOCUMENT_NODE", INT2FIX(XML_DOCUMENT_NODE));
  rb_define_const(cXMLNode, "DOCUMENT_TYPE_NODE", INT2FIX(XML_DOCUMENT_TYPE_NODE));
  rb_define_const(cXMLNode, "DOCUMENT_FRAG_NODE", INT2FIX(XML_DOCUMENT_FRAG_NODE));
  rb_define_const(cXMLNode, "NOTATION_NODE", INT2FIX(XML_NOTATION_NODE));
  rb_define_const(cXMLNode, "HTML_DOCUMENT_NODE", INT2FIX(XML_HTML_DOCUMENT_NODE));
  rb_define_const(cXMLNode, "DTD_NODE", INT2FIX(XML_DTD_NODE));
  rb_define_const(cXMLNode, "ELEMENT_DECL", INT2FIX(XML_ELEMENT_DECL));
  rb_define_const(cXMLNode, "ATTRIBUTE_DECL", INT2FIX(XML_ATTRIBUTE_DECL));
  rb_define_const(cXMLNode, "ENTITY_DECL", INT2FIX(XML_ENTITY_DECL));
  rb_define_const(cXMLNode, "NAMESPACE_DECL", INT2FIX(XML_NAMESPACE_DECL));
  rb_define_const(cXMLNode, "XINCLUDE_START", INT2FIX(XML_XINCLUDE_START));
  rb_define_const(cXMLNode, "XINCLUDE_END", INT2FIX(XML_XINCLUDE_END));

#ifdef LIBXML_DOCB_ENABLED
  rb_define_const(cXMLNode, "DOCB_DOCUMENT_NODE", INT2FIX(XML_DOCB_DOCUMENT_NODE));
#else
  rb_define_const(cXMLNode, "DOCB_DOCUMENT_NODE", Qnil);
#endif
  
  rb_define_singleton_method(cXMLNode, "new", rxml_node_new_string_bc, -1);
  rb_define_singleton_method(cXMLNode, "new_cdata", rxml_node_new_cdata, -1);
  rb_define_singleton_method(cXMLNode, "new_comment", rxml_node_new_comment, -1); 
  rb_define_singleton_method(cXMLNode, "new_text", rxml_node_new_text, 1); 
  
  /* Traversal */
  rb_include_module(cXMLNode, rb_mEnumerable);
  rb_define_method(cXMLNode, "[]", rxml_node_attribute_get, 1);
  rb_define_method(cXMLNode, "each", rxml_node_each, 0);
  rb_define_method(cXMLNode, "first", rxml_node_first_get, 0);
  rb_define_method(cXMLNode, "last", rxml_node_last_get, 0);
  rb_define_method(cXMLNode, "next", rxml_node_next_get, 0);
  rb_define_method(cXMLNode, "parent", rxml_node_parent_get, 0);
  rb_define_method(cXMLNode, "prev", rxml_node_prev_get, 0);

  /* Modification */
  rb_define_method(cXMLNode, "<<", rxml_node_content_add, 1);
  rb_define_method(cXMLNode, "[]=", rxml_node_property_set, 2);
  rb_define_method(cXMLNode, "child_add", rxml_node_child_add, 1);
  rb_define_method(cXMLNode, "child=", rxml_node_child_set, 1);
  rb_define_method(cXMLNode, "sibling=", rxml_node_sibling_set, 1);
  rb_define_method(cXMLNode, "next=", rxml_node_next_set, 1);
  rb_define_method(cXMLNode, "prev=", rxml_node_prev_set, 1);

 
  /* Rest of the node api */
  rb_define_method(cXMLNode, "attributes", rxml_node_attributes_get, 0);
  rb_define_method(cXMLNode, "base", rxml_node_base_get, 0);
  rb_define_method(cXMLNode, "base=", rxml_node_base_set, 1);
  rb_define_method(cXMLNode, "blank?", rxml_node_empty_q, 0);
  rb_define_method(cXMLNode, "copy", rxml_node_copy, 1);
  rb_define_method(cXMLNode, "content", rxml_node_content_get, 0);
  rb_define_method(cXMLNode, "content=", rxml_node_content_set, 1);
  rb_define_method(cXMLNode, "content_stripped", rxml_node_content_stripped_get, 0);
  rb_define_method(cXMLNode, "doc", rxml_node_doc, 0);
  rb_define_method(cXMLNode, "dump", rxml_node_dump, 0);
  rb_define_method(cXMLNode, "debug_dump", rxml_node_debug_dump, 0);
  rb_define_method(cXMLNode, "empty?", rxml_node_empty_q, 0);
  rb_define_method(cXMLNode, "eql?", rxml_node_eql_q, 1);
  rb_define_method(cXMLNode, "lang", rxml_node_lang_get, 0);
  rb_define_method(cXMLNode, "lang=", rxml_node_lang_set, 1);
  rb_define_method(cXMLNode, "line_num", rxml_node_line_num, 0);
  rb_define_method(cXMLNode, "name", rxml_node_name_get, 0);
  rb_define_method(cXMLNode, "name=", rxml_node_name_set, 1);
  rb_define_method(cXMLNode, "namespace", rxml_node_namespace_get, 0);
  rb_define_method(cXMLNode, "namespace_node", rxml_node_namespace_get_node, 0);
  rb_define_method(cXMLNode, "namespace=", rxml_node_namespace_set, -1);  
  rb_define_method(cXMLNode, "node_type", rxml_node_type, 0);
  rb_define_method(cXMLNode, "ns", rxml_node_namespace_get, 0);
  rb_define_method(cXMLNode, "ns?", rxml_node_ns_q, 0);
  rb_define_method(cXMLNode, "ns_def?", rxml_node_ns_def_q, 0);
  rb_define_method(cXMLNode, "ns_def", rxml_node_ns_def_get, 0);
  rb_define_method(cXMLNode, "path", rxml_node_path, 0);
  rb_define_method(cXMLNode, "pointer", rxml_node_pointer, 1);
  rb_define_method(cXMLNode, "remove!", rxml_node_remove_ex, 0);
  rb_define_method(cXMLNode, "search_ns", rxml_node_search_ns, 1);
  rb_define_method(cXMLNode, "search_href", rxml_node_search_href, 1);
  rb_define_method(cXMLNode, "space_preserve", rxml_node_space_preserve_get, 0);
  rb_define_method(cXMLNode, "space_preserve=", rxml_node_space_preserve_set, 1);
  rb_define_method(cXMLNode, "to_s", rxml_node_to_s, 0);
  rb_define_method(cXMLNode, "xlink?", rxml_node_xlink_q, 0);
  rb_define_method(cXMLNode, "xlink_type", rxml_node_xlink_type, 0);
  rb_define_method(cXMLNode, "xlink_type_name", rxml_node_xlink_type_name, 0);

  rb_define_alias(cXMLNode, "==", "eql?");
}
