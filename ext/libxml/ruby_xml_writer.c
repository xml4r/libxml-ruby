#include <libxml/xmlversion.h>

#ifdef LIBXML_WRITER_ENABLED

# include <libxml/xmlwriter.h>

# include "ruby_libxml.h"
# include "ruby_xml_document.h"
# include "ruby_xml_writer.h"

# include <ruby/io.h>

VALUE cXMLWriter;
static ID sEncoding, sStandalone;

typedef enum {
    RXMLW_OUTPUT_NONE,
    RXMLW_OUTPUT_IO,
    RXMLW_OUTPUT_DOC,
    RXMLW_OUTPUT_STRING
} rxmlw_output_type;

typedef struct {
    VALUE output;
    xmlBufferPtr buffer;
    xmlTextWriterPtr writer;
    rxmlw_output_type output_type;
} rxml_writer_object;

# ifdef HAVE_RUBY_ENCODING_H

#  define /*VALUE*/ rxml_writer_c_to_ruby_string(/*const xmlChar **/ string, /*long*/ string_len) \
    rb_external_str_new_with_enc(string, string_len, rb_utf8_encoding())

#  define /*VALUE*/ rxml_writer_ruby_string_to_utf8(/*VALUE*/ string) \
    rb_str_conv_enc(string, rb_enc_get(string), rb_utf8_encoding())
// rb_str_export_to_enc(string, rb_utf8_encoding())

#  define /*void*/ rxml_writer_free_utf8_string(/*VALUE*/ orig, /*VALUE*/ utf8) \
    do {                       \
        if (orig != utf8) {    \
            rb_str_free(utf8); \
        }                      \
    } while (0);

# else

#  define /*VALUE*/ rxml_writer_c_to_ruby_string(/*const xmlChar **/ string, /*long*/ string_len) \
    rb_str_new(string, string_len)

#  define /*VALUE*/ rxml_writer_ruby_string_to_utf8(/*VALUE*/ string) \
    string

#  define /*void*/ rxml_writer_free_utf8_string(/*VALUE*/ orig, /*VALUE*/ utf8) \
    /* NOP */

# endif /* HAVE_RUBY_ENCODING_H */

static void rxml_writer_free(rxml_writer_object *rwo)
{
#if 0 /* seems to be done by xmlFreeTextWriter */
    if (NULL != rwo->buffer) {
        xmlBufferFree(rwo->buffer);
    }
#endif
    xmlFreeTextWriter(rwo->writer);
    xfree(rwo);
}

static void rxml_writer_mark(rxml_writer_object *rwo)
{
    if (!NIL_P(rwo->output)) {
        rb_gc_mark(rwo->output);
    }
}

static VALUE rxml_writer_wrap(rxml_writer_object *rwo)
{
    return Data_Wrap_Struct(cXMLWriter, rxml_writer_mark, rxml_writer_free, rwo);
}

static rxml_writer_object *rxml_textwriter_get(VALUE obj)
{
    rxml_writer_object *rwo;

    Data_Get_Struct(obj, rxml_writer_object, rwo);

    return rwo;
}

/* ===== public class methods ===== */

/* call-seq:
 *    XML::Writer::io(io) -> XML::Writer
 *
 * Create a XML::Writer which will write XML directly into an IO object.
 */
static VALUE rxml_writer_io(VALUE klass, VALUE io)
{
#if 0
typedef int (*xmlOutputCloseCallback)(void * context);
typedef int (*xmlOutputWriteCallback)(void * context, const char * buffer, int len);

ssize_t rb_io_bufwrite(VALUE io, const void *buf, size_t size);

xmlOutputBufferPtr xmlOutputBufferCreateIO(xmlOutputWriteCallback iowrite, xmlOutputCloseCallback ioclose, void * ioctx, xmlCharEncodingHandlerPtr encoder)

xmlCharEncodingHandlerPtr xmlFindCharEncodingHandler(const char * name);
#endif
    xmlOutputBufferPtr out;
    rxml_writer_object *rwo;

    rwo = ALLOC(rxml_writer_object);
    rwo->output = io;
    rwo->buffer = NULL;
    rwo->output_type = RXMLW_OUTPUT_IO;
    if (NULL == (out = xmlOutputBufferCreateIO((xmlOutputWriteCallback) rb_io_bufwrite, NULL, (void *) io, NULL))) {
        rxml_raise(&xmlLastError);
    }
    if (NULL == (rwo->writer = xmlNewTextWriter(out))) {
        rxml_raise(&xmlLastError);
    }

    return rxml_writer_wrap(rwo);
}


/* call-seq:
 *    XML::Writer::file(path) -> XML::Writer
 *
 * Create a XML::Writer object which will write XML into the file with
 * the given name.
 */
