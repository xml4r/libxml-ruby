#include "ruby_libxml.h"
#include "ruby_xml_schema_facet.h"

#include <libxml/schemasInternals.h>
#include <libxml/xmlschemas.h>
#include <libxml/xmlschemastypes.h>

VALUE cXMLSchemaFacet;

static const rb_data_type_t rxml_schema_facet_type = {
  "XML::Schema::Facet",
  {NULL, NULL, NULL},
  NULL, NULL, 0
};

VALUE rxml_wrap_schema_facet(xmlSchemaFacetPtr facet)
{
  VALUE result;

  if (!facet)
    rb_raise(rb_eArgError, "XML::Schema::Facet required!");

  result = TypedData_Wrap_Struct(cXMLSchemaFacet, &rxml_schema_facet_type, facet);

  rb_iv_set(result, "@kind", INT2NUM(facet->type));
  rb_iv_set(result, "@value", QNIL_OR_STRING(facet->value));

  return result;

}

/* START FACET*/

static VALUE rxml_schema_facet_node(VALUE self)
{
  xmlSchemaFacetPtr facet;

  TypedData_Get_Struct(self, xmlSchemaFacet, &rxml_schema_facet_type, facet);

  return rxml_node_wrap(facet->node);
}

void rxml_init_schema_facet(void)
{
  cXMLSchemaFacet = rb_define_class_under(cXMLSchema, "Facet", rb_cObject);
  rb_define_attr(cXMLSchemaFacet, "kind", 1, 0);
  rb_define_attr(cXMLSchemaFacet, "value", 1, 0);

  rb_define_method(cXMLSchemaFacet, "node", rxml_schema_facet_node, 0);
}
