#ifndef __RUBY_XML_SCHEMA__
#define __RUBY_XML_SCHEMA__

#include <libxml/schemasInternals.h>
#include <libxml/xmlschemas.h>

extern VALUE cXMLSchema;

typedef struct rxp_schema {
  xmlSchemaPtr schema;   /* Schema interface */
} ruby_xml_schema;

void  ruby_init_xml_schema(void);
void  ruby_schema_free(ruby_xml_schema *rxs);
#endif