static VALUE rxml_writer_file(VALUE klass, VALUE filename)
{
    rxml_writer_object *rwo;

    rwo = ALLOC(rxml_writer_object);
    rwo->output = Qnil;
    rwo->buffer = NULL;
    rwo->output_type = RXMLW_OUTPUT_NONE;
    if (NULL == (rwo->writer = xmlNewTextWriterFilename(StringValueCStr(filename), 0))) {
        rxml_raise(&xmlLastError);
    }

    return rxml_writer_wrap(rwo);
}

/* call-seq:
 *    XML::Writer::string -> XML::Writer
 *
 * Create a XML::Writer which will write XML into memory, as string.
 */
static VALUE rxml_writer_string(VALUE klass)
{
    rxml_writer_object *rwo;

    rwo = ALLOC(rxml_writer_object);
    rwo->output = Qnil;
    rwo->output_type = RXMLW_OUTPUT_STRING;
    if (NULL == (rwo->buffer = xmlBufferCreate())) {
        rxml_raise(&xmlLastError);
    }
    if (NULL == (rwo->writer = xmlNewTextWriterMemory(rwo->buffer, 0))) {
        xmlBufferFree(rwo->buffer);
        rxml_raise(&xmlLastError);
    }

    return rxml_writer_wrap(rwo);
}

/* call-seq:
 *    XML::Writer::document -> XML::Writer
 *
 * Create a XML::Writer which will write into an in memory XML::Document
 */
static VALUE rxml_writer_doc(VALUE klass)
{
    xmlDocPtr doc;
    rxml_writer_object *rwo;

    rwo = ALLOC(rxml_writer_object);
    rwo->buffer = NULL;
    rwo->output = Qnil;
    rwo->output_type = RXMLW_OUTPUT_DOC;
    if (NULL == (rwo->writer = xmlNewTextWriterDoc(&doc, 0))) {
        rxml_raise(&xmlLastError);
    }
    rwo->output = rxml_document_wrap(doc);

    return rxml_writer_wrap(rwo);
}

/* ===== public instance methods ===== */

/* call-seq:
 *    XML::Writer.flush -> (true|false)
 *
 * Flushes the output buffer.
 */
static VALUE rxml_writer_flush(VALUE self)
{
    int ret;
    rxml_writer_object *rwo;

    rwo = rxml_textwriter_get(self);
    ret = xmlTextWriterFlush(rwo->writer);

    return (-1 == ret ? Qfalse : Qtrue);
}

/* call-seq:
 *    XML::Writer.result -> (XML::Document|"string"|nil)
 *
 * Returns the associated result object to the XML::Writer creation.
 * A String for a XML::Writer object created with XML::Writer::string,
 * a XML::Document with XML::Writer::document, etc.
 */
static VALUE rxml_writer_result(VALUE self)
{
    VALUE ret;
    rxml_writer_object *rwo;

    ret = Qnil;
    rwo = rxml_textwriter_get(self);
    switch (rwo->output_type) {
        case RXMLW_OUTPUT_DOC:
            ret = rwo->output;
            break;
        case RXMLW_OUTPUT_STRING:
            ret = rxml_writer_c_to_ruby_string(rwo->buffer->content, rwo->buffer->use);
            break;
        case RXMLW_OUTPUT_IO:
        case RXMLW_OUTPUT_NONE:
            break;
        default:
            rb_bug("unexpected output");
            break;
    }

    return ret;
}

/* ===== private helpers ===== */

static VALUE numeric_rxml_writer_void(VALUE obj, int (*fn)(xmlTextWriterPtr))
{
    int ret;
    rxml_writer_object *rwo;

    rwo = rxml_textwriter_get(obj);
    ret = fn(rwo->writer);

    return (-1 == ret ? Qfalse : Qtrue);
}

#if 0
static VALUE numeric_rxml_writer_string(VALUE obj, VALUE name_or_content, int (*fn)(xmlTextWriterPtr, const xmlChar *))
{
    int ret;
    VALUE utf8;
    rxml_writer_object *rwo;

    rwo = rxml_textwriter_get(obj);
    utf8 = rxml_writer_ruby_string_to_utf8(name_or_content);
    ret = fn(rwo->writer, (const xmlChar *) StringValueCStr(utf8));
    rxml_writer_free_utf8_string(name_or_content, utf8);

    return (-1 == ret ? Qfalse : Qtrue);
}
#else
# define numeric_rxml_writer_string(/*VALUE*/ obj, /*VALUE*/ name_or_content, /*int (**/fn/*)(xmlTextWriterPtr, const xmlChar *)*/) \
    numeric_rxml_writer_va_strings(obj, Qnil, 1, fn, name_or_content)
#endif

/**
 * This is quite ugly but thanks to libxml2 coding style, all xmlTextWriter*
 * calls can be redirected to a single function. This can be convenient to:
 * - avoid repeating yourself
 * - convert strings to UTF-8
 * - validate names
 * and so on
 **/
