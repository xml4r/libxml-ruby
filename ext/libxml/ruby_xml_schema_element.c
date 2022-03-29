#include "ruby_libxml.h"
#include "ruby_xml_schema_element.h"
#include "ruby_xml_schema_type.h"

VALUE cXMLSchemaElement;

static void rxml_schema_element_free(xmlSchemaElementPtr xschema_element)
{
  xschema_element = NULL;
  xmlFree(xschema_element);
}

VALUE rxml_wrap_schema_element(xmlSchemaElementPtr xelem)
{
  VALUE class;

  if (!xelem)
    rb_raise(rb_eArgError, "XML::Schema::Element is required!");

  class = Data_Wrap_Struct(cXMLSchemaElement, NULL, rxml_schema_element_free, xelem);

  rb_iv_set(class, "@name", QNIL_OR_STRING(xelem->name));
  rb_iv_set(class, "@value", QNIL_OR_STRING(xelem->value));
  rb_iv_set(class, "@namespace", QNIL_OR_STRING(xelem->targetNamespace));
  rb_iv_set(class, "@type", rxml_wrap_schema_type((xmlSchemaTypePtr) (xelem->subtypes)));

  return class;
}

static VALUE rexml_schema_element_node(VALUE self)
{
  xmlSchemaElementPtr xelem;

  Data_Get_Struct(self, xmlSchemaElement, xelem);

  return rxml_node_wrap(xelem->node);
}

static VALUE rexml_schema_element_annot(VALUE self)
{
  xmlSchemaElementPtr xelem;
  VALUE annotation = Qnil;

  Data_Get_Struct(self, xmlSchemaElement, xelem);

  if ((xelem->annot != NULL) && (xelem->annot->content != NULL))
  {
    xmlChar *content = xmlNodeGetContent(xelem->annot->content);
    if (content)
    {
      annotation = rxml_new_cstr(content, NULL);
      xmlFree(content);
    }
  }

  return annotation;
}

void rxml_init_schema_element(void)
{
  cXMLSchemaElement = rb_define_class_under(cXMLSchema, "Element", rb_cObject);
  rb_define_attr(cXMLSchemaElement, "name", 1, 0);
  rb_define_attr(cXMLSchemaElement, "value", 1, 0);
  rb_define_attr(cXMLSchemaElement, "namespace", 1, 0);
  rb_define_attr(cXMLSchemaElement, "type", 1, 0);

  rb_define_method(cXMLSchemaElement, "node", rexml_schema_element_node, 0);
  rb_define_method(cXMLSchemaElement, "annotation", rexml_schema_element_annot, 0);
}
