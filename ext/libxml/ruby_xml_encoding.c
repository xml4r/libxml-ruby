/* $Id: rxml_input.c 528 2008-11-15 23:43:48Z cfis $ */

/* Please see the LICENSE file for copyright and distribution information */

#include <stdarg.h>
#include "ruby_libxml.h"

/*
 * Document-class: LibXML::XML::Encoding
 *
 * Input is a helper class that defines a libxml data source.
 * Libxml can parse files, strings, io streams and documents
 * accessible via networking protocols such as http.
 * Be default, the ruby-libxml bindings expose parsing
 * files, io streams and strings.
 *
 * Generally you will not directly work with the input object,
 * but instead will use the various Document and Parser apis.
 * For example:
 *
 *   parser = XML::Parser.file('my_file')
 *   parser = XML::Parser.string('<myxml/>')
 *   parser = XML::Parser.io(File.open('my_file'))
 *
 *  XML::HTMLParser, XML::Reader, XML::SaxParser and
 *  XML::Document work in the same way.
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

VALUE mXMLEncoding;


/*
 * call-seq:
 *    Input.s_to_encoding("UTF_8") -> XML::Encoding::UTF_8
 *
 * Converts an encoding string to an encoding constant
 * defined on the XML::Encoding class.
 */
static VALUE rxml_encoding_from_s(VALUE klass, VALUE encoding)
{
  xmlCharEncoding xencoding;
  
  if (encoding == Qnil)
    return Qnil;

  xencoding = xmlParseCharEncoding(StringValuePtr(encoding));
  return NUM2INT(xencoding);
}

/*
 * call-seq:
 *    Input.encoding_to_s(Input::ENCODING) -> "encoding"
 *
 * Converts an encoding contstant defined on the XML::Encoding
 * class to its text representation.
 */
static VALUE rxml_encoding_to_s(VALUE klass, VALUE encoding)
{
  const char* xecoding = xmlGetCharEncodingName(NUM2INT(encoding));

  if (!xecoding)
    return Qnil;
  else
    return rb_str_new2(xecoding);
}

// Rdoc needs to know
#ifdef RDOC_NEVER_DEFINED
mLibXML = rb_define_module("LibXML");
mXML = rb_define_module_under(mLibXML, "XML");
#endif

void ruby_init_xml_encoding(void)
{
  mXMLEncoding = rb_define_module_under(mXML, "Encoding", rb_cObject);
  rb_define_module_function(mXMLEncoding, "from_s", rxml_encoding_from_s, 1);
  rb_define_module_function(mXMLEncoding, "to_s", rxml_encoding_to_s, 1);

  rb_define_const(mXMLEncoding, "UNDEFINED", INT2NUM(XPATH_UNDEFINED));
  rb_define_const(mXMLEncoding, "ERROR", INT2NUM(XML_CHAR_ENCODING_ERROR)); /* No char encoding detected */
  rb_define_const(mXMLEncoding, "NONE", INT2NUM(XML_CHAR_ENCODING_NONE)); /* No char encoding detected */
  rb_define_const(mXMLEncoding, "UTF_8", INT2NUM(XML_CHAR_ENCODING_UTF8)); /* UTF-8 */
  rb_define_const(mXMLEncoding, "UTF_16LE", INT2NUM(XML_CHAR_ENCODING_UTF16LE)); /* UTF-16 little endian */
  rb_define_const(mXMLEncoding, "UTF_16BE", INT2NUM(XML_CHAR_ENCODING_UTF16BE)); /* UTF-16 big endian */
  rb_define_const(mXMLEncoding, "UCS_4LE", INT2NUM(XML_CHAR_ENCODING_UCS4LE)); /* UCS-4 little endian */
  rb_define_const(mXMLEncoding, "UCS_4BE", INT2NUM(XML_CHAR_ENCODING_UCS4BE)); /* UCS-4 big endian */
  rb_define_const(mXMLEncoding, "EBCDIC", INT2NUM(XML_CHAR_ENCODING_EBCDIC)); /* EBCDIC uh! */
  rb_define_const(mXMLEncoding, "UCS_4_2143", INT2NUM(XML_CHAR_ENCODING_UCS4_2143)); /* UCS-4 unusual ordering */
  rb_define_const(mXMLEncoding, "UCS_4_3412", INT2NUM(XML_CHAR_ENCODING_UCS4_3412)); /* UCS-4 unusual ordering */
  rb_define_const(mXMLEncoding, "UCS_2", INT2NUM(XML_CHAR_ENCODING_UCS2)); /* UCS-2 */
  rb_define_const(mXMLEncoding, "ISO_8859_1", INT2NUM(XML_CHAR_ENCODING_8859_1)); /* ISO-8859-1 ISO Latin 1 */
  rb_define_const(mXMLEncoding, "ISO_8859_2", INT2NUM(XML_CHAR_ENCODING_8859_2)); /* ISO-8859-2 ISO Latin 2 */
  rb_define_const(mXMLEncoding, "ISO_8859_3", INT2NUM(XML_CHAR_ENCODING_8859_3)); /* ISO-8859-3 */
  rb_define_const(mXMLEncoding, "ISO_8859_4", INT2NUM(XML_CHAR_ENCODING_8859_4)); /* ISO-8859-4 */
  rb_define_const(mXMLEncoding, "ISO_8859_5", INT2NUM(XML_CHAR_ENCODING_8859_5)); /* ISO-8859-5 */
  rb_define_const(mXMLEncoding, "ISO_8859_6", INT2NUM(XML_CHAR_ENCODING_8859_6)); /* ISO-8859-6 */
  rb_define_const(mXMLEncoding, "ISO_8859_7", INT2NUM(XML_CHAR_ENCODING_8859_7)); /* ISO-8859-7 */
  rb_define_const(mXMLEncoding, "ISO_8859_8", INT2NUM(XML_CHAR_ENCODING_8859_8)); /* ISO-8859-8 */
  rb_define_const(mXMLEncoding, "ISO_8859_9", INT2NUM(XML_CHAR_ENCODING_8859_9)); /* ISO-8859-9 */
  rb_define_const(mXMLEncoding, "ISO_2022_JP", INT2NUM(XML_CHAR_ENCODING_2022_JP)); /* ISO-2022-JP */
  rb_define_const(mXMLEncoding, "SHIFT_JIS", INT2NUM(XML_CHAR_ENCODING_SHIFT_JIS)); /* Shift_JIS */
  rb_define_const(mXMLEncoding, "EUC_JP", INT2NUM(XML_CHAR_ENCODING_EUC_JP)); /* EUC-JP */
  rb_define_const(mXMLEncoding, "ASCII", INT2NUM(XML_CHAR_ENCODING_ASCII)); /* pure ASCII */
}