# define XMLWRITER_MAX_STRING_ARGS 5
static VALUE numeric_rxml_writer_va_strings(VALUE obj, VALUE pe, size_t strings_count, int (*fn)(ANYARGS), ...)
{
    va_list ap;
    int argc, ret;
    const char *p;
    rxml_writer_object *rwo;
    const xmlChar *argv[XMLWRITER_MAX_STRING_ARGS];
    VALUE utf8[XMLWRITER_MAX_STRING_ARGS], orig[XMLWRITER_MAX_STRING_ARGS];

    if (strings_count > XMLWRITER_MAX_STRING_ARGS) {
        rb_bug("more arguments than expected");
    }
    ret = -1;
    va_start(ap, fn);
    rwo = rxml_textwriter_get(obj);
    for (argc = 0; argc < strings_count; argc++) {
        VALUE arg;

        arg = va_arg(ap, VALUE);
        orig[argc] = arg;
        if (NIL_P(arg)) {
            utf8[argc] = Qnil;
            argv[argc] = NULL;
        } else {
            utf8[argc] = rxml_writer_ruby_string_to_utf8(orig[argc]);
            argv[argc] = BAD_CAST StringValueCStr(utf8[argc]);
        }
    }
    va_end(ap);

    if (NIL_P(pe)) {
        switch (strings_count) {
            case 0:
                ret = fn(rwo->writer);
                break;
            case 1:
                ret = fn(rwo->writer, argv[0]);
                break;
            case 2:
                ret = fn(rwo->writer, argv[0], argv[1]);
                break;
            case 3:
                ret = fn(rwo->writer, argv[0], argv[1], argv[2]);
                break;
            case 4:
                ret = fn(rwo->writer, argv[0], argv[1], argv[2], argv[3]);
                break;
            case 5:
                ret = fn(rwo->writer, argv[0], argv[1], argv[2], argv[3], argv[4]);
                break;
            default:
                break;
        }
    } else {
        int xpe;

        xpe = R_TEST(pe);
        switch (strings_count) { /* strings_count doesn't include pe */
            case 0:
                ret = fn(rwo->writer, xpe);
                break;
            case 1:
                ret = fn(rwo->writer, xpe, argv[0]);
                break;
            case 2:
                ret = fn(rwo->writer, xpe, argv[0], argv[1]);
                break;
            case 3:
                ret = fn(rwo->writer, xpe, argv[0], argv[1], argv[2]);
                break;
            case 4:
                ret = fn(rwo->writer, xpe, argv[0], argv[1], argv[2], argv[3]);
                break;
            case 5:
                ret = fn(rwo->writer, xpe, argv[0], argv[1], argv[2], argv[3], argv[4]);
                break;
            default:
                break;
        }
    }
# ifdef HAVE_RUBY_ENCODING_H
    while (--strings_count > 0) {
        if (!NIL_P(orig[strings_count])) {
            rxml_writer_free_utf8_string(orig[strings_count], utf8[strings_count]);
        }
    }
# endif /* HAVE_RUBY_ENCODING_H */

    return (-1 == ret ? Qfalse : Qtrue);
}

/* ===== public instance methods ===== */

# if LIBXML_VERSION > 20604
/* call-seq:
 *    XML::Writer.set_indent(indentation) -> (true|false)
 *
 * Toggles indentation on or off.
 */
static VALUE rxml_writer_set_indent(VALUE self, VALUE indentation)
{
    int ret;
    rxml_writer_object *rwo;

    rwo = rxml_textwriter_get(self);
    ret = xmlTextWriterSetIndent(rwo->writer, RTEST(indentation));

    return (-1 == ret ? Qfalse : Qtrue);
}

/* call-seq:
 *    XML::Writer.set_indent_string(string) -> (true|false)
 *
 * Sets the string to use to indent each element of the document.
 * Don't forget to enable indentation with set_indent.
 */
static VALUE rxml_writer_set_indent_string(VALUE self, VALUE indentation)
{
    return numeric_rxml_writer_string(self, indentation, xmlTextWriterSetIndentString);
}
# endif /* LIBXML_VERSION > 20604 */

/* ===== public full tag interface ===== */

/* write_<X> = start_<X> + write_string + end_<X> */

/* call-seq:
 *    XML::Writer.write_comment(content) -> (true|false)
 *
 * Writes a full comment tag, all at once. Returns false on failure.
 * This is equivalent to start_comment + write_string(content) + end_comment.
 */
static VALUE rxml_writer_write_comment(VALUE self, VALUE content)
{
    return numeric_rxml_writer_string(self, content, xmlTextWriterWriteComment);
}

/* call-seq:
 *    XML::Writer.write_cdata(content) -> (true|false)
 *
 * Writes a full CDATA section, all at once. Returns false on failure.
 * This is equivalent to start_cdata + write_string(content) + end_cdata.
 */
static VALUE rxml_writer_write_cdata(VALUE self, VALUE content)
{
    return numeric_rxml_writer_string(self, content, xmlTextWriterWriteCDATA);
}

static VALUE rxml_writer_start_element(VALUE, VALUE);
static VALUE rxml_writer_start_element_ns(int, VALUE *, VALUE);
static VALUE rxml_writer_end_element(VALUE);

