#include "ruby_libxml.h"
#include "ruby_xml_schema_attribute.h"
#include "ruby_xml_schema_type.h"

VALUE cXMLSchemaAttribute;

static void rxml_schema_attribute_free(xmlSchemaAttributeUsePtr attr)
{
  attr = NULL;
  xmlFree(attr);
}

VALUE rxml_wrap_schema_attribute(xmlSchemaAttributeUsePtr attr)
{
  VALUE result;
  const xmlChar *tns_str, *name_str;

  if (!attr)
    rb_raise(rb_eArgError, "XML::Schema::Attribute required!");

  result = Data_Wrap_Struct(cXMLSchemaAttribute, NULL, rxml_schema_attribute_free, attr);

  if (attr->type == XML_SCHEMA_EXTRA_ATTR_USE_PROHIB) {
    tns_str = ((xmlSchemaAttributeUseProhibPtr) attr)->targetNamespace;
    name_str = ((xmlSchemaAttributeUseProhibPtr) attr)->name;
  } else if (attr->type == XML_SCHEMA_EXTRA_QNAMEREF) {
    tns_str = ((xmlSchemaQNameRefPtr) attr)->targetNamespace;
    name_str = ((xmlSchemaQNameRefPtr) attr)->name;
  } else {
    tns_str = ((xmlSchemaAttributePtr) (attr->attrDecl))->targetNamespace;
    name_str = ((xmlSchemaAttributePtr) (attr->attrDecl))->name;
  }
  rb_iv_set(result, "@target_namespace", QNIL_OR_STRING(tns_str));
  rb_iv_set(result, "@name", QNIL_OR_STRING(name_str));
  rb_iv_set(result, "@type", rxml_wrap_schema_type((xmlSchemaTypePtr)attr->attrDecl->subtypes));
  rb_iv_set(result, "@value", QNIL_OR_STRING(attr->defValue));
  rb_iv_set(result, "@occurs", INT2NUM(attr->occurs));

  return result;
}

static VALUE rxml_schema_attribute_node(VALUE self)
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

  rb_define_method(cXMLSchemaAttribute, "node", rxml_schema_attribute_node, 0);
}
