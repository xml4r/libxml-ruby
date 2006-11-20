/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

/*
 * SAX CALLBACK HANDLERS
 */ 
static void internal_subset_func(ruby_xml_sax_parser *cbp, 
       const char *name,
       const char *extid,
       const char *sysid) {
  VALUE handler = cbp->callbackHandler;
  
  if (handler && handler != Qnil) {
    rb_funcall(handler, cbidOnInternalSubset, 3, rb_str_new2(name),
        rb_str_new2(extid), rb_str_new2(sysid));
  }    
}

static void is_standalone_func(ruby_xml_sax_parser *cbp) {
  VALUE handler = cbp->callbackHandler;
  
  if (handler && handler != Qnil) {
    rb_funcall(handler,cbidOnIsStandalone,0);
  }    
}

static void has_internal_subset_func(ruby_xml_sax_parser *cbp) {
  VALUE handler = cbp->callbackHandler;
  
  if (handler && handler != Qnil) {
    rb_funcall(handler,cbidOnHasInternalSubset,0);
  }    
}

static void has_external_subset_func(ruby_xml_sax_parser *cbp) {
  VALUE handler = cbp->callbackHandler;
  
  if (handler && handler != Qnil) {
    rb_funcall(handler,cbidOnHasExternalSubset,0);
  }    
}

static void start_document_func(ruby_xml_sax_parser *cbp) {
  VALUE handler = cbp->callbackHandler;
  
  if (handler && handler != Qnil) {
    rb_funcall(handler,cbidOnStartDocument,0);
  }    
}

static void end_document_func(ruby_xml_sax_parser *cbp) {
  VALUE handler = cbp->callbackHandler;
  
  if (handler && handler != Qnil) {
    rb_funcall(handler,cbidOnEndDocument,0);
  }    
}

static void start_element_func(ruby_xml_sax_parser *cbp, 
       const char *name, const char **attrs) {  
  VALUE handler = cbp->callbackHandler;
  VALUE ahsh = rb_hash_new();
  const char *attr, *value;
  
  if (attrs) {
    while ((attr = *(attrs++))) {
      value = *(attrs++);            
      rb_hash_aset(ahsh, rb_str_new2(attr), rb_str_new2(value));
    }
  }    
  
  if (handler && handler != Qnil) {
    rb_funcall(handler,cbidOnStartElement,2,rb_str_new2(name),ahsh);
  }    
}        

static void end_element_func(ruby_xml_sax_parser *cbp, 
       const char *name) {
  VALUE handler = cbp->callbackHandler;
  
  if (handler && handler != Qnil) {
    rb_funcall(handler,cbidOnEndElement,1,rb_str_new2(name));
  }    
}

static void reference_func(ruby_xml_sax_parser *cbp, 
       const char *name) {
  VALUE handler = cbp->callbackHandler;
  
  if (handler && handler != Qnil) {
    rb_funcall(handler,cbidOnReference,1,rb_str_new2(name));
  }    
}

static void characters_func(ruby_xml_sax_parser *cbp, 
       const char *chars, int len) {
  VALUE handler = cbp->callbackHandler;
  
  if (handler && handler != Qnil) {
    rb_funcall(handler,cbidOnCharacters,1,rb_str_new(chars, len));
  }    
}

static void processing_instruction_func(ruby_xml_sax_parser *cbp, 
       const char *target, const char *data) {
  VALUE handler = cbp->callbackHandler;
  
  if (handler && handler != Qnil) {
    rb_funcall(handler, cbidOnProcessingInstruction, 2,
        rb_str_new2(target),rb_str_new2(data));
  }    
}

static void comment_func(ruby_xml_sax_parser *cbp, 
       const char *msg) {
  VALUE handler = cbp->callbackHandler;
  
  if (handler && handler != Qnil) {
    rb_funcall(handler,cbidOnComment,1,rb_str_new2(msg));
  }    
}

// TODO these next three should actually be formatting messages.
static void warning_func(ruby_xml_sax_parser *cbp, 
       const char *msg, ...) {
  VALUE handler = cbp->callbackHandler;
  
  if (handler && handler != Qnil) {
    rb_funcall(handler,cbidOnXmlParserWarning,1,rb_str_new2(msg));
  }    
}

static void error_func(ruby_xml_sax_parser *cbp, 
       const char *msg, ...) {
  VALUE handler = cbp->callbackHandler;
  
  if (handler && handler != Qnil) {
    rb_funcall(handler,cbidOnXmlParserError,1,rb_str_new2(msg));
  }    
}

static void fatal_error_func(ruby_xml_sax_parser *cbp, 
       const char *msg, ...) {
  VALUE handler = cbp->callbackHandler;
  
  if (handler && handler != Qnil) {
    rb_funcall(handler,cbidOnXmlParserFatalError,1,rb_str_new2(msg));
  }    
}

static void cdata_block_func(ruby_xml_sax_parser *cbp, 
       const char *value, int len) {
  VALUE handler = cbp->callbackHandler;
  
  if (handler && handler != Qnil) {
    rb_funcall(handler,cbidOnCdataBlock,1,rb_str_new(value, len));
  }    
}

static void external_subset_func(ruby_xml_sax_parser *cbp, 
       const char *name,
       const char *extid,
       const char *sysid) {
  VALUE handler = cbp->callbackHandler;
  
  if (handler && handler != Qnil) {
    rb_funcall(handler, cbidOnExternalSubset, 3, rb_str_new2(name),
        rb_str_new2(extid), rb_str_new2(sysid));
  }    
}

static xmlSAXHandler rubySAXHandlerStruct = {
  (internalSubsetSAXFunc)internal_subset_func,
  (isStandaloneSAXFunc)is_standalone_func,
  (hasInternalSubsetSAXFunc)has_internal_subset_func,
  (hasExternalSubsetSAXFunc)has_external_subset_func,
  0, /* resolveEntity */
  0, /* getEntity */
  0, /* entityDecl */
  0, /* notationDecl */
  0, /* attributeDecl */
  0, /* elementDecl */
  0, /* unparsedEntityDecl */
  0, /* setDocumentLocator */
  (startDocumentSAXFunc)start_document_func, 
  (endDocumentSAXFunc)end_document_func, 
  (startElementSAXFunc)start_element_func, 
  (endElementSAXFunc)end_element_func, 
  (referenceSAXFunc)reference_func,
  (charactersSAXFunc)characters_func,
  0, /* ignorableWhitespace */
  (processingInstructionSAXFunc)processing_instruction_func,
  (commentSAXFunc)comment_func,
  (warningSAXFunc)warning_func,
  (errorSAXFunc)error_func,
  (fatalErrorSAXFunc)fatal_error_func,
  0, /* xmlGetParameterEntity */
  (cdataBlockSAXFunc)cdata_block_func,
  (externalSubsetSAXFunc)external_subset_func,
  1
};