/* call-seq:
 *    XML::Writer.write_element(name, content) -> (true|false)
 *
 * Writes a full element tag, all at once. Returns false on failure.
 * This is equivalent to start_element(name) + write_string(content) +
 * end_element.
 */
static VALUE rxml_writer_write_element(int argc, VALUE *argv, VALUE self)
{
    VALUE name, content;

    content = Qnil;
    rb_scan_args(argc, argv, "11", &name, &content);
    if (Qnil == content) {
        if (Qfalse == rxml_writer_start_element(self, name)) {
            return Qfalse;
        }
        return rxml_writer_end_element(self);
    } else {
        return numeric_rxml_writer_va_strings(self, Qnil, 2, xmlTextWriterWriteElement, name, content);
    }
}

# define ARRAY_SIZE(array) \
    (sizeof(array) / sizeof((array)[0]))

/* call-seq:
 *    XML::Writer.write_element_ns(prefix, name, namespaceURI, content) -> (true|false)
 *
 * Writes a full namespaced element tag, all at once. Returns false on failure.
 * This is a shortcut for start_element_ns(prefix, name, namespaceURI) +
 * write_string(content) + end_element.
 *
 * Notes:
 * - by default, the xmlns: definition is repeated on every element. If you want
 * the prefix, but don't want the xmlns: declaration repeated, set +namespaceURI+
 * to nil or omit it. Don't forget to declare the namespace prefix somewhere
 * earlier.
 * - +content+ can be omitted too for an empty tag
 */
static VALUE rxml_writer_write_element_ns(int argc, VALUE *argv, VALUE self)
{
    VALUE prefix, name, namespaceURI, content;

    namespaceURI = content = Qnil;
    rb_scan_args(argc, argv, "22", &prefix, &name, &namespaceURI, &content);
    if (Qnil == content) {
        VALUE argv[3] = { prefix, name, namespaceURI };

        if (Qfalse == rxml_writer_start_element_ns(ARRAY_SIZE(argv), argv, self)) {
            return Qfalse;
        }
        return rxml_writer_end_element(self);
    } else {
        return numeric_rxml_writer_va_strings(self, Qnil, 4, xmlTextWriterWriteElementNS, prefix, name, namespaceURI, content);
    }
}

/* call-seq:
 *    XML::Writer.write_attribute(name, content) -> (true|false)
 *
 * Writes a full attribute, all at once. Returns false on failure.
 * Same as start_attribute(name) + write_string(content) + end_attribute.
 */
static VALUE rxml_writer_write_attribute(VALUE self, VALUE name, VALUE content)
{
    return numeric_rxml_writer_va_strings(self, Qnil, 2, xmlTextWriterWriteAttribute, name, content);
}

/* call-seq:
 *    XML::Writer.write_attribute_ns(prefix, name, namespaceURI, content) -> (true|false)
 *
 * Writes a full namespaced attribute, all at once. Returns false on failure.
 * Same as start_attribute_ns(prefix, name, namespaceURI) +
 * write_string(content) + end_attribute.
 *
 * Notes:
 * - by default, the xmlns: definition is repeated on every element. If you want
 * the prefix, but don't want the xmlns: declaration repeated, set +namespaceURI+
 * to nil or omit it. Don't forget to declare the namespace prefix somewhere
 * earlier.
 * - +content+ can be omitted too for an empty attribute
 */
static VALUE rxml_writer_write_attribute_ns(int argc, VALUE *argv, VALUE self)
{
    VALUE prefix, name, namespaceURI, content;

    content = namespaceURI = Qnil; // does empty content makes sense here?
    rb_scan_args(argc, argv, "22", &prefix, &name, &namespaceURI, &content);

    return numeric_rxml_writer_va_strings(self, Qnil, 4, xmlTextWriterWriteAttributeNS, prefix, name, namespaceURI, content);
}

/* call-seq:
 *    XML::Writer.write_pi(target, content) -> (true|false)
 *
 * Writes a full CDATA tag, all at once. Returns false on failure.
 * This is a shortcut for start_pi(target) + write_string(content) + end_pi.
 */
static VALUE rxml_writer_write_pi(VALUE self, VALUE target, VALUE content)
{
    return numeric_rxml_writer_va_strings(self, Qnil, 2, xmlTextWriterWritePI, target, content);
}

/* ===== public start/end interface ===== */

/* call-seq:
 *    XML::Writer.write_string(content) -> (true|false)
 *
 * Safely (problematic characters are internally translated to their
 * associated named entities) writes a string into the current node
 * (attribute, element, comment, ...). Returns false on failure.
 */
static VALUE rxml_writer_write_string(VALUE self, VALUE content)
{
    return numeric_rxml_writer_string(self, content, xmlTextWriterWriteString);
}

/* call-seq:
 *    XML::Writer.write_raw(content) -> (true|false)
 *
 * Writes the string +content+ as is, reserved characters are not
 * translated to their associated entities. Returns false on failure.
 * Consider write_string to handle them.
 */
