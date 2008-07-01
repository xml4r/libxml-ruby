/* $Id$ */

#ifndef __RUBY_XML_STATE__
#define __RUBY_XML_STATE__

extern VALUE cXMLState;

void ruby_init_state(void);
void ruby_xml_state_marker(void);
VALUE ruby_xml_state_object_find(VALUE id);

#endif
