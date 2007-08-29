/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

#include "libxml.h"

/* Ruby's util.h has ruby_strdup */
#include "util.h"

#ifdef xmlMalloc
#undef xmlMalloc
#endif
#ifdef xmlRealloc
#undef xmlRealloc
#endif
#ifdef xmlMemStrdup
#undef xmlMemStrdup
#endif
#ifdef xmlMemFree
#undef xmlMemFree
#endif

#ifdef RubyMemMalloc
#undef RubyMemMalloc
#endif
#ifdef RubyMemRealloc
#undef RubyMemRealloc
#endif
#ifdef RubyMemStrdup
#undef RubyMemStrdup
#endif
#ifdef RubyMemFree
#undef RubyMemFree
#endif

#define RubyMemFree     ruby_xfree
#define RubyMemRealloc  ruby_xrealloc
#define RubyMemMalloc   ruby_xmalloc
#define RubyMemStrdup   ruby_strdup

VALUE mXML;
VALUE eXMLError;

static xmlFreeFunc freeFunc = NULL;
static xmlMallocFunc mallocFunc = NULL;
static xmlReallocFunc reallocFunc = NULL;
static xmlStrdupFunc strdupFunc = NULL;

void
Init_libxml_so(void) {
  /* Some libxml memory goo that should be done before anything else */
  xmlMemGet((xmlFreeFunc *) & freeFunc,
            (xmlMallocFunc *) & mallocFunc,
            (xmlReallocFunc *) & reallocFunc,
            (xmlStrdupFunc *) & strdupFunc);

  if (xmlMemSetup((xmlFreeFunc)RubyMemFree, (xmlMallocFunc)RubyMemMalloc,
                  (xmlReallocFunc)RubyMemRealloc, (xmlStrdupFunc)RubyMemStrdup) != 0)
    rb_fatal("could not install the memory handlers for libxml");
  xmlInitParser();

  mXML = rb_define_module("XML");
  eXMLError = rb_define_class_under(mXML, "Error", rb_eRuntimeError);

  rb_define_const(mXML, "XML_NAMESPACE", rb_str_new2((const char*)XML_XML_NAMESPACE));

  ruby_init_parser();  
  ruby_init_xml_parser_context();
  ruby_init_xml_attr();
  ruby_init_xml_attribute();
  ruby_init_xml_document();
  ruby_init_xml_node();
  ruby_init_xml_node_set();
  ruby_init_xml_ns();
  ruby_init_xml_sax_parser();
  ruby_init_xml_tree();
  ruby_init_xml_xinclude();
  ruby_init_xml_xpath();
  ruby_init_xml_xpath_context();
  ruby_init_xml_xpointer();
  ruby_init_xml_xpointer_context();
  ruby_init_html_parser();
  ruby_init_input_callbacks(); 
  ruby_init_xml_dtd();         
  ruby_init_xml_schema();      
  ruby_init_xml_reader();      

  ruby_xml_parser_default_substitute_entities_set(cXMLParser, Qtrue);
  ruby_xml_parser_default_load_external_dtd_set(cXMLParser, Qtrue);
}