static VALUE rxml_writer_write_raw(VALUE self, VALUE content)
{
    return numeric_rxml_writer_string(self, content, xmlTextWriterWriteRaw);
}

/* call-seq:
 *    XML::Writer.start_attribute(name) -> (true|false)
 *
 * Starts an attribute. Returns false on failure.
 */
static VALUE rxml_writer_start_attribute(VALUE self, VALUE name)
{
    return numeric_rxml_writer_string(self, name, xmlTextWriterStartAttribute);
}

/* call-seq:
 *    XML::Writer.start_attribute_ns(prefix, name, namespaceURI) -> (true|false)
 *
 * Starts a namespaced attribute. Returns false on failure.
 *
 * Note: by default, the xmlns: definition is repeated on every element. If
 * you want the prefix, but don't want the xmlns: declaration repeated, set
 * +namespaceURI+ to nil or omit it. Don't forget to declare the namespace
 * prefix somewhere earlier.
 */
static VALUE rxml_writer_start_attribute_ns(int argc, VALUE *argv, VALUE self)
{
    VALUE prefix, name, namespaceURI;

    namespaceURI = Qnil;
    rb_scan_args(argc, argv, "21", &prefix, &name, &namespaceURI);

    return numeric_rxml_writer_va_strings(self, Qnil, 3, xmlTextWriterStartAttributeNS, prefix, name, namespaceURI);
}

/* call-seq:
 *    XML::Writer.end_attribute -> (true|false)
 *
 * Ends an attribute, namespaced or not. Returns false on failure.
 */
static VALUE rxml_writer_end_attribute(VALUE self)
{
    return numeric_rxml_writer_void(self, xmlTextWriterEndAttribute);
}

# if LIBXML_VERSION >= 20607
/* call-seq:
 *    XML::Writer.start_comment -> (true|false)
 *
 * Starts a comment. Returns false on failure.
 * Note: libxml2 >= 2.6.7 required
 */
static VALUE rxml_writer_start_comment(VALUE self)
{
    return numeric_rxml_writer_void(self, xmlTextWriterStartComment);
}

/* call-seq:
 *    XML::Writer.end_comment -> (true|false)
 *
 * Ends current comment, returns false on failure.
 * Note: libxml2 >= 2.6.7 required
 */
static VALUE rxml_writer_end_comment(VALUE self)
{
    return numeric_rxml_writer_void(self, xmlTextWriterEndComment);
}
# endif /* LIBXML_VERSION >= 20607 */

/* call-seq:
 *    XML::Writer.start_element(name) -> (true|false)
 *
 * Starts a new element. Returns false on failure.
 */
static VALUE rxml_writer_start_element(VALUE self, VALUE name)
{
    return numeric_rxml_writer_string(self, name, xmlTextWriterStartElement);
}

/* call-seq:
 *    XML::Writer.start_element_ns(prefix, name, namespaceURI) -> (true|false)
 *
 * Starts a new namespaced element. Returns false on failure.
 *
 * Note: by default, the xmlns: definition is repeated on every element. If
 * you want the prefix, but don't want the xmlns: declaration repeated, set
 * +namespaceURI+ to nil or omit it. Don't forget to declare the namespace
 * prefix somewhere earlier.
 */
static VALUE rxml_writer_start_element_ns(int argc, VALUE *argv, VALUE self)
{
    VALUE prefix, name, namespaceURI;

    namespaceURI = Qnil;
    rb_scan_args(argc, argv, "21", &prefix, &name, &namespaceURI);

    return numeric_rxml_writer_va_strings(self, Qnil, 3, xmlTextWriterStartElementNS, prefix, name, namespaceURI);
}

/* call-seq:
 *    XML::Writer.end_element -> (true|false)
 *
 * Ends current element, namespaced or not. Returns false on failure.
 */
static VALUE rxml_writer_end_element(VALUE self)
{
    return numeric_rxml_writer_void(self, xmlTextWriterEndElement);
}

/* call-seq:
 *    XML::Writer.write_full_end_element -> (true|false)
 *
 * Ends current element, namespaced or not. Returns false on failure.
 * This method writes an end tag even if the element is empty (<foo></foo>),
 * end_element does not (<foo/>).
 */
static VALUE rxml_writer_full_end_element(VALUE self)
{
    return numeric_rxml_writer_void(self, xmlTextWriterFullEndElement);
}

/* call-seq:
 *    XML::Writer.start_cdata -> (true|false)
 *
 * Starts a new CDATA section. Returns false on failure.
 */
static VALUE rxml_writer_start_cdata(VALUE self)
{
    return numeric_rxml_writer_void(self, xmlTextWriterStartCDATA);
}

/* call-seq:
 *    XML::Writer.end_cdata -> (true|false)
 *
 * Ends current CDATA section. Returns false on failure.
 */
static VALUE rxml_writer_end_cdata(VALUE self)
{
    return numeric_rxml_writer_void(self, xmlTextWriterEndCDATA);
}

