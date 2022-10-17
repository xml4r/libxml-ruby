#ifndef __RXML_SCHEMA_ATTRIBUTE__
#define __RXML_SCHEMA_ATTRIBUTE__

#include "ruby_xml_schema.h"

extern VALUE cXMLSchemaAttribute;

/**
 * xmlSchemaAttributeUsePtr:
 *
 * The abstract base type for tree-like structured schema components.
 * (Extends xmlSchemaTreeItem)
 */
typedef struct _xmlSchemaAttributeUse xmlSchemaAttributeUse;
typedef xmlSchemaAttributeUse *xmlSchemaAttributeUsePtr;
struct _xmlSchemaAttributeUse {
    xmlSchemaTypeType type;
    xmlSchemaAnnotPtr annot;
    xmlSchemaAttributeUsePtr next; /* The next attr. use. */
    /*
    * The attr. decl. OR a QName-ref. to an attr. decl. OR
    * a QName-ref. to an attribute group definition.
    */
    xmlSchemaAttributePtr attrDecl;

    int flags;
    xmlNodePtr node;
    int occurs;
    /* required, optional */
    const xmlChar *defValue;
    xmlSchemaValPtr defVal;
};

void rxml_init_schema_attribute(void);
VALUE rxml_wrap_schema_attribute(xmlSchemaAttributeUsePtr attr);

#endif
