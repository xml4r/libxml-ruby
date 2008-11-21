/* $Id: rxml_parser.h 39 2006-02-21 20:40:16Z roscopeco $ */

/* Please see the LICENSE file for copyright and distribution information */

#ifndef __rxml_INPUT__
#define __rxml_INPUT__

extern VALUE cXMLInput;

void ruby_init_xml_input();

extern ID ENCODING_ATTR;
extern ID FILE_ATTR;
extern ID STRING_ATTR;
extern ID DOCUMENT_ATTR;
extern ID IO_ATTR;

#endif
