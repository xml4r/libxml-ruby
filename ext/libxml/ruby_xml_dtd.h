#ifndef __RUBY_XML_DTD__
#define __RUBY_XML_DTD__

extern VALUE cXMLDtd;

typedef struct rxp_dtd {
  xmlDtdPtr dtd;   /* DTD interface */
  //int data_type;   /* The data type referenced by *data */
  //void *data;      /* Pointer to an external structure of options */
  //int is_ptr;      /* Determines if this object owns its data or points to it someplace else */
  //VALUE xmlver;    /* T_STRING with the xml version */
} ruby_xml_dtd;

void  ruby_init_xml_dtd(void);
void  ruby_dtd_free(ruby_xml_dtd *rxdtd);

#endif
