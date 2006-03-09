/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

#ifndef __RUBY_LIBXSLT_H__
#define __RUBY_LIBXSLT_H__

#include <ruby.h>
#include <rubyio.h>
#include <libxml/parser.h>
#include <libxml/debugXML.h>
#include <libxslt/extra.h>
#include <libxslt/xslt.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>
#include <libexslt/exslt.h>

#include "libxml-ruby/libxml.h"
#include "libxml-ruby/ruby_xml_document.h"
#include "ruby_xslt_stylesheet.h"
#include "ruby_xslt_transform_context.h"

#define RUBY_LIBXSLT_VERSION  "0.3.6"
#define RUBY_LIBXSLT_VERNUM   036

#define RUBY_LIBXSLT_SRC_TYPE_NULL    0
#define RUBY_LIBXSLT_SRC_TYPE_FILE    1

extern VALUE mXML;
extern VALUE cXMLDocument;

extern VALUE cXSLT;
extern VALUE eXMLXSLTStylesheetRequireParsedDoc;

typedef struct ruby_xslt {
  int data_type;
  void *data;
  VALUE str;
  VALUE xml_doc_obj;
  VALUE ctxt;
  xsltStylesheetPtr xsp;
} ruby_xslt;

#if (RUBY_LIBXML_VERNUM != RUBY_LIBXSLT_VERNUM)
#error "Incompatible LibXML-Ruby headers - please install matching version"
#endif

#endif
