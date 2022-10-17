#ifndef __RXML_SCHEMA_FACET__
#define __RXML_SCHEMA_FACET__

extern VALUE cXMLSchemaFacet;

VALUE rxml_wrap_schema_facet(xmlSchemaFacetPtr facet);
void rxml_init_schema_facet(void);

#endif
