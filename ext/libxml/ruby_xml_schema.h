#ifndef __RXML_SCHEMA__
#define __RXML_SCHEMA__

#include <libxml/schemasInternals.h>
#include <libxml/xmlschemastypes.h>

typedef struct _xmlSchemaItemList xmlSchemaItemList;
typedef xmlSchemaItemList *xmlSchemaItemListPtr;
struct _xmlSchemaItemList {
    void **items;
    /* used for dynamic addition of schemata */
    int nbItems;
    /* used for dynamic addition of schemata */
    int sizeItems; /* used for dynamic addition of schemata */
};

#define QNIL_OR_STRING(slot) \
    (slot == NULL) ? Qnil : rb_str_new2((const char *)slot)

extern VALUE cXMLSchema;
extern const rb_data_type_t rxml_schema_type;

void rxml_init_schema(void);

#endif

