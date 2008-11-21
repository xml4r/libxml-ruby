#ifndef __rxml_SCHEMA__
#define __rxml_SCHEMA__

#include <libxml/schemasInternals.h>
#include <libxml/xmlschemas.h>

extern VALUE cXMLSchema;

typedef struct rxp_schema {
  xmlSchemaPtr schema;   /* Schema interface */
} rxml_schema;

void  ruby_init_xml_schema(void);
void  ruby_schema_free(rxml_schema *rxs);
#endif

