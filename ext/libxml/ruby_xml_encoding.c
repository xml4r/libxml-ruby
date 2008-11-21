/* $Id: rxml_ns.c 524 2008-08-20 08:59:54Z cfis $ */

/* Please see the LICENSE file for copyright and distribution information */

#include "ruby_libxml.h"
#include "ruby_xml_encoding.h"

VALUE mXMLEncoding;

/*
* Document-module: LibXML::XML::Encoding
*
* LibXML converts all data sources to UTF8 internally before 
* processing them.  By default, LibXML will determine a data
* source's encoding using the algorithm described on its
* website[* http://xmlsoft.org/encoding.html].
*
* However, its some cases it is possible to tell LibXML
* the data source's encoding via the constants defined in
* the Encoding module.
*
* Example 1:
* 
*   parser = XML::Parser.new
*   parser.input.encoding = XML::Encoding::ISO_8859_1
*   parser.io = File.open('some_file', 'rb')
*   doc = parser.parse
*
* Example 2:
* 
*   parser = XML::HTMLParser.new
*   parser.encoding = XML::Encoding::ISO_8859_1
*   parser.file = "some_file"
*   doc = parser.parse
*
* Example 3:
* 
*   document = XML::Document.new
*   encoding_string =  XML::Encoding.encoding_to_s(XML::Encoding::ISO_8859_1)
*   document.encoding = document
*   doc << XML::Node.new */

VALUE
rxml_encoding_to_s(VALUE self, VALUE encoding)
{
  char* encodingStr = NULL;

  switch (NUM2INT(encoding))
  {
    case XML_CHAR_ENCODING_ERROR:
      encodingStr = "Error";
      break;
    case XML_CHAR_ENCODING_NONE:
      encodingStr = "None";
      break;
    case XML_CHAR_ENCODING_UTF8:
      encodingStr = "UTF-8";
      break;
    case XML_CHAR_ENCODING_UTF16LE:
      encodingStr = "UTF-16LE";
      break;
    case XML_CHAR_ENCODING_UTF16BE:
      encodingStr = "UTF-16BE";
      break;
    case XML_CHAR_ENCODING_UCS4LE:
      encodingStr = "UCS-4LE";
      break;
    case XML_CHAR_ENCODING_UCS4BE:
      encodingStr = "UCS-4BE";
      break;
    case XML_CHAR_ENCODING_EBCDIC:
      encodingStr = "EBCDIC";
      break;
    case XML_CHAR_ENCODING_UCS4_2143:
      encodingStr = "UCS-4";
      break;
    case XML_CHAR_ENCODING_UCS4_3412:
      encodingStr = "UCS-4";
      break;
    case XML_CHAR_ENCODING_UCS2:
      encodingStr = "UCS-2";
      break;
    case XML_CHAR_ENCODING_8859_1:
      encodingStr = "ISO-8859-1";
      break;
    case XML_CHAR_ENCODING_8859_2:
      encodingStr = "ISO-8859-2";
      break;
    case XML_CHAR_ENCODING_8859_3:
      encodingStr = "ISO-8859-3";
      break;
    case XML_CHAR_ENCODING_8859_4:
      encodingStr = "ISO-8859-4";
      break;
    case XML_CHAR_ENCODING_8859_5:
      encodingStr = "ISO-8859-5";
      break;
    case XML_CHAR_ENCODING_8859_6:
      encodingStr = "ISO-8859-6";
      break;
    case XML_CHAR_ENCODING_8859_7:
      encodingStr = "ISO-8859-7";
      break;
    case XML_CHAR_ENCODING_8859_8:
      encodingStr = "ISO-8859-8";
      break;
    case XML_CHAR_ENCODING_8859_9:
      encodingStr = "ISO-8859-9";
      break;
    case XML_CHAR_ENCODING_2022_JP:
      encodingStr = "ISO-2022-JP";
      break;
    case XML_CHAR_ENCODING_SHIFT_JIS:
      encodingStr = "Shift_JIS";
      break;
    case XML_CHAR_ENCODING_EUC_JP:
      encodingStr = "EUC-JP";
      break;
    case XML_CHAR_ENCODING_ASCII:
      encodingStr = "ASCII";
      break;
  }

  return rb_str_new2(encodingStr);
}

// Rdoc needs to know 
#ifdef RDOC_NEVER_DEFINED
  mLibXML = rb_define_module("LibXML");
  mXML = rb_define_module_under(mLibXML, "XML");
