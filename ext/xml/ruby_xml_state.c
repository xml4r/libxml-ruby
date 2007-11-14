/* $Id$ */

#include "libxml.h"

VALUE cXMLState;
static VALUE weak_holder;
static int id_inst;

ID id_state;

#undef DEBUG

void
ruby_xml_state_free(int * dummy) {
  if ( dummy==NULL ) return;
  xmlCleanupParser();
  free(dummy);
  dummy=NULL;
  weak_holder=Qnil;
  rb_ivar_set(cXMLState,id_state,Qnil);
}

void
ruby_xml_state_mark(void * a) {
#ifdef DEBUG
  fprintf(stderr,"marked 0x%x\n",NUM2INT(rb_ivar_get(cXMLState,id_state)));
#endif
}

VALUE
ruby_xml_state_object() {
  VALUE obj,id;
  int *dummy;

  id=rb_ivar_get(cXMLState,id_state);
  if (NIL_P(id)) {
    obj = Data_Make_Struct(cXMLState, int,
			   ruby_xml_state_mark,
			   ruby_xml_state_free,
			   dummy);
    rb_ivar_set(cXMLState,id_state,id=rb_obj_id(obj));
    *dummy=0;
    weak_holder=obj;
    return obj;
  }

#ifdef DEBUG
  fprintf(stderr,"getting 0x%x\n",NUM2INT(id));
#endif
  return weak_holder;

  return rb_funcall(rb_const_get(rb_cModule,rb_intern("ObjectSpace")),
		    rb_intern("_id2ref"),
		    1,
		    id);
}

VALUE
ruby_xml_state_object_find_aux(VALUE id) {
  rb_funcall(rb_const_get(rb_cModule,rb_intern("ObjectSpace")),
	     rb_intern("_id2ref"),
	     1,
	     id);
}


static VALUE
find_failed() {
  return Qnil;
}

VALUE
ruby_xml_state_object_find(VALUE id) {
  return rb_rescue(ruby_xml_state_object_find_aux, id, find_failed, 0);
}
/*
 * This will establish a use and mark it or mark an existing use.
 * should be called by parser.mark and document.mark
 */
void
ruby_xml_state_marker(void) {
  ruby_xml_state_object();
  rb_gc_mark(weak_holder);
}

VALUE
ruby_xml_state_used_p(VALUE klass)
{
  return rb_ivar_get(cXMLState,id_state);
}

VALUE
ruby_xml_state_use(VALUE klass)
{
  return ruby_xml_state_object();
}

// Rdoc needs to know 
#ifdef RDOC_NEVER_DEFINED
  mXML = rb_define_module("XML");
#endif


void
ruby_init_state(void) {	
  cXMLState = rb_define_class_under(mXML, "State", rb_cObject);

  rb_define_singleton_method(cXMLState, "used?",
			     ruby_xml_state_used_p, 0);
  rb_define_singleton_method(cXMLState, "use",
			     ruby_xml_state_use, 0);

  rb_ivar_set(cXMLState, id_state=rb_intern("@stateid"), Qnil);
}
