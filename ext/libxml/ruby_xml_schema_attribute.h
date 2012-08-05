#ifndef __RXML_SCHEMA_ATTRIBUTE__
#define __RXML_SCHEMA_ATTRIBUTE__

#include "ruby_xml_schema.h"
#include <libxml/schemasInternals.h>
#include <libxml/xmlschemas.h>
#include <libxml/xmlschemastypes.h>

extern VALUE cXMLSchemaAttribute;

typedef struct _xmlSchemaAttributeUse xmlSchemaAttributeUse;
typedef xmlSchemaAttributeUse *xmlSchemaAttributeUsePtr;

VALUE rxml_wrap_schema_attribute(xmlSchemaAttributeUsePtr attr);
void rxml_init_schema_attribute(void);

#endif