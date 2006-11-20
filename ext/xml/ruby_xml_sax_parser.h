/* $Id$ */

/* Please see the LICENSE file for copyright and distribution information */

#ifndef __RUBY_XML_SAX_PARSER__
#define __RUBY_XML_SAX_PARSER__

extern VALUE cXMLSaxParser;
extern VALUE mXMLSaxParserCallbacks;

/*
typedef struct ruby_xml_sax_parser_callbacks {
    VALUE internalSubset;
    VALUE isStandalone;
    VALUE hasInternalSubset;
    VALUE hasExternalSubset;
    VALUE resolveEntity;
    VALUE getEntity;
    VALUE entityDecl;
    VALUE notationDecl;
    VALUE attributeDecl;
    VALUE elementDecl;
    VALUE unparsedEntityDecl;
    VALUE setDocumentLocator;
    VALUE startDocument;
    VALUE endDocument;
    VALUE startElement;
    VALUE endElement;
    VALUE reference;
    VALUE characters;
    VALUE ignorableWhitespace;
    VALUE processingInstruction;
    VALUE comment;
    VALUE xmlParserWarning;
    VALUE xmlParserError;
    VALUE xmlParserFatalError;
    VALUE getParameterEntity;
    VALUE cdataBlock;
    VALUE externalSubset;  
} ruby_xml_sax_parser_callbacks;
*/

typedef struct ruby_xml_sax_parser {
  xmlParserCtxtPtr xpc;
  xmlSAXHandlerPtr xsh;
  //ruby_xml_sax_parser_callbacks *cbp;
  VALUE callbackHandler;
  VALUE filename;
  VALUE str;
} ruby_xml_sax_parser;

void ruby_xml_sax_parser_free(ruby_xml_sax_parser *rxsp);
void ruby_init_xml_sax_parser(void);
VALUE ruby_xml_sax_parser_new(VALUE class);

#endif
