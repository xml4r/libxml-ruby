#include "ruby_libxml.h"
#include "ruby_xml_dtd.h"

VALUE cXMLDtd;

void
ruby_xml_dtd_free(ruby_xml_dtd *rxdtd) {
  if (rxdtd->dtd != NULL) {
    xmlFreeDtd(rxdtd->dtd);
    rxdtd->dtd = NULL;
  }

  ruby_xfree(rxdtd);
}

static void
ruby_xml_dtd_mark(ruby_xml_dtd *rxdtd) {
  return;
  //if (rxdtd == NULL) return;
  //if (!NIL_P(rxd->xmlver)) rb_gc_mark(rxd->xmlver);
}

/*
 * call-seq:
 *    XML::Dtd.new("public system") -> dtd
 *    XML::Dtd.new("public", "system") -> dtd
 * 
 * Create a new Dtd from the specified public and system
 * identifiers.
 */
VALUE
ruby_xml_dtd_initialize(int argc, VALUE *argv, VALUE class) {
  ruby_xml_dtd *rxdtd;
  VALUE external, system, dtd_string;
  xmlParserInputBufferPtr buffer;
  xmlCharEncoding enc = XML_CHAR_ENCODING_NONE;
  xmlChar *new_string;

  // 1 argument -- string               --> parsujeme jako dtd
  // 2 argumenty -- public, system      --> bude se hledat
  switch (argc) {
  case 2:
    rb_scan_args(argc, argv, "20", &external, &system);

    Check_Type(external, T_STRING);
    Check_Type(system,   T_STRING);
    rxdtd = ALLOC(ruby_xml_dtd);
    rxdtd->dtd = xmlParseDTD( (xmlChar*)StringValuePtr(external),
                              (xmlChar*)StringValuePtr(system) );
    if (rxdtd->dtd == NULL) {
      ruby_xfree(rxdtd);
      return(Qfalse);
    }

    xmlSetTreeDoc( (xmlNodePtr)rxdtd->dtd, NULL );
    return( Data_Wrap_Struct(cXMLDtd, ruby_xml_dtd_mark, ruby_xml_dtd_free, rxdtd) );
    break;

/*
SV *
new(CLASS, external, system)
        char * CLASS
        char * external
        char * system
    ALIAS:
        parse_uri = 1
    PREINIT:
        xmlDtdPtr dtd = NULL;
    CODE:
        LibXML_error = sv_2mortal(newSVpv("", 0));
        dtd = xmlParseDTD((const xmlChar*)external, (const xmlChar*)system);
        if ( dtd == NULL ) {
            XSRETURN_UNDEF;
        }
        xmlSetTreeDoc((xmlNodePtr)dtd, NULL);
        RETVAL = PmmNodeToSv( (xmlNodePtr) dtd, NULL );
    OUTPUT:
        RETVAL
*/

  case 1:

    rb_scan_args(argc, argv, "10", &dtd_string);
    buffer = xmlAllocParserInputBuffer(enc);
    //if ( !buffer) return Qnil
    new_string = xmlStrdup((xmlChar*)StringValuePtr(dtd_string));
    xmlParserInputBufferPush(buffer, xmlStrlen(new_string), (const char*)new_string);

    rxdtd = ALLOC(ruby_xml_dtd);
    rxdtd->dtd = xmlIOParseDTD(NULL, buffer, enc);

    // NOTE: For some reason freeing this InputBuffer causes a segfault! 
    // xmlFreeParserInputBuffer(buffer); 
    xmlFree(new_string);

    return( Data_Wrap_Struct(cXMLDtd, ruby_xml_dtd_mark, ruby_xml_dtd_free, rxdtd) );

    break;
/*
SV * parse_string(CLASS, str, ...)
        char * CLASS
        char * str
    PREINIT:
        STRLEN n_a;
        xmlDtdPtr res;
        SV * encoding_sv;
        xmlParserInputBufferPtr buffer;
        xmlCharEncoding enc = XML_CHAR_ENCODING_NONE;
        xmlChar * new_string;
        STRLEN len;
    CODE:
        LibXML_init_error();
        if (items > 2) {
            encoding_sv = ST(2);
            if (items > 3) {
                croak("parse_string: too many parameters");
            }
            // warn("getting encoding...\n"); 
            enc = xmlParseCharEncoding(SvPV(encoding_sv, n_a));
            if (enc == XML_CHAR_ENCODING_ERROR) {
                croak("Parse of encoding %s failed: %s", SvPV(encoding_sv, n_a), SvPV(LibXML_error, n_a));
            }
        }
        buffer = xmlAllocParserInputBuffer(enc);
        // buffer = xmlParserInputBufferCreateMem(str, xmlStrlen(str), enc); 
        if ( !buffer)
            croak("cant create buffer!\n" );

        new_string = xmlStrdup((const xmlChar*)str);
        xmlParserInputBufferPush(buffer, xmlStrlen(new_string), (const char*)new_string);

        res = xmlIOParseDTD(NULL, buffer, enc);

        // NOTE: For some reason freeing this InputBuffer causes a segfault! 
        // xmlFreeParserInputBuffer(buffer); 
        xmlFree(new_string);

        sv_2mortal( LibXML_error );
        LibXML_croak_error();

        if (res == NULL) {
            croak("no DTD parsed!");
        }
        RETVAL = PmmNodeToSv((xmlNodePtr)res, NULL);
    OUTPUT:
        RETVAL
 */
    
  default:
    rb_raise(rb_eArgError, "wrong number of arguments (need 1 or 2)");
  }

  //docobj = ruby_xml_document_new2(cXMLDocument, xmlver);
  return Qnil;
}

// Rdoc needs to know 
#ifdef RDOC_NEVER_DEFINED
  mLibXML = rb_define_module("LibXML");
  mXML = rb_define_module_under(mLibXML, "XML");
#endif

void
ruby_init_xml_dtd(void) {
  cXMLDtd = rb_define_class_under(mXML, "Dtd", rb_cObject);
  rb_define_singleton_method(cXMLDtd, "new", ruby_xml_dtd_initialize, -1);
  //rb_define_method(cXMLDocument, "xinclude", ruby_xml_document_xinclude, 0);
}

