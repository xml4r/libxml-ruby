#include "ruby_libxml.h"
#include "ruby_xml_schema_attribute.h"
#include "ruby_xml_schema_type.h"

/**
 * xmlSchemaBasicItem:
 *
 * The abstract base type for schema components.
 */
typedef struct _xmlSchemaBasicItem xmlSchemaBasicItem;
typedef xmlSchemaBasicItem *xmlSchemaBasicItemPtr;
struct _xmlSchemaBasicItem {
    xmlSchemaTypeType type;
};

/**
 * xmlSchemaQNameRef:
 *
 * A component reference item (not a schema component)
 * (Extends xmlSchemaBasicItem)
 */
typedef struct _xmlSchemaQNameRef xmlSchemaQNameRef;
typedef xmlSchemaQNameRef *xmlSchemaQNameRefPtr;
struct _xmlSchemaQNameRef {
    xmlSchemaTypeType type;
    xmlSchemaBasicItemPtr item;
    /* The resolved referenced item. */
    xmlSchemaTypeType itemType;
    const xmlChar *name;
    const xmlChar *targetNamespace;
    xmlNodePtr node;
};

/**
 * xmlSchemaAttributeUseProhibPtr:
 *
 * A helper component to reflect attribute prohibitions.
 * (Extends xmlSchemaBasicItem)
 */
typedef struct _xmlSchemaAttributeUseProhib xmlSchemaAttributeUseProhib;
typedef xmlSchemaAttributeUseProhib *xmlSchemaAttributeUseProhibPtr;
struct _xmlSchemaAttributeUseProhib {
    xmlSchemaTypeType type;
    /* == XML_SCHEMA_EXTRA_ATTR_USE_PROHIB */
    xmlNodePtr node;
    const xmlChar *name;
    const xmlChar *targetNamespace;
    int isRef;
};

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
  rb_undef_alloc_func(cXMLSchemaAttribute);
  rb_define_attr(cXMLSchemaAttribute, "name", 1, 0);
  rb_define_attr(cXMLSchemaAttribute, "type", 1, 0);
  rb_define_attr(cXMLSchemaAttribute, "namespace", 1, 0);
  rb_define_attr(cXMLSchemaAttribute, "value", 1, 0);
  rb_define_attr(cXMLSchemaAttribute, "occurs", 1, 0);

  rb_define_method(cXMLSchemaAttribute, "node", rxml_schema_attribute_node, 0);
}