/* call-seq:
 *    XML::Writer.start_document -> (true|false)
 *    XML::Writer.start_document(:encoding => XML::Encoding::UTF_8,
 *      :standalone => true) -> (true|false)
 *
 * Starts a new document. Returns false on failure.
 *
 * You may provide an optional hash table to control XML header that will be
 * generated. Valid options are:
 *
 * - encoding: the output document encoding, defaults to nil (= UTF-8). Valid
 *             values are the encoding constants defined on XML::Encoding
 * - standalone: nil (default) or a boolean to indicate if the document is
 *               standalone or not
 */
static VALUE rxml_writer_start_document(int argc, VALUE *argv, VALUE self)
{
    int ret;
    VALUE options;
    rxml_writer_object *rwo;
    const char *xencoding, *xstandalone;

    options = Qnil;
    xstandalone = xencoding = NULL;
    rb_scan_args(argc, argv, "01", &options);
    if (!NIL_P(options)) {
        VALUE encoding, standalone;

        encoding = standalone = Qnil;
        Check_Type(options, T_HASH);
        encoding = rb_hash_aref(options, sEncoding);
        xencoding = NIL_P(encoding) ? NULL : xmlGetCharEncodingName(NUM2INT(encoding));
        standalone = rb_hash_aref(options, sStandalone);
        if (NIL_P(standalone)) {
            xstandalone = NULL;
        } else {
            xstandalone = RTEST(standalone) ? "yes" : "no";
        }
    }
    rwo = rxml_textwriter_get(self);
    ret = xmlTextWriterStartDocument(rwo->writer, NULL, xencoding, xstandalone);

    return (-1 == ret ? Qfalse : Qtrue);
}

/* call-seq:
 *    XML::Writer.end_document -> (true|false)
 *
 * Ends current document. Returns false on failure.
 */
static VALUE rxml_writer_end_document(VALUE self)
{
    return numeric_rxml_writer_void(self, xmlTextWriterEndDocument);
}

/* call-seq:
 *    XML::Writer.start_pi(target) -> (true|false)
 *
 * Starts a new processing instruction. Returns false on failure.
 */
static VALUE rxml_writer_start_pi(VALUE self, VALUE target)
{
    return numeric_rxml_writer_string(self, target, xmlTextWriterStartPI);
}

/* call-seq:
 *    XML::Writer.end_pi -> (true|false)
 *
 * Ends current processing instruction. Returns false on failure.
 */
static VALUE rxml_writer_end_pi(VALUE self)
{
    return numeric_rxml_writer_void(self, xmlTextWriterEndPI);
}

/* call-seq:
 *    XML::Writer.start_dtd(qualifiedName, publicId, systemId) -> (true|false)
 *
 * Starts a DTD. Returns false on failure.
 *   writer.start_dtd 'html' # => <!DOCTYPE html>
 */
static VALUE rxml_writer_start_dtd(int argc, VALUE *argv, VALUE self)
{
    VALUE name, pubid, sysid;

    pubid = sysid = Qnil;
    rb_scan_args(argc, argv, "12", &name, &pubid, &sysid);

    return numeric_rxml_writer_va_strings(self, Qnil, 3, xmlTextWriterStartDTD, name, pubid, sysid);
}

/* call-seq:
 *    XML::Writer.start_dtd_element(qualifiedName) -> (true|false)
 *
 * Starts a DTD element (<!ELEMENT ... >). Returns false on failure.
 */
static VALUE rxml_writer_start_dtd_element(VALUE self, VALUE name)
{
    return numeric_rxml_writer_string(self, name, xmlTextWriterStartDTDElement);
}

/* call-seq:
 *    XML::Writer.start_dtd_entity(pe, name) -> (true|false)
 *
 * Starts a DTD entity (<!ENTITY ... >). Returns false on failure.
 */
static VALUE rxml_writer_start_dtd_entity(VALUE self, VALUE pe, VALUE name)
{
    return numeric_rxml_writer_va_strings(self, Qnil, 1, xmlTextWriterStartDTDEntity, name);
}

/* call-seq:
 *    XML::Writer.start_dtd_attlist(name) -> (true|false)
 *
 * Starts a DTD attribute list (<!ATTLIST ... >). Returns false on failure.
 */
static VALUE rxml_writer_start_dtd_attlist(VALUE self, VALUE name)
{
    return numeric_rxml_writer_string(self, name, xmlTextWriterStartDTDAttlist);
}

/* call-seq:
 *    XML::Writer.end_dtd -> (true|false)
 *
 * Ends of DTD, returns false on failure.
 */
static VALUE rxml_writer_end_dtd(VALUE self)
{
    return numeric_rxml_writer_void(self, xmlTextWriterEndDTD);
}

/* call-seq:
 *    XML::Writer.end_dtd_entity -> (true|false)
 *
 * Ends of current DTD entity, returns false on failure.
 */
static VALUE rxml_writer_end_dtd_entity(VALUE self)
{
    return numeric_rxml_writer_void(self, xmlTextWriterEndDTDEntity);
}

/* call-seq:
 *    XML::Writer.end_dtd_attlist -> (true|false)
 */
static VALUE rxml_writer_end_dtd_attlist(VALUE self)
{
    return numeric_rxml_writer_void(self, xmlTextWriterEndDTDAttlist);
}

/* call-seq:
 *    XML::Writer.end_dtd_element -> (true|false)
 */
static VALUE rxml_writer_end_dtd_element(VALUE self)
{
    return numeric_rxml_writer_void(self, xmlTextWriterEndDTDElement);
}

/* call-seq:
 *    XML::Writer.write_dtd(...) -> (true|false)
 */
static VALUE rxml_writer_write_dtd(int argc, VALUE *argv, VALUE self)
{
    VALUE name, pubid, sysid, subset;

    pubid = sysid = subset = Qnil;
    rb_scan_args(argc, argv, "13", &name, &pubid, &sysid, &subset);

    return numeric_rxml_writer_va_strings(self, Qnil, 4, xmlTextWriterWriteDTD, name, pubid, sysid, subset);
}

/* call-seq:
 *    XML::Writer.write_dtd...(...) -> (true|false)
 */
static VALUE rxml_writer_write_dtd_attlist(VALUE self, VALUE name, VALUE content)
{
    return numeric_rxml_writer_va_strings(self, Qnil, 2, xmlTextWriterWriteDTDAttlist, name, content);
}

/* call-seq:
 *    XML::Writer.write_dtd...(...) -> (true|false)
 */
static VALUE rxml_writer_write_dtd_element(VALUE self, VALUE name, VALUE content)
{
    return numeric_rxml_writer_va_strings(self, Qnil, 2, xmlTextWriterWriteDTDElement, name, content);
}

/* call-seq:
 *    XML::Writer.write_dtd...(...) -> (true|false)
 */
static VALUE rxml_writer_write_dtd_entity(VALUE self, VALUE pe, VALUE name, VALUE pubid, VALUE sysid, VALUE ndataid, VALUE content)
{
    return numeric_rxml_writer_va_strings(self, pe, 5, xmlTextWriterWriteDTDEntity, name, pubid, sysid, ndataid, content);
}

/* call-seq:
 *    XML::Writer.write_dtd...(...) -> (true|false)
 */
static VALUE rxml_writer_write_dtd_enternal_entity(VALUE self, VALUE pe, VALUE name, VALUE pubid, VALUE sysid, VALUE ndataid)
{
    return numeric_rxml_writer_va_strings(self, pe, 4, xmlTextWriterWriteDTDExternalEntity, name, pubid, sysid, ndataid);
}

/* call-seq:
 *    XML::Writer.write_dtd...(...) -> (true|false)
 */
static VALUE rxml_writer_write_dtd_external_entity_contents(VALUE self, VALUE pubid, VALUE sysid, VALUE ndataid)
{
    return numeric_rxml_writer_va_strings(self, Qnil, 3, xmlTextWriterWriteDTDExternalEntityContents, pubid, sysid, ndataid);
}

/* call-seq:
 *    XML::Writer.write_dtd...(...) -> (true|false)
 */
static VALUE rxml_writer_write_dtd_internal_entity(VALUE self, VALUE pe, VALUE name, VALUE content)
{
    return numeric_rxml_writer_va_strings(self, pe, 2, xmlTextWriterWriteDTDInternalEntity, name, content);
}

/* call-seq:
 *    XML::Writer.write_dtd...(...) -> (true|false)
 */
static VALUE rxml_writer_write_dtd_notation(VALUE self, VALUE name, VALUE pubid, VALUE sysid)
{
    return numeric_rxml_writer_va_strings(self, Qnil, 3, xmlTextWriterWriteDTDNotation, name, pubid, sysid);
}

# if LIBXML_VERSION > 20900
/* call-seq:
 *    XML::Writer.set_quote_char(...) -> (true|false)
 *
 * Sets the character used for quoting attributes. Returns false on failure.
 * Notes:
 * - only " (default) and ' are valid
 * - implies libxml2 >= 2.9.0
 */
static VALUE rxml_writer_set_quote_char(VALUE self, VALUE quotechar)
{
    int ret;
    const char *xquotechar;
    rxml_writer_object *rwo;

    rwo = rxml_textwriter_get(self);
    xquotechar = StringValueCStr(quotechar);
    ret = xmlTextWriterSetQuoteChar(rwo->writer, (xmlChar) xquotechar[0]);

    return (-1 == ret ? Qfalse : Qtrue);
}
# endif

# define RXMLW_IM(numargs, method, cfunc) \
    rb_define_method(cXMLWriter, method, cfunc, numargs)

