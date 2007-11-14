/* Please see the LICENSE file for copyright and distribution information */

#ifndef __RUBY_LIBXML_H__
#define __RUBY_LIBXML_H__

/* Don't nuke this block!  It is used for automatically updating the
 * versions below. VERSION = string formatting, VERNUM = numbered
 * version for inline testing: increment both or none at all. */
#define RUBY_LIBXML_VERSION  "0.5.2.1"
#define RUBY_LIBXML_VERNUM   521
#define RUBY_LIBXML_VER_MAJ   0
#define RUBY_LIBXML_VER_MIN   5
#define RUBY_LIBXML_VER_MIC   2
#define RUBY_LIBXML_VER_PATCH 1

#include <ruby.h>
#include <rubyio.h>
#include <util.h>
#include <libxml/parser.h>
#include <libxml/parserInternals.h>
#include <libxml/debugXML.h>
#include <libxml/xmlversion.h>
#include <libxml/xmlmemory.h>
#include <libxml/xpath.h>
#include <libxml/valid.h>
#include <libxml/catalog.h>
#include <libxml/HTMLparser.h>
#include <libxml/xmlreader.h>

// Maybe not yet defined in ruby
#ifndef RSTRING_LEN
#define RSTRING_LEN(x) RSTRING(x)->len
#endif

#ifdef LIBXML_DEBUG_ENABLED
#include <libxml/xpathInternals.h>
#endif
#ifdef LIBXML_XINCLUDE_ENABLED
#include <libxml/xinclude.h>
#endif
#ifdef LIBXML_XPTR_ENABLED
#include <libxml/xpointer.h>
#endif

#define RUBY_LIBXML_SRC_TYPE_NULL    0
#define RUBY_LIBXML_SRC_TYPE_FILE    1
#define RUBY_LIBXML_SRC_TYPE_STRING  2
#define RUBY_LIBXML_SRC_TYPE_IO      3
#define RUBY_LIBXML_SRC_TYPE_XPATH   4

typedef struct rx_file_data {
  VALUE filename; /* Filename/path to self */
} rx_file_data;

typedef struct rx_io_data {
  VALUE io;
} rx_io_data;

typedef struct rx_string_data {
  VALUE str;
} rx_string_data;

typedef struct rx_xpath_data {
  VALUE ctxt;
} rx_xpath_data;

#include "ruby_xml_state.h"
#include "ruby_xml_attr.h"
#include "ruby_xml_document.h"
#include "ruby_xml_node.h"
#include "ruby_xml_node_set.h"
#include "ruby_xml_ns.h"
#include "ruby_xml_parser.h"
#include "ruby_xml_parser_context.h"
#include "ruby_xml_sax_parser.h"
#include "ruby_xml_tree.h"
#include "ruby_xml_xinclude.h"
#include "ruby_xml_xpath.h"
#include "ruby_xml_xpath_context.h"
#include "ruby_xml_xpath_object.h"
#include "ruby_xml_xpointer.h"
#include "ruby_xml_xpointer_context.h"
#include "ruby_xml_input_cbg.h"
#include "ruby_xml_dtd.h"
#include "ruby_xml_schema.h"
#include "ruby_xml_html_parser.h"
#include "ruby_xml_reader.h"

extern VALUE mXML;
extern VALUE eXMLError;

void ruby_init_parser(void);
void ruby_xml_parser_free(ruby_xml_parser *rxp);

#endif