#endif

void
ruby_init_xml_encoding(void) {
  mXMLEncoding = rb_define_module_under(mXML, "Encoding");

  rb_define_module_function(mXMLEncoding, "encoding_to_s", rxml_encoding_to_s, 1);

  rb_define_const(mXMLEncoding, "UNDEFINED", INT2NUM(XPATH_UNDEFINED));
  rb_define_const(mXMLEncoding, "ERROR", INT2NUM(XML_CHAR_ENCODING_ERROR));         /* No char encoding detected */
  rb_define_const(mXMLEncoding, "NONE", INT2NUM(XML_CHAR_ENCODING_NONE));           /* No char encoding detected */
  rb_define_const(mXMLEncoding, "UTF8", INT2NUM(XML_CHAR_ENCODING_UTF8));	          /* UTF-8 */
  rb_define_const(mXMLEncoding, "UTF16LE", INT2NUM(XML_CHAR_ENCODING_UTF16LE));	    /* UTF-16 little endian */
  rb_define_const(mXMLEncoding, "UTF16BE", INT2NUM(XML_CHAR_ENCODING_UTF16BE));	    /* UTF-16 big endian */
  rb_define_const(mXMLEncoding, "UCS4LE", INT2NUM(XML_CHAR_ENCODING_UCS4LE));	      /* UCS-4 little endian */
  rb_define_const(mXMLEncoding, "UCS4BE", INT2NUM(XML_CHAR_ENCODING_UCS4BE));	      /* UCS-4 big endian */
  rb_define_const(mXMLEncoding, "EBCDIC", INT2NUM(XML_CHAR_ENCODING_EBCDIC));	      /* EBCDIC uh! */
  rb_define_const(mXMLEncoding, "UCS4_2143", INT2NUM(XML_CHAR_ENCODING_UCS4_2143)); /* UCS-4 unusual ordering */
  rb_define_const(mXMLEncoding, "UCS4_3412", INT2NUM(XML_CHAR_ENCODING_UCS4_3412)); /* UCS-4 unusual ordering */
  rb_define_const(mXMLEncoding, "UCS2", INT2NUM(XML_CHAR_ENCODING_UCS2));	          /* UCS-2 */
  rb_define_const(mXMLEncoding, "ISO_8859_1", INT2NUM(XML_CHAR_ENCODING_8859_1));	      /* ISO-8859-1 ISO Latin 1 */
  rb_define_const(mXMLEncoding, "ISO_8859_2", INT2NUM(XML_CHAR_ENCODING_8859_2));	      /* ISO-8859-2 ISO Latin 2 */
  rb_define_const(mXMLEncoding, "ISO_8859_3", INT2NUM(XML_CHAR_ENCODING_8859_3));	      /* ISO-8859-3 */
  rb_define_const(mXMLEncoding, "ISO_8859_4", INT2NUM(XML_CHAR_ENCODING_8859_4));	      /* ISO-8859-4 */
  rb_define_const(mXMLEncoding, "ISO_8859_5", INT2NUM(XML_CHAR_ENCODING_8859_5));	      /* ISO-8859-5 */
  rb_define_const(mXMLEncoding, "ISO_8859_6", INT2NUM(XML_CHAR_ENCODING_8859_6));	      /* ISO-8859-6 */
  rb_define_const(mXMLEncoding, "ISO_8859_7", INT2NUM(XML_CHAR_ENCODING_8859_7));	      /* ISO-8859-7 */
  rb_define_const(mXMLEncoding, "ISO_8859_8", INT2NUM(XML_CHAR_ENCODING_8859_8));	      /* ISO-8859-8 */
  rb_define_const(mXMLEncoding, "ISO_8859_9", INT2NUM(XML_CHAR_ENCODING_8859_9));	      /* ISO-8859-9 */
  rb_define_const(mXMLEncoding, "ISO_2022_JP", INT2NUM(XML_CHAR_ENCODING_2022_JP));     /* ISO-2022-JP */
  rb_define_const(mXMLEncoding, "SHIFT_JIS", INT2NUM(XML_CHAR_ENCODING_SHIFT_JIS)); /* Shift_JIS */
  rb_define_const(mXMLEncoding, "EUC_JP", INT2NUM(XML_CHAR_ENCODING_EUC_JP));       /* EUC-JP */
  rb_define_const(mXMLEncoding, "ASCII", INT2NUM(XML_CHAR_ENCODING_ASCII));         /* pure ASCII */
}
