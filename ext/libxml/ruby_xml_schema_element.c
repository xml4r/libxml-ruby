#include "ruby_libxml.h"
#include "ruby_xml_schema_element.h"

VALUE cXMLSchemaElement;

static void rxml_schema_element_free(xmlSchemaElementPtr xschema_element)
{
  xschema_element = NULL;
  xmlFree(xschema_element);
}

VALUE rxml_wrap_schema_element(xmlSchemaElementPtr xelement)
{
  return Data_Wrap_Struct(cXMLSchemaElement, NULL, rxml_schema_element_free, xelement);
}

static VALUE rxml_schema_element_namespace(VALUE self)
{
  xmlSchemaElementPtr xelem;

  Data_Get_Struct(self, xmlSchemaElementPtr, xelem);

  QNIL_OR_STRING(xelem->targetNamespace)
}

static VALUE rxml_schema_element_name(VALUE self)
{
  xmlSchemaElementPtr xelem;

  Data_Get_Struct(self, xmlSchemaElementPtr, xelem);

  QNIL_OR_STRING(xelem->name)
}

static VALUE rxml_schema_element_type(VALUE self)
{
  xmlSchemaElementPtr xelem;
  xmlSchemaTypePtr xtype;

  Data_Get_Struct(self, xmlSchemaElementPtr, xelem);

  xtype = xelem->subtypes;

  return rxml_wrap_schema_type((xmlSchemaTypePtr) xtype);
}

static VALUE rxml_schema_element_node(VALUE self)
{
  xmlSchemaElementPtr xelem;

  Data_Get_Struct(self, xmlSchemaElementPtr, xelem);

  return rxml_node_wrap(xelem->node);
}

static VALUE rxml_schema_element_value(VALUE self)
{
  xmlSchemaElementPtr xelem;

  Data_Get_Struct(self, xmlSchemaElementPtr, xelem);

  QNIL_OR_STRING(xelem->value)
}

static VALUE rxml_schema_element_scope(VALUE self)
{
  xmlSchemaElementPtr xelem;

  Data_Get_Struct(self, xmlSchemaElementPtr, xelem);

  QNIL_OR_STRING(xelem->scope)
}

static VALUE rxml_schema_element_subst_group(VALUE self)
{
  xmlSchemaElementPtr xelem;

  Data_Get_Struct(self, xmlSchemaElementPtr, xelem);

  QNIL_OR_STRING(xelem->substGroup)
}

static VALUE rxml_schema_element_min_occurs(VALUE self)
{
  xmlSchemaElementPtr xelem;

  Data_Get_Struct(self, xmlSchemaElementPtr, xelem);

  QNIL_OR_STRING(xelem->minOccurs)
}

static VALUE rxml_schema_element_max_occurs(VALUE self)
{
  xmlSchemaElementPtr xelem;

  Data_Get_Struct(self, xmlSchemaElementPtr, xelem);

  QNIL_OR_STRING(xelem->maxOccurs)
}

static VALUE get_annotation(xmlSchemaAnnotPtr annot)
{
  if(annot != NULL && annot->content != NULL && annot->content->content != NULL)
    return rb_str_new2(annot->content->content);
  else
    return Qnil;
}

static VALUE rxml_schema_element_annot(VALUE self)
{
  xmlSchemaTypePtr xelem;

  Data_Get_Struct(self, xmlSchemaElementPtr, xelem);

  if(xelem != NULL && xelem->annot != NULL)
    return get_annotation(xelem->annot);
  else
    return Qnil;
}

void rxml_init_schema_element(void)
{
  cXMLSchemaElement = rb_define_class_under(cXMLSchema, "Element", rb_cObject);
  rb_define_method(cXMLSchemaElement, "namespace", rxml_schema_element_namespace, 0);
  rb_define_method(cXMLSchemaElement, "name", rxml_schema_element_name, 0);
  rb_define_method(cXMLSchemaElement, "type", rxml_schema_element_type, 0);
  rb_define_method(cXMLSchemaElement, "node", rxml_schema_element_node, 0);
  rb_define_method(cXMLSchemaElement, "value", rxml_schema_element_value, 0);
  rb_define_method(cXMLSchemaElement, "scope", rxml_schema_element_scope, 0);
  rb_define_method(cXMLSchemaElement, "subst_group", rxml_schema_element_subst_group, 0);
  rb_define_method(cXMLSchemaElement, "min_occurs", rxml_schema_element_min_occurs, 0);
  rb_define_method(cXMLSchemaElement, "max_occurs", rxml_schema_element_max_occurs, 0);
}