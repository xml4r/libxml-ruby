/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

#ifndef __RUBY_XML_DOCUMENT__
#define __RUBY_XML_DOCUMENT__

extern VALUE cXMLDocument;

typedef struct rxp_document {
  xmlDocPtr doc;   /* Tree/DOM interface */
  int data_type;   /* The data type referenced by *data */
  void *data;      /* Pointer to an external structure of options */
  int is_ptr;      /* Determines if this object owns its data or points to it someplace else */
  VALUE xmlver;    /* T_STRING with the xml version */
} ruby_xml_document;

VALUE ruby_xml_document_filename_get(VALUE self);
void  ruby_xml_document_free(ruby_xml_document *rxd);
VALUE ruby_xml_document_new(VALUE class, xmlDocPtr doc);
VALUE ruby_xml_document_new2(VALUE class, VALUE xmlver);
VALUE ruby_xml_document_new3(VALUE class);
VALUE ruby_xml_document_new4(VALUE class, xmlDocPtr doc);
VALUE ruby_xml_document_root_get(VALUE self);
void  ruby_init_xml_document(void);

#endif
