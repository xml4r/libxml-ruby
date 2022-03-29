#include "ruby_libxml.h"
#include "ruby_xml_schema_attribute.h"
#include "ruby_xml_schema_type.h"

VALUE cXMLSchemaAttribute;

static void rxml_schema_attribute_free(xmlSchemaAttributeUsePtr attr)
{
  attr = NULL;
  xmlFree(attr);
}

#define GET_SCHEMA_ATTRIBUTE(attr_ptr, attr_name) \
    (attr_ptr->type == XML_SCHEMA_EXTRA_ATTR_USE_PROHIB) ? \
        (const char *)(((xmlSchemaAttributeUseProhibPtr) attr_ptr)->attr_name) : \
            (attr_ptr->type == XML_SCHEMA_EXTRA_QNAMEREF) ? \
                (const char *)(((xmlSchemaQNameRefPtr) attr_ptr)->attr_name) : \
                (const char *)((xmlSchemaAttributePtr) (attr_ptr->attrDecl)->attr_name)

VALUE rxml_wrap_schema_attribute(xmlSchemaAttributeUsePtr attr)
{
  VALUE class;
  const char *str;

  if (!attr)
    rb_raise(rb_eArgError, "XML::Schema::Attribute required!");

  class = Data_Wrap_Struct(cXMLSchemaAttribute, NULL, rxml_schema_attribute_free, attr);

  str = GET_SCHEMA_ATTRIBUTE(attr, targetNamespace);
  rb_iv_set(class, "@target_namespace", QNIL_OR_STRING(str));
  str = GET_SCHEMA_ATTRIBUTE(attr, name);
  rb_iv_set(class, "@name", QNIL_OR_STRING(str));
  rb_iv_set(class, "@type", rxml_wrap_schema_type((xmlSchemaTypePtr)attr->attrDecl->subtypes));
  rb_iv_set(class, "@value", QNIL_OR_STRING(attr->defValue));
  rb_iv_set(class, "@occurs", INT2NUM(attr->occurs));

  return class;
}

static VALUE rexml_schema_attribute_node(VALUE self)
{
  xmlSchemaAttributeUsePtr attr;

  Data_Get_Struct(self, xmlSchemaAttributeUse, attr);

  return rxml_node_wrap(attr->node);
}

void rxml_init_schema_attribute(void)
{
  cXMLSchemaAttribute = rb_define_class_under(cXMLSchema, "Attribute", rb_cObject);
  rb_define_attr(cXMLSchemaAttribute, "name", 1, 0);
  rb_define_attr(cXMLSchemaAttribute, "type", 1, 0);
  rb_define_attr(cXMLSchemaAttribute, "namespace", 1, 0);
  rb_define_attr(cXMLSchemaAttribute, "value", 1, 0);
  rb_define_attr(cXMLSchemaAttribute, "occurs", 1, 0);

  rb_define_method(cXMLSchemaAttribute, "node", rexml_schema_attribute_node, 0);
}
