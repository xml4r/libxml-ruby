#include "ruby_libxml.h"
#include "ruby_xml_schema_type.h"

VALUE cXMLSchemaType;

static void rxml_schema_type_free(xmlSchemaTypePtr xschema_type)
{
  xschema_type = NULL;
  xmlFree(xschema_type);
}

VALUE rxml_wrap_schema_type(xmlSchemaTypePtr xtype)
{
  return Data_Wrap_Struct(cXMLSchemaType, NULL, rxml_schema_type_free, xtype);
}

static VALUE rxml_schema_type_namespace(VALUE self)
{
  xmlSchemaTypePtr xtype;

  Data_Get_Struct(self, xmlSchemaTypePtr, xtype);

  QNIL_OR_STRING(xtype->targetNamespace)
}

static VALUE rxml_schema_type_name(VALUE self)
{
  xmlSchemaTypePtr xtype;

  Data_Get_Struct(self, xmlSchemaTypePtr, xtype);

  QNIL_OR_STRING(xtype->name)
}

static VALUE rxml_schema_type_base(VALUE self)
{
  xmlSchemaTypePtr xtype;

  Data_Get_Struct(self, xmlSchemaTypePtr, xtype);

  return Data_Wrap_Struct(cXMLSchemaType, NULL, rxml_schema_type_free, xtype->baseType);
}

static VALUE rxml_schema_type_facets(VALUE self)
{
  xmlSchemaTypePtr xtype;
  xmlSchemaFacetPtr facet;
  VALUE facets;
  VALUE rfacet;

  facets = rb_iv_get(self, "@facets");

  if (facets == Qnil) {
    facets = rb_ary_new();
    Data_Get_Struct(self, xmlSchemaTypePtr, xtype);

    facet = xtype->facets;

    while (facet != NULL) {
      rfacet = rxml_wrap_schema_facet((xmlSchemaFacetPtr) facet);
      rb_ary_push(facets, rfacet);
      facet = facet->next;
    }

    rb_iv_set(self, "@facets", facets);
  }

  return facets;
}

static VALUE rxml_schema_type_node(VALUE self)
{
  xmlSchemaTypePtr xtype;

  Data_Get_Struct(self, xmlSchemaTypePtr, xtype);

  if(xtype->node != NULL)
    return rxml_node_wrap(xtype->node);
  else
    return Qnil;
}

static VALUE rxml_schema_type_kind(VALUE self)
{
  xmlSchemaTypePtr xtype;

  Data_Get_Struct(self, xmlSchemaTypePtr, xtype);

  if(xtype->type == NULL)
    return Qnil;
  else
    return INT2NUM(xtype->type);
}

static VALUE get_annotation(xmlSchemaAnnotPtr annot)
{
  if(annot != NULL && annot->content != NULL && annot->content->content != NULL)
    return rb_str_new2(annot->content->content);
  else
    return Qnil;
}

static VALUE rxml_schema_type_annot(VALUE self)
{
  xmlSchemaTypePtr xtype;

  Data_Get_Struct(self, xmlSchemaTypePtr, xtype);

  if(xtype != NULL && xtype->annot != NULL)
    return get_annotation(xtype->annot);
  else
    return Qnil;
}

static void rxmlSchemaCollectElements(xmlSchemaParticlePtr particle, FILE *output, VALUE self)
{
  VALUE elements;
  VALUE relement;
  xmlSchemaTreeItemPtr term;

  if (particle == NULL)
    return;

  term = particle->children;

  if (term != NULL) {
    switch (term->type) {
      case XML_SCHEMA_TYPE_ELEMENT:

        elements = rb_iv_get(self, "@elements");
        relement = rxml_wrap_schema_element((xmlSchemaElementPtr) term);

        rb_hash_aset(elements, rb_str_new2(((xmlSchemaElementPtr) term)->name), relement);

        break;

      case XML_SCHEMA_TYPE_SEQUENCE:
        rb_iv_set(self, "@type", rb_str_new2("SEQUENCE"));
        break;

      case XML_SCHEMA_TYPE_CHOICE:
        rb_iv_set(self, "@type", rb_str_new2("CHOICE"));
        break;

      case XML_SCHEMA_TYPE_ALL:
        rb_iv_set(self, "@type", rb_str_new2("ALL"));
        break;

      case XML_SCHEMA_TYPE_ANY:
        rb_iv_set(self, "@type", rb_str_new2("ANY"));
        break;

      default:
        rb_iv_set(self, "@type", rb_str_new2("UNKNOWN"));

        return;
    }
  }

  if (term &&
      ((term->type == XML_SCHEMA_TYPE_SEQUENCE) ||
          (term->type == XML_SCHEMA_TYPE_CHOICE) ||
          (term->type == XML_SCHEMA_TYPE_ALL)) &&
      (term->children != NULL)) {
    rxmlSchemaCollectElements((xmlSchemaParticlePtr) term->children, output, self);
  }

  if (particle->next != NULL)
    rxmlSchemaCollectElements((xmlSchemaParticlePtr) particle->next, output, self);
}

static VALUE
rxml_schema_type_elements(VALUE self)
{
  VALUE elements;
  xmlSchemaTypePtr xtype;
  xmlSchemaParticlePtr particle;

  Data_Get_Struct(self, xmlSchemaTypePtr, xtype);

  if (rb_iv_get(self, "@elements") == Qnil) {
    elements = rb_hash_new();
    rb_iv_set(self, "@elements", elements);
    rxmlSchemaCollectElements((xmlSchemaParticlePtr) xtype->subtypes, stdout, self);
  }

  return rb_iv_get(self, "@elements");
}

void rxml_init_schema_type(void)
{
  cXMLSchemaType = rb_define_class_under(cXMLSchema, "Type", rb_cObject);

  rb_define_method(cXMLSchemaType, "namespace", rxml_schema_type_namespace, 0);
  rb_define_method(cXMLSchemaType, "name", rxml_schema_type_name, 0);
  rb_define_method(cXMLSchemaType, "elements", rxml_schema_type_elements, 0);
  rb_define_method(cXMLSchemaType, "base", rxml_schema_type_base, 0);
  rb_define_method(cXMLSchemaType, "kind", rxml_schema_type_kind, 0);
  rb_define_method(cXMLSchemaType, "node", rxml_schema_type_node, 0);
  rb_define_method(cXMLSchemaType, "facets", rxml_schema_type_facets, 0);
  rb_define_method(cXMLSchemaType, "annotation", rxml_schema_type_annot, 0);
}