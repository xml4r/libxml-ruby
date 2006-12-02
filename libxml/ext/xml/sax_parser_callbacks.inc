/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

/*
 * SAX CALLBACK HANDLERS
 */ 
static void internal_subset_func(ruby_xml_sax_parser_callbacks *cbp, 
       const char *name,
       const char *extid,
       const char *sysid) {
  VALUE handler = cbp->internalSubset;
  
  if (handler && handler != Qnil) {
    rb_funcall(handler, callsym, 3, rb_str_new2(name),
        rb_str_new2(extid), rb_str_new2(sysid));
  }    
}

static void is_standalone_func(ruby_xml_sax_parser_callbacks *cbp) {
  VALUE handler = cbp->isStandalone;
  
  if (handler && handler != Qnil) {
    rb_funcall(handler,callsym,0);
  }    
}

static void has_internal_subset_func(ruby_xml_sax_parser_callbacks *cbp) {
  VALUE handler = cbp->hasInternalSubset;
  
  if (handler && handler != Qnil) {
    rb_funcall(handler,callsym,0);
  }    
}

static void has_external_subset_func(ruby_xml_sax_parser_callbacks *cbp) {
  VALUE handler = cbp->hasExternalSubset;
  
  if (handler && handler != Qnil) {
    rb_funcall(handler,callsym,0);
  }    
}

static void start_document_func(ruby_xml_sax_parser_callbacks *cbp) {
  VALUE handler = cbp->startDocument;
  
  if (handler && handler != Qnil) {
    rb_funcall(handler,callsym,0);
  }    
}

static void end_document_func(ruby_xml_sax_parser_callbacks *cbp) {
  VALUE handler = cbp->endDocument;
  
  if (handler && handler != Qnil) {
    rb_funcall(handler,callsym,0);
  }    
}

static void start_element_func(ruby_xml_sax_parser_callbacks *cbp, 
       const char *name, const char **attrs) {  
  VALUE handler = cbp->startElement;
  VALUE ahsh = rb_hash_new();
  const char *attr, *value;
  
  if (attrs) {
    while ((attr = *(attrs++))) {
      value = *(attrs++);            
      rb_hash_aset(ahsh, rb_str_new2(attr), rb_str_new2(value));
    }
  }    
  
  if (handler && handler != Qnil) {
    rb_funcall(handler,callsym,2,rb_str_new2(name),ahsh);
  }    
}        

static void end_element_func(ruby_xml_sax_parser_callbacks *cbp, 
       const char *name) {
  VALUE handler = cbp->endElement;
  
  if (handler && handler != Qnil) {
    rb_funcall(handler,callsym,1,rb_str_new2(name));
  }    
}

static void reference_func(ruby_xml_sax_parser_callbacks *cbp, 
       const char *name) {
  VALUE handler = cbp->reference;
  
  if (handler && handler != Qnil) {
    rb_funcall(handler,callsym,1,rb_str_new2(name));
  }    
}

static void characters_func(ruby_xml_sax_parser_callbacks *cbp, 
       const char *chars, int len) {
  VALUE handler = cbp->characters;
  
  if (handler && handler != Qnil) {
    rb_funcall(handler,callsym,1,rb_str_new(chars, len));
  }    
}

static void processing_instruction_func(ruby_xml_sax_parser_callbacks *cbp, 
       const char *target, const char *data) {
  VALUE handler = cbp->processingInstruction;
  
  if (handler && handler != Qnil) {
    rb_funcall(handler, callsym, 2,
        rb_str_new2(target),rb_str_new2(data));
  }    
}

static void comment_func(ruby_xml_sax_parser_callbacks *cbp, 
       const char *msg) {
  VALUE handler = cbp->comment;
  
  if (handler && handler != Qnil) {
    rb_funcall(handler,callsym,1,rb_str_new2(msg));
  }    
}

#define FORMAT_AND_CALL() \
  do { \
    va_list args; \
    char buf[1024]; \
    va_start(args, msg); \
    vsnprintf(buf, sizeof buf, msg, args); \
    rb_funcall(handler,callsym,1,rb_str_new2(buf)); \
    va_end(args); \
  } \
  while (0)

static void warning_func(ruby_xml_sax_parser_callbacks *cbp, 
       const char *msg, ...) {
  VALUE handler = cbp->xmlParserWarning;
  
  if (handler && handler != Qnil) {
    FORMAT_AND_CALL();
  }    
}

static void error_func(ruby_xml_sax_parser_callbacks *cbp, 
       const char *msg, ...) {
  VALUE handler = cbp->xmlParserError;
  
  if (handler && handler != Qnil) {
    FORMAT_AND_CALL();
  }    
}

static void fatal_error_func(ruby_xml_sax_parser_callbacks *cbp, 
       const char *msg, ...) {
  VALUE handler = cbp->xmlParserFatalError;
  
  if (handler && handler != Qnil) {
    FORMAT_AND_CALL();
  }    
}

static void cdata_block_func(ruby_xml_sax_parser_callbacks *cbp, 
       const char *value, int len) {
  VALUE handler = cbp->cdataBlock;
  
  if (handler && handler != Qnil) {
    rb_funcall(handler,callsym,1,rb_str_new(value, len));
  }    
}

static void external_subset_func(ruby_xml_sax_parser_callbacks *cbp, 
       const char *name,
       const char *extid,
       const char *sysid) {
  VALUE handler = cbp->externalSubset;
  
  if (handler && handler != Qnil) {
    rb_funcall(handler, callsym, 3, rb_str_new2(name),
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