/* grep -P 'xmlTextWriter(Start|End|Write)(?!DTD|V?Format)[^(]+' /usr/include/libxml2/libxml/xmlwriter.h */
void rxml_init_writer(void)
{
    sEncoding = ID2SYM(rb_intern("encoding"));
    sStandalone = ID2SYM(rb_intern("standalone"));

    cXMLWriter = rb_define_class_under(mXML, "Writer", rb_cObject);

    rb_define_singleton_method(cXMLWriter, "io", rxml_writer_io, 1);
    rb_define_singleton_method(cXMLWriter, "file", rxml_writer_file, 1);
    rb_define_singleton_method(cXMLWriter, "document", rxml_writer_doc, 0);
    rb_define_singleton_method(cXMLWriter, "string", rxml_writer_string, 0);

    /* misc */
# if LIBXML_VERSION > 20604
    RXMLW_IM(1, "set_indent", rxml_writer_set_indent);
    RXMLW_IM(1, "set_indent_string", rxml_writer_set_indent_string);
# endif /* LIBXML_VERSION >= 20604 */
# if LIBXML_VERSION > 20900
    RXMLW_IM(1, "set_quote_char", rxml_writer_set_quote_char);
# endif  /* LIBXML_VERSION >= 20900 */
    RXMLW_IM(0, "flush", rxml_writer_flush);
    RXMLW_IM(-1, "start_dtd", rxml_writer_start_dtd);
    RXMLW_IM(2, "start_dtd_entity", rxml_writer_start_dtd_entity);
    RXMLW_IM(1, "start_dtd_attlist", rxml_writer_start_dtd_attlist);
    RXMLW_IM(1, "start_dtd_element", rxml_writer_start_dtd_element);
    RXMLW_IM(-1, "write_dtd", rxml_writer_write_dtd);
    RXMLW_IM(2, "write_dtd_attlist", rxml_writer_write_dtd_attlist);
    RXMLW_IM(2, "write_dtd_element", rxml_writer_write_dtd_element);
    RXMLW_IM(6, "write_dtd_entity", rxml_writer_write_dtd_entity);
    RXMLW_IM(5, "write_dtd_external_entity", rxml_writer_write_dtd_enternal_entity);
    RXMLW_IM(3, "write_dtd_external_entity_contents", rxml_writer_write_dtd_external_entity_contents);
    RXMLW_IM(2, "write_dtd_internal_entity", rxml_writer_write_dtd_internal_entity);
    RXMLW_IM(3, "write_dtd_notation", rxml_writer_write_dtd_notation);
    RXMLW_IM(0, "end_dtd", rxml_writer_end_dtd);
    RXMLW_IM(0, "end_dtd_entity", rxml_writer_end_dtd_entity);
    RXMLW_IM(0, "end_dtd_attlist", rxml_writer_end_dtd_attlist);
    RXMLW_IM(0, "end_dtd_element", rxml_writer_end_dtd_element);

    /* tag by parts */
    RXMLW_IM(1, "write_raw", rxml_writer_write_raw);
    RXMLW_IM(1, "write_string", rxml_writer_write_string);

    RXMLW_IM(0, "start_cdata", rxml_writer_start_cdata);
    RXMLW_IM(0, "end_cdata", rxml_writer_end_cdata);
    RXMLW_IM(1, "start_attribute", rxml_writer_start_attribute);
    RXMLW_IM(-1, "start_attribute_ns", rxml_writer_start_attribute_ns);
    RXMLW_IM(0, "end_attribute", rxml_writer_end_attribute);
    RXMLW_IM(1, "start_element", rxml_writer_start_element);
    RXMLW_IM(-1, "start_element_ns", rxml_writer_start_element_ns);
    RXMLW_IM(0, "end_element", rxml_writer_end_element);
    RXMLW_IM(0, "full_end_element", rxml_writer_full_end_element);
    RXMLW_IM(-1, "start_document", rxml_writer_start_document);
    RXMLW_IM(0, "end_document", rxml_writer_end_document);
# if LIBXML_VERSION > 20606
    RXMLW_IM(0, "start_comment", rxml_writer_start_comment);
    RXMLW_IM(0, "end_comment", rxml_writer_end_comment);
# endif /* LIBXML_VERSION > 20606 */
    RXMLW_IM(1, "start_pi", rxml_writer_start_pi);
    RXMLW_IM(0, "end_pi", rxml_writer_end_pi);

    /* full tag at once */
    RXMLW_IM(2, "write_attribute", rxml_writer_write_attribute);
    RXMLW_IM(-1, "write_attribute_ns", rxml_writer_write_attribute_ns);
    RXMLW_IM(1, "write_comment", rxml_writer_write_comment);
    RXMLW_IM(1, "write_cdata", rxml_writer_write_cdata);
    RXMLW_IM(-1, "write_element", rxml_writer_write_element);
    RXMLW_IM(-1, "write_element_ns", rxml_writer_write_element_ns);
    RXMLW_IM(2, "write_pi", rxml_writer_write_pi);

    RXMLW_IM(0, "result", rxml_writer_result);
}

#endif /* LIBXML_WRITER_ENABLED */
