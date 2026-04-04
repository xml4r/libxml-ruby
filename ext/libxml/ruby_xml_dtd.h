#ifndef __RXML_DTD__
#define __RXML_DTD__

extern VALUE cXMLDtd;
extern const rb_data_type_t rxml_dtd_data_type;

void  rxml_init_dtd(void);
VALUE rxml_dtd_wrap(xmlDtdPtr xdtd);

#endif
