#include "ruby_libxml.h"
#include "ruby_xml_dtd.h"
#include "ruby_xml_error.h"

#include <libxml/xmlerror.h>

VALUE eXMLError;
static ID ERROR_HANDLER_ID;


/* Hook that receives xml error message */
static void
structuredErrorFunc(void *userData, xmlErrorPtr xerror)
{
  /* Wrap error up as Ruby object and send it off to ruby */
  VALUE block = rb_cvar_get(eXMLError, ERROR_HANDLER_ID);

  if (block != Qnil)
  {
    VALUE error = ruby_xml_error_wrap(xerror);
    rb_funcall(block, rb_intern("call"), 1, error);
  }
}

/*
 * call-seq:
 *    Error.set_error_handler { ... }
 *
 * Register a callback block that will be called with an instance of
 * an error message.
 */
static VALUE
ruby_xml_error_set_handler(VALUE self)
{
  VALUE block;

  if (rb_block_given_p() == Qfalse)
    rb_raise(rb_eRuntimeError, "No block given");

  block = rb_block_proc();
 
  /* Embed the block within the Error class to avoid it to be collected.
     Previous handler will be overwritten if it exists. */
  rb_cvar_set(self, ERROR_HANDLER_ID, block, 0); 
  
  return self;
}

/*
 * call-seq:
 *    Error.reset_error_handler
 *
 * Restores the default error and warning handlers.
 */
static VALUE
ruby_xml_error_reset_handler(VALUE self)
{
  rb_cvar_set(self, ERROR_HANDLER_ID, Qnil, 0); 
  return self;
}


VALUE
ruby_xml_error_wrap(xmlErrorPtr xerror) {
  VALUE result = Qnil;
  if (xerror->message)
    result = rb_exc_new2(eXMLError, xerror->message);
  else
    result = rb_class_new_instance(0, NULL, eXMLError);

  rb_iv_set(result, "@domain", INT2NUM(xerror->domain));
  rb_iv_set(result, "@code", INT2NUM(xerror->code));
  rb_iv_set(result, "@level", INT2NUM(xerror->level));
  
  if (xerror->file)
    rb_iv_set(result, "@file", rb_str_new2(xerror->file));
  
  if (xerror->line)
    rb_iv_set(result, "@line", INT2NUM(xerror->line));
  
  if (xerror->str1)
    rb_iv_set(result, "@str1", rb_str_new2(xerror->str1));
  
  if (xerror->str2)
    rb_iv_set(result, "@str2", rb_str_new2(xerror->str2));
  
  if (xerror->str3)
    rb_iv_set(result, "@str3", rb_str_new2(xerror->str3));
  
  rb_iv_set(result, "@int1", INT2NUM(xerror->int1));
  rb_iv_set(result, "@int2", INT2NUM(xerror->int2));

  //rb_define_attr(eXMLError, "ctxt", 1, 0);
  if (xerror->node) {
    VALUE node = ruby_xml_node2_wrap(cXMLNode, xerror->node);
    rb_iv_set(result, "@node", node);
  }
  return result;
}


// Rdoc needs to know 
#ifdef RDOC_NEVER_DEFINED
  mLibXML = rb_define_module("LibXML");
  mXML = rb_define_module_under(mLibXML, "XML");
#endif

void
ruby_xml_raise(xmlErrorPtr xerror)
{
  /* Wrap error up as Ruby object and send it off to ruby */
  VALUE error = ruby_xml_error_wrap(xerror);
  rb_exc_raise(error);
}


void
ruby_init_xml_error() {
  /* Intercept libxml error handlers */
  xmlSetStructuredErrorFunc(NULL, structuredErrorFunc);

  /* Error class */
  eXMLError = rb_define_class_under(mXML, "Error", rb_eStandardError);
  rb_define_singleton_method(eXMLError, "set_handler", ruby_xml_error_set_handler, 0);
  rb_define_singleton_method(eXMLError, "reset_handler", ruby_xml_error_reset_handler, 0);


  /* Ruby callback to receive errors - set it to nil by default. */
  ERROR_HANDLER_ID = rb_intern("@@__error_handler_callback__"); 
  rb_cvar_set(eXMLError, ERROR_HANDLER_ID, Qnil, 0);

  /* Error attributes */
  rb_define_attr(eXMLError, "level", 1, 0);
  rb_define_attr(eXMLError, "domain", 1, 0);
  rb_define_attr(eXMLError, "code", 1, 0);
  rb_define_attr(eXMLError, "file", 1, 0);
  rb_define_attr(eXMLError, "line", 1, 0);
  rb_define_attr(eXMLError, "str1", 1, 0);
  rb_define_attr(eXMLError, "str2", 1, 0);
  rb_define_attr(eXMLError, "str3", 1, 0);
  rb_define_attr(eXMLError, "int1", 1, 0);
  rb_define_attr(eXMLError, "int2", 1, 0);
  rb_define_attr(eXMLError, "ctxt", 1, 0);
  rb_define_attr(eXMLError, "node", 1, 0);

  /* xml error levels */
  rb_define_const(eXMLError, "XML_ERR_NONE",    INT2NUM(0));
  rb_define_const(eXMLError, "XML_ERR_WARNING", INT2NUM(1));
  rb_define_const(eXMLError, "XML_ERR_ERROR",   INT2NUM(2));
  rb_define_const(eXMLError, "XML_ERR_FATAL",   INT2NUM(3));

  /* xml error domains */
  rb_define_const(eXMLError, "XML_FROM_NONE", INT2NUM(0));
  rb_define_const(eXMLError, "XML_FROM_PARSER", INT2NUM(1));
  rb_define_const(eXMLError, "XML_FROM_TREE", INT2NUM(2));
  rb_define_const(eXMLError, "XML_FROM_NAMESPACE", INT2NUM(3));
  rb_define_const(eXMLError, "XML_FROM_DTD", INT2NUM(4));
  rb_define_const(eXMLError, "XML_FROM_HTML", INT2NUM(5));
  rb_define_const(eXMLError, "XML_FROM_MEMORY", INT2NUM(6));
  rb_define_const(eXMLError, "XML_FROM_OUTPUT", INT2NUM(7));
  rb_define_const(eXMLError, "XML_FROM_IO", INT2NUM(8));
  rb_define_const(eXMLError, "XML_FROM_FTP", INT2NUM(9));
  rb_define_const(eXMLError, "XML_FROM_HTTP", INT2NUM(10));
  rb_define_const(eXMLError, "XML_FROM_XINCLUDE", INT2NUM(11));
  rb_define_const(eXMLError, "XML_FROM_XPATH", INT2NUM(12));
  rb_define_const(eXMLError, "XML_FROM_XPOINTER", INT2NUM(13));
  rb_define_const(eXMLError, "XML_FROM_REGEXP", INT2NUM(14));
  rb_define_const(eXMLError, "XML_FROM_DATATYPE", INT2NUM(15));
  rb_define_const(eXMLError, "XML_FROM_SCHEMASP", INT2NUM(16));
  rb_define_const(eXMLError, "XML_FROM_SCHEMASV", INT2NUM(17));
  rb_define_const(eXMLError, "XML_FROM_RELAXNGP", INT2NUM(18));
  rb_define_const(eXMLError, "XML_FROM_RELAXNGV", INT2NUM(19));
  rb_define_const(eXMLError, "XML_FROM_CATALOG", INT2NUM(20));
  rb_define_const(eXMLError, "XML_FROM_C14N", INT2NUM(21));
  rb_define_const(eXMLError, "XML_FROM_XSLT", INT2NUM(22));
  rb_define_const(eXMLError, "XML_FROM_VALID", INT2NUM(23));
  rb_define_const(eXMLError, "XML_FROM_CHECK", INT2NUM(24));
  rb_define_const(eXMLError, "XML_FROM_WRITER", INT2NUM(25));
  rb_define_const(eXMLError, "XML_FROM_MODULE", INT2NUM(26));
  rb_define_const(eXMLError, "XML_FROM_I18N", INT2NUM(27));
  rb_define_const(eXMLError, "XML_FROM_SCHEMATRONV", INT2NUM(28));

  /* errors */
  rb_define_const(eXMLError, "XML_ERR_OK", INT2NUM(0));
  rb_define_const(eXMLError, "XML_ERR_INTERNAL_ERROR", INT2NUM(1));
  rb_define_const(eXMLError, "XML_ERR_NO_MEMORY", INT2NUM(2));
  rb_define_const(eXMLError, "XML_ERR_DOCUMENT_START", INT2NUM(3));
  rb_define_const(eXMLError, "XML_ERR_DOCUMENT_EMPTY", INT2NUM(4));
  rb_define_const(eXMLError, "XML_ERR_DOCUMENT_END", INT2NUM(5));
  rb_define_const(eXMLError, "XML_ERR_INVALID_HEX_CHARREF", INT2NUM(6));
  rb_define_const(eXMLError, "XML_ERR_INVALID_DEC_CHARREF", INT2NUM(7));
  rb_define_const(eXMLError, "XML_ERR_INVALID_CHARREF", INT2NUM(8));
  rb_define_const(eXMLError, "XML_ERR_INVALID_CHAR", INT2NUM(9));
  rb_define_const(eXMLError, "XML_ERR_CHARREF_AT_EOF", INT2NUM(10));
  rb_define_const(eXMLError, "XML_ERR_CHARREF_IN_PROLOG", INT2NUM(11));
  rb_define_const(eXMLError, "XML_ERR_CHARREF_IN_EPILOG", INT2NUM(12));
  rb_define_const(eXMLError, "XML_ERR_CHARREF_IN_DTD", INT2NUM(13));
  rb_define_const(eXMLError, "XML_ERR_ENTITYREF_AT_EOF", INT2NUM(14));
  rb_define_const(eXMLError, "XML_ERR_ENTITYREF_IN_PROLOG", INT2NUM(15));
  rb_define_const(eXMLError, "XML_ERR_ENTITYREF_IN_EPILOG", INT2NUM(16));
  rb_define_const(eXMLError, "XML_ERR_ENTITYREF_IN_DTD", INT2NUM(17));
  rb_define_const(eXMLError, "XML_ERR_PEREF_AT_EOF", INT2NUM(18));
  rb_define_const(eXMLError, "XML_ERR_PEREF_IN_PROLOG", INT2NUM(19));
  rb_define_const(eXMLError, "XML_ERR_PEREF_IN_EPILOG", INT2NUM(20));
  rb_define_const(eXMLError, "XML_ERR_PEREF_IN_INT_SUBSET", INT2NUM(21));
  rb_define_const(eXMLError, "XML_ERR_ENTITYREF_NO_NAME", INT2NUM(22));
  rb_define_const(eXMLError, "XML_ERR_ENTITYREF_SEMICOL_MISSING", INT2NUM(23));
  rb_define_const(eXMLError, "XML_ERR_PEREF_NO_NAME", INT2NUM(24));
  rb_define_const(eXMLError, "XML_ERR_PEREF_SEMICOL_MISSING", INT2NUM(25));
  rb_define_const(eXMLError, "XML_ERR_UNDECLARED_ENTITY", INT2NUM(26));
  rb_define_const(eXMLError, "XML_WAR_UNDECLARED_ENTITY", INT2NUM(27));
  rb_define_const(eXMLError, "XML_ERR_UNPARSED_ENTITY", INT2NUM(28));
  rb_define_const(eXMLError, "XML_ERR_ENTITY_IS_EXTERNAL", INT2NUM(29));
  rb_define_const(eXMLError, "XML_ERR_ENTITY_IS_PARAMETER", INT2NUM(30));
  rb_define_const(eXMLError, "XML_ERR_UNKNOWN_ENCODING", INT2NUM(31));
  rb_define_const(eXMLError, "XML_ERR_UNSUPPORTED_ENCODING", INT2NUM(32));
  rb_define_const(eXMLError, "XML_ERR_STRING_NOT_STARTED", INT2NUM(33));
  rb_define_const(eXMLError, "XML_ERR_STRING_NOT_CLOSED", INT2NUM(34));
  rb_define_const(eXMLError, "XML_ERR_NS_DECL_ERROR", INT2NUM(35));
  rb_define_const(eXMLError, "XML_ERR_ENTITY_NOT_STARTED", INT2NUM(36));
  rb_define_const(eXMLError, "XML_ERR_ENTITY_NOT_FINISHED", INT2NUM(37));
  rb_define_const(eXMLError, "XML_ERR_LT_IN_ATTRIBUTE", INT2NUM(38));
  rb_define_const(eXMLError, "XML_ERR_ATTRIBUTE_NOT_STARTED", INT2NUM(39));
  rb_define_const(eXMLError, "XML_ERR_ATTRIBUTE_NOT_FINISHED", INT2NUM(40));
  rb_define_const(eXMLError, "XML_ERR_ATTRIBUTE_WITHOUT_VALUE", INT2NUM(41));
  rb_define_const(eXMLError, "XML_ERR_ATTRIBUTE_REDEFINED", INT2NUM(42));
  rb_define_const(eXMLError, "XML_ERR_LITERAL_NOT_STARTED", INT2NUM(43));
  rb_define_const(eXMLError, "XML_ERR_LITERAL_NOT_FINISHED", INT2NUM(44));
  rb_define_const(eXMLError, "XML_ERR_COMMENT_NOT_FINISHED", INT2NUM(45));
  rb_define_const(eXMLError, "XML_ERR_PI_NOT_STARTED", INT2NUM(46));
  rb_define_const(eXMLError, "XML_ERR_PI_NOT_FINISHED", INT2NUM(47));
  rb_define_const(eXMLError, "XML_ERR_NOTATION_NOT_STARTED", INT2NUM(48));
  rb_define_const(eXMLError, "XML_ERR_NOTATION_NOT_FINISHED", INT2NUM(49));
  rb_define_const(eXMLError, "XML_ERR_ATTLIST_NOT_STARTED", INT2NUM(50));
  rb_define_const(eXMLError, "XML_ERR_ATTLIST_NOT_FINISHED", INT2NUM(51));
  rb_define_const(eXMLError, "XML_ERR_MIXED_NOT_STARTED", INT2NUM(52));
  rb_define_const(eXMLError, "XML_ERR_MIXED_NOT_FINISHED", INT2NUM(53));
  rb_define_const(eXMLError, "XML_ERR_ELEMCONTENT_NOT_STARTED", INT2NUM(54));
  rb_define_const(eXMLError, "XML_ERR_ELEMCONTENT_NOT_FINISHED", INT2NUM(55));
  rb_define_const(eXMLError, "XML_ERR_XMLDECL_NOT_STARTED", INT2NUM(56));
  rb_define_const(eXMLError, "XML_ERR_XMLDECL_NOT_FINISHED", INT2NUM(57));
  rb_define_const(eXMLError, "XML_ERR_CONDSEC_NOT_STARTED", INT2NUM(58));
  rb_define_const(eXMLError, "XML_ERR_CONDSEC_NOT_FINISHED", INT2NUM(59));
  rb_define_const(eXMLError, "XML_ERR_EXT_SUBSET_NOT_FINISHED", INT2NUM(60));
  rb_define_const(eXMLError, "XML_ERR_DOCTYPE_NOT_FINISHED", INT2NUM(61));
  rb_define_const(eXMLError, "XML_ERR_MISPLACED_CDATA_END", INT2NUM(62));
  rb_define_const(eXMLError, "XML_ERR_CDATA_NOT_FINISHED", INT2NUM(63));
  rb_define_const(eXMLError, "XML_ERR_RESERVED_XML_NAME", INT2NUM(64));
  rb_define_const(eXMLError, "XML_ERR_SPACE_REQUIRED", INT2NUM(65));
  rb_define_const(eXMLError, "XML_ERR_SEPARATOR_REQUIRED", INT2NUM(66));
  rb_define_const(eXMLError, "XML_ERR_NMTOKEN_REQUIRED", INT2NUM(67));
  rb_define_const(eXMLError, "XML_ERR_NAME_REQUIRED", INT2NUM(68));
  rb_define_const(eXMLError, "XML_ERR_PCDATA_REQUIRED", INT2NUM(69));
  rb_define_const(eXMLError, "XML_ERR_URI_REQUIRED", INT2NUM(70));
  rb_define_const(eXMLError, "XML_ERR_PUBID_REQUIRED", INT2NUM(71));
  rb_define_const(eXMLError, "XML_ERR_LT_REQUIRED", INT2NUM(72));
  rb_define_const(eXMLError, "XML_ERR_GT_REQUIRED", INT2NUM(73));
  rb_define_const(eXMLError, "XML_ERR_LTSLASH_REQUIRED", INT2NUM(74));
  rb_define_const(eXMLError, "XML_ERR_EQUAL_REQUIRED", INT2NUM(75));
  rb_define_const(eXMLError, "XML_ERR_TAG_NAME_MISMATCH", INT2NUM(76));
  rb_define_const(eXMLError, "XML_ERR_TAG_NOT_FINISHED", INT2NUM(77));
  rb_define_const(eXMLError, "XML_ERR_STANDALONE_VALUE", INT2NUM(78));
  rb_define_const(eXMLError, "XML_ERR_ENCODING_NAME", INT2NUM(79));
  rb_define_const(eXMLError, "XML_ERR_HYPHEN_IN_COMMENT", INT2NUM(80));
  rb_define_const(eXMLError, "XML_ERR_INVALID_ENCODING", INT2NUM(81));
  rb_define_const(eXMLError, "XML_ERR_EXT_ENTITY_STANDALONE", INT2NUM(82));
  rb_define_const(eXMLError, "XML_ERR_CONDSEC_INVALID", INT2NUM(83));
  rb_define_const(eXMLError, "XML_ERR_VALUE_REQUIRED", INT2NUM(84));
  rb_define_const(eXMLError, "XML_ERR_NOT_WELL_BALANCED", INT2NUM(85));
  rb_define_const(eXMLError, "XML_ERR_EXTRA_CONTENT", INT2NUM(86));
  rb_define_const(eXMLError, "XML_ERR_ENTITY_CHAR_ERROR", INT2NUM(87));
  rb_define_const(eXMLError, "XML_ERR_ENTITY_PE_INTERNAL", INT2NUM(88));
  rb_define_const(eXMLError, "XML_ERR_ENTITY_LOOP", INT2NUM(89));
  rb_define_const(eXMLError, "XML_ERR_ENTITY_BOUNDARY", INT2NUM(90));
  rb_define_const(eXMLError, "XML_ERR_INVALID_URI", INT2NUM(91));
  rb_define_const(eXMLError, "XML_ERR_URI_FRAGMENT", INT2NUM(92));
  rb_define_const(eXMLError, "XML_WAR_CATALOG_PI", INT2NUM(93));
  rb_define_const(eXMLError, "XML_ERR_NO_DTD", INT2NUM(94));
  rb_define_const(eXMLError, "XML_ERR_CONDSEC_INVALID_KEYWORD", INT2NUM(95));
  rb_define_const(eXMLError, "XML_ERR_VERSION_MISSING", INT2NUM(96));
  rb_define_const(eXMLError, "XML_WAR_UNKNOWN_VERSION", INT2NUM(97));
  rb_define_const(eXMLError, "XML_WAR_LANG_VALUE", INT2NUM(98));
  rb_define_const(eXMLError, "XML_WAR_NS_URI", INT2NUM(99));
  rb_define_const(eXMLError, "XML_WAR_NS_URI_RELATIVE", INT2NUM(100));
  rb_define_const(eXMLError, "XML_ERR_MISSING_ENCODING", INT2NUM(101));
  rb_define_const(eXMLError, "XML_WAR_SPACE_VALUE", INT2NUM(102));
  rb_define_const(eXMLError, "XML_ERR_NOT_STANDALONE", INT2NUM(103));
  rb_define_const(eXMLError, "XML_ERR_ENTITY_PROCESSING", INT2NUM(104));
  rb_define_const(eXMLError, "XML_ERR_NOTATION_PROCESSING", INT2NUM(105));
  rb_define_const(eXMLError, "XML_WAR_NS_COLUMN", INT2NUM(106));
  rb_define_const(eXMLError, "XML_WAR_ENTITY_REDEFINED", INT2NUM(107));
  rb_define_const(eXMLError, "XML_NS_ERR_XML_NAMESPACE", INT2NUM(200));
  rb_define_const(eXMLError, "XML_NS_ERR_UNDEFINED_NAMESPACE", INT2NUM(201));
  rb_define_const(eXMLError, "XML_NS_ERR_QNAME", INT2NUM(202));
  rb_define_const(eXMLError, "XML_NS_ERR_ATTRIBUTE_REDEFINED", INT2NUM(203));
  rb_define_const(eXMLError, "XML_NS_ERR_EMPTY", INT2NUM(204));
  rb_define_const(eXMLError, "XML_DTD_ATTRIBUTE_DEFAULT", INT2NUM(500));
  rb_define_const(eXMLError, "XML_DTD_ATTRIBUTE_REDEFINED", INT2NUM(501));
  rb_define_const(eXMLError, "XML_DTD_ATTRIBUTE_VALUE", INT2NUM(502));
  rb_define_const(eXMLError, "XML_DTD_CONTENT_ERROR", INT2NUM(503));
  rb_define_const(eXMLError, "XML_DTD_CONTENT_MODEL", INT2NUM(504));
  rb_define_const(eXMLError, "XML_DTD_CONTENT_NOT_DETERMINIST", INT2NUM(505));
  rb_define_const(eXMLError, "XML_DTD_DIFFERENT_PREFIX", INT2NUM(506));
  rb_define_const(eXMLError, "XML_DTD_ELEM_DEFAULT_NAMESPACE", INT2NUM(507));
  rb_define_const(eXMLError, "XML_DTD_ELEM_NAMESPACE", INT2NUM(508));
  rb_define_const(eXMLError, "XML_DTD_ELEM_REDEFINED", INT2NUM(509));
  rb_define_const(eXMLError, "XML_DTD_EMPTY_NOTATION", INT2NUM(510));
  rb_define_const(eXMLError, "XML_DTD_ENTITY_TYPE", INT2NUM(511));
  rb_define_const(eXMLError, "XML_DTD_ID_FIXED", INT2NUM(512));
  rb_define_const(eXMLError, "XML_DTD_ID_REDEFINED", INT2NUM(513));
  rb_define_const(eXMLError, "XML_DTD_ID_SUBSET", INT2NUM(514));
  rb_define_const(eXMLError, "XML_DTD_INVALID_CHILD", INT2NUM(515));
  rb_define_const(eXMLError, "XML_DTD_INVALID_DEFAULT", INT2NUM(516));
  rb_define_const(eXMLError, "XML_DTD_LOAD_ERROR", INT2NUM(517));
  rb_define_const(eXMLError, "XML_DTD_MISSING_ATTRIBUTE", INT2NUM(518));
  rb_define_const(eXMLError, "XML_DTD_MIXED_CORRUPT", INT2NUM(519));
  rb_define_const(eXMLError, "XML_DTD_MULTIPLE_ID", INT2NUM(520));
  rb_define_const(eXMLError, "XML_DTD_NO_DOC", INT2NUM(521));
  rb_define_const(eXMLError, "XML_DTD_NO_DTD", INT2NUM(522));
  rb_define_const(eXMLError, "XML_DTD_NO_ELEM_NAME", INT2NUM(523));
  rb_define_const(eXMLError, "XML_DTD_NO_PREFIX", INT2NUM(524));
  rb_define_const(eXMLError, "XML_DTD_NO_ROOT", INT2NUM(525));
  rb_define_const(eXMLError, "XML_DTD_NOTATION_REDEFINED", INT2NUM(526));
  rb_define_const(eXMLError, "XML_DTD_NOTATION_VALUE", INT2NUM(527));
  rb_define_const(eXMLError, "XML_DTD_NOT_EMPTY", INT2NUM(528));
  rb_define_const(eXMLError, "XML_DTD_NOT_PCDATA", INT2NUM(529));
  rb_define_const(eXMLError, "XML_DTD_NOT_STANDALONE", INT2NUM(530));
  rb_define_const(eXMLError, "XML_DTD_ROOT_NAME", INT2NUM(531));
  rb_define_const(eXMLError, "XML_DTD_STANDALONE_WHITE_SPACE", INT2NUM(532));
  rb_define_const(eXMLError, "XML_DTD_UNKNOWN_ATTRIBUTE", INT2NUM(533));
  rb_define_const(eXMLError, "XML_DTD_UNKNOWN_ELEM", INT2NUM(534));
  rb_define_const(eXMLError, "XML_DTD_UNKNOWN_ENTITY", INT2NUM(535));
  rb_define_const(eXMLError, "XML_DTD_UNKNOWN_ID", INT2NUM(536));
  rb_define_const(eXMLError, "XML_DTD_UNKNOWN_NOTATION", INT2NUM(537));
  rb_define_const(eXMLError, "XML_DTD_STANDALONE_DEFAULTED", INT2NUM(538));
  rb_define_const(eXMLError, "XML_DTD_XMLID_VALUE", INT2NUM(539));
  rb_define_const(eXMLError, "XML_DTD_XMLID_TYPE", INT2NUM(540));
  rb_define_const(eXMLError, "XML_HTML_STRUCURE_ERROR", INT2NUM(800));
  rb_define_const(eXMLError, "XML_HTML_UNKNOWN_TAG", INT2NUM(801));
  rb_define_const(eXMLError, "XML_RNGP_ANYNAME_ATTR_ANCESTOR", INT2NUM(1000));
  rb_define_const(eXMLError, "XML_RNGP_ATTR_CONFLICT", INT2NUM(1001));
  rb_define_const(eXMLError, "XML_RNGP_ATTRIBUTE_CHILDREN", INT2NUM(1002));
  rb_define_const(eXMLError, "XML_RNGP_ATTRIBUTE_CONTENT", INT2NUM(1003));
  rb_define_const(eXMLError, "XML_RNGP_ATTRIBUTE_EMPTY", INT2NUM(1004));
  rb_define_const(eXMLError, "XML_RNGP_ATTRIBUTE_NOOP", INT2NUM(1005));
  rb_define_const(eXMLError, "XML_RNGP_CHOICE_CONTENT", INT2NUM(1006));
  rb_define_const(eXMLError, "XML_RNGP_CHOICE_EMPTY", INT2NUM(1007));
  rb_define_const(eXMLError, "XML_RNGP_CREATE_FAILURE", INT2NUM(1008));
  rb_define_const(eXMLError, "XML_RNGP_DATA_CONTENT", INT2NUM(1009));
  rb_define_const(eXMLError, "XML_RNGP_DEF_CHOICE_AND_INTERLEAVE", INT2NUM(1010));
  rb_define_const(eXMLError, "XML_RNGP_DEFINE_CREATE_FAILED", INT2NUM(1011));
  rb_define_const(eXMLError, "XML_RNGP_DEFINE_EMPTY", INT2NUM(1012));
  rb_define_const(eXMLError, "XML_RNGP_DEFINE_MISSING", INT2NUM(1013));
  rb_define_const(eXMLError, "XML_RNGP_DEFINE_NAME_MISSING", INT2NUM(1014));
  rb_define_const(eXMLError, "XML_RNGP_ELEM_CONTENT_EMPTY", INT2NUM(1015));
  rb_define_const(eXMLError, "XML_RNGP_ELEM_CONTENT_ERROR", INT2NUM(1016));
  rb_define_const(eXMLError, "XML_RNGP_ELEMENT_EMPTY", INT2NUM(1017));
  rb_define_const(eXMLError, "XML_RNGP_ELEMENT_CONTENT", INT2NUM(1018));
  rb_define_const(eXMLError, "XML_RNGP_ELEMENT_NAME", INT2NUM(1019));
  rb_define_const(eXMLError, "XML_RNGP_ELEMENT_NO_CONTENT", INT2NUM(1020));
  rb_define_const(eXMLError, "XML_RNGP_ELEM_TEXT_CONFLICT", INT2NUM(1021));
  rb_define_const(eXMLError, "XML_RNGP_EMPTY", INT2NUM(1022));
  rb_define_const(eXMLError, "XML_RNGP_EMPTY_CONSTRUCT", INT2NUM(1023));
  rb_define_const(eXMLError, "XML_RNGP_EMPTY_CONTENT", INT2NUM(1024));
  rb_define_const(eXMLError, "XML_RNGP_EMPTY_NOT_EMPTY", INT2NUM(1025));
  rb_define_const(eXMLError, "XML_RNGP_ERROR_TYPE_LIB", INT2NUM(1026));
  rb_define_const(eXMLError, "XML_RNGP_EXCEPT_EMPTY", INT2NUM(1027));
  rb_define_const(eXMLError, "XML_RNGP_EXCEPT_MISSING", INT2NUM(1028));
  rb_define_const(eXMLError, "XML_RNGP_EXCEPT_MULTIPLE", INT2NUM(1029));
  rb_define_const(eXMLError, "XML_RNGP_EXCEPT_NO_CONTENT", INT2NUM(1030));
  rb_define_const(eXMLError, "XML_RNGP_EXTERNALREF_EMTPY", INT2NUM(1031));
  rb_define_const(eXMLError, "XML_RNGP_EXTERNAL_REF_FAILURE", INT2NUM(1032));
  rb_define_const(eXMLError, "XML_RNGP_EXTERNALREF_RECURSE", INT2NUM(1033));
  rb_define_const(eXMLError, "XML_RNGP_FORBIDDEN_ATTRIBUTE", INT2NUM(1034));
  rb_define_const(eXMLError, "XML_RNGP_FOREIGN_ELEMENT", INT2NUM(1035));
  rb_define_const(eXMLError, "XML_RNGP_GRAMMAR_CONTENT", INT2NUM(1036));
  rb_define_const(eXMLError, "XML_RNGP_GRAMMAR_EMPTY", INT2NUM(1037));
  rb_define_const(eXMLError, "XML_RNGP_GRAMMAR_MISSING", INT2NUM(1038));
  rb_define_const(eXMLError, "XML_RNGP_GRAMMAR_NO_START", INT2NUM(1039));
  rb_define_const(eXMLError, "XML_RNGP_GROUP_ATTR_CONFLICT", INT2NUM(1040));
  rb_define_const(eXMLError, "XML_RNGP_HREF_ERROR", INT2NUM(1041));
  rb_define_const(eXMLError, "XML_RNGP_INCLUDE_EMPTY", INT2NUM(1042));
  rb_define_const(eXMLError, "XML_RNGP_INCLUDE_FAILURE", INT2NUM(1043));
  rb_define_const(eXMLError, "XML_RNGP_INCLUDE_RECURSE", INT2NUM(1044));
  rb_define_const(eXMLError, "XML_RNGP_INTERLEAVE_ADD", INT2NUM(1045));
  rb_define_const(eXMLError, "XML_RNGP_INTERLEAVE_CREATE_FAILED", INT2NUM(1046));
  rb_define_const(eXMLError, "XML_RNGP_INTERLEAVE_EMPTY", INT2NUM(1047));
  rb_define_const(eXMLError, "XML_RNGP_INTERLEAVE_NO_CONTENT", INT2NUM(1048));
  rb_define_const(eXMLError, "XML_RNGP_INVALID_DEFINE_NAME", INT2NUM(1049));
  rb_define_const(eXMLError, "XML_RNGP_INVALID_URI", INT2NUM(1050));
  rb_define_const(eXMLError, "XML_RNGP_INVALID_VALUE", INT2NUM(1051));
  rb_define_const(eXMLError, "XML_RNGP_MISSING_HREF", INT2NUM(1052));
  rb_define_const(eXMLError, "XML_RNGP_NAME_MISSING", INT2NUM(1053));
  rb_define_const(eXMLError, "XML_RNGP_NEED_COMBINE", INT2NUM(1054));
  rb_define_const(eXMLError, "XML_RNGP_NOTALLOWED_NOT_EMPTY", INT2NUM(1055));
  rb_define_const(eXMLError, "XML_RNGP_NSNAME_ATTR_ANCESTOR", INT2NUM(1056));
  rb_define_const(eXMLError, "XML_RNGP_NSNAME_NO_NS", INT2NUM(1057));
  rb_define_const(eXMLError, "XML_RNGP_PARAM_FORBIDDEN", INT2NUM(1058));
  rb_define_const(eXMLError, "XML_RNGP_PARAM_NAME_MISSING", INT2NUM(1059));
  rb_define_const(eXMLError, "XML_RNGP_PARENTREF_CREATE_FAILED", INT2NUM(1060));
  rb_define_const(eXMLError, "XML_RNGP_PARENTREF_NAME_INVALID", INT2NUM(1061));
  rb_define_const(eXMLError, "XML_RNGP_PARENTREF_NO_NAME", INT2NUM(1062));
  rb_define_const(eXMLError, "XML_RNGP_PARENTREF_NO_PARENT", INT2NUM(1063));
  rb_define_const(eXMLError, "XML_RNGP_PARENTREF_NOT_EMPTY", INT2NUM(1064));
  rb_define_const(eXMLError, "XML_RNGP_PARSE_ERROR", INT2NUM(1065));
  rb_define_const(eXMLError, "XML_RNGP_PAT_ANYNAME_EXCEPT_ANYNAME", INT2NUM(1066));
  rb_define_const(eXMLError, "XML_RNGP_PAT_ATTR_ATTR", INT2NUM(1067));
  rb_define_const(eXMLError, "XML_RNGP_PAT_ATTR_ELEM", INT2NUM(1068));
  rb_define_const(eXMLError, "XML_RNGP_PAT_DATA_EXCEPT_ATTR", INT2NUM(1069));
  rb_define_const(eXMLError, "XML_RNGP_PAT_DATA_EXCEPT_ELEM", INT2NUM(1070));
  rb_define_const(eXMLError, "XML_RNGP_PAT_DATA_EXCEPT_EMPTY", INT2NUM(1071));
  rb_define_const(eXMLError, "XML_RNGP_PAT_DATA_EXCEPT_GROUP", INT2NUM(1072));
  rb_define_const(eXMLError, "XML_RNGP_PAT_DATA_EXCEPT_INTERLEAVE", INT2NUM(1073));
  rb_define_const(eXMLError, "XML_RNGP_PAT_DATA_EXCEPT_LIST", INT2NUM(1074));
  rb_define_const(eXMLError, "XML_RNGP_PAT_DATA_EXCEPT_ONEMORE", INT2NUM(1075));
  rb_define_const(eXMLError, "XML_RNGP_PAT_DATA_EXCEPT_REF", INT2NUM(1076));
  rb_define_const(eXMLError, "XML_RNGP_PAT_DATA_EXCEPT_TEXT", INT2NUM(1077));
  rb_define_const(eXMLError, "XML_RNGP_PAT_LIST_ATTR", INT2NUM(1078));
  rb_define_const(eXMLError, "XML_RNGP_PAT_LIST_ELEM", INT2NUM(1079));
  rb_define_const(eXMLError, "XML_RNGP_PAT_LIST_INTERLEAVE", INT2NUM(1080));
  rb_define_const(eXMLError, "XML_RNGP_PAT_LIST_LIST", INT2NUM(1081));
  rb_define_const(eXMLError, "XML_RNGP_PAT_LIST_REF", INT2NUM(1082));
  rb_define_const(eXMLError, "XML_RNGP_PAT_LIST_TEXT", INT2NUM(1083));
  rb_define_const(eXMLError, "XML_RNGP_PAT_NSNAME_EXCEPT_ANYNAME", INT2NUM(1084));
  rb_define_const(eXMLError, "XML_RNGP_PAT_NSNAME_EXCEPT_NSNAME", INT2NUM(1085));
  rb_define_const(eXMLError, "XML_RNGP_PAT_ONEMORE_GROUP_ATTR", INT2NUM(1086));
  rb_define_const(eXMLError, "XML_RNGP_PAT_ONEMORE_INTERLEAVE_ATTR", INT2NUM(1087));
  rb_define_const(eXMLError, "XML_RNGP_PAT_START_ATTR", INT2NUM(1088));
  rb_define_const(eXMLError, "XML_RNGP_PAT_START_DATA", INT2NUM(1089));
  rb_define_const(eXMLError, "XML_RNGP_PAT_START_EMPTY", INT2NUM(1090));
  rb_define_const(eXMLError, "XML_RNGP_PAT_START_GROUP", INT2NUM(1091));
  rb_define_const(eXMLError, "XML_RNGP_PAT_START_INTERLEAVE", INT2NUM(1092));
  rb_define_const(eXMLError, "XML_RNGP_PAT_START_LIST", INT2NUM(1093));
  rb_define_const(eXMLError, "XML_RNGP_PAT_START_ONEMORE", INT2NUM(1094));
  rb_define_const(eXMLError, "XML_RNGP_PAT_START_TEXT", INT2NUM(1095));
  rb_define_const(eXMLError, "XML_RNGP_PAT_START_VALUE", INT2NUM(1096));
  rb_define_const(eXMLError, "XML_RNGP_PREFIX_UNDEFINED", INT2NUM(1097));
  rb_define_const(eXMLError, "XML_RNGP_REF_CREATE_FAILED", INT2NUM(1098));
  rb_define_const(eXMLError, "XML_RNGP_REF_CYCLE", INT2NUM(1099));
  rb_define_const(eXMLError, "XML_RNGP_REF_NAME_INVALID", INT2NUM(1100));
  rb_define_const(eXMLError, "XML_RNGP_REF_NO_DEF", INT2NUM(1101));
  rb_define_const(eXMLError, "XML_RNGP_REF_NO_NAME", INT2NUM(1102));
  rb_define_const(eXMLError, "XML_RNGP_REF_NOT_EMPTY", INT2NUM(1103));
  rb_define_const(eXMLError, "XML_RNGP_START_CHOICE_AND_INTERLEAVE", INT2NUM(1104));
  rb_define_const(eXMLError, "XML_RNGP_START_CONTENT", INT2NUM(1105));
  rb_define_const(eXMLError, "XML_RNGP_START_EMPTY", INT2NUM(1106));
  rb_define_const(eXMLError, "XML_RNGP_START_MISSING", INT2NUM(1107));
  rb_define_const(eXMLError, "XML_RNGP_TEXT_EXPECTED", INT2NUM(1108));
  rb_define_const(eXMLError, "XML_RNGP_TEXT_HAS_CHILD", INT2NUM(1109));
  rb_define_const(eXMLError, "XML_RNGP_TYPE_MISSING", INT2NUM(1110));
  rb_define_const(eXMLError, "XML_RNGP_TYPE_NOT_FOUND", INT2NUM(1111));
  rb_define_const(eXMLError, "XML_RNGP_TYPE_VALUE", INT2NUM(1112));
  rb_define_const(eXMLError, "XML_RNGP_UNKNOWN_ATTRIBUTE", INT2NUM(1113));
  rb_define_const(eXMLError, "XML_RNGP_UNKNOWN_COMBINE", INT2NUM(1114));
  rb_define_const(eXMLError, "XML_RNGP_UNKNOWN_CONSTRUCT", INT2NUM(1115));
  rb_define_const(eXMLError, "XML_RNGP_UNKNOWN_TYPE_LIB", INT2NUM(1116));
  rb_define_const(eXMLError, "XML_RNGP_URI_FRAGMENT", INT2NUM(1117));
  rb_define_const(eXMLError, "XML_RNGP_URI_NOT_ABSOLUTE", INT2NUM(1118));
  rb_define_const(eXMLError, "XML_RNGP_VALUE_EMPTY", INT2NUM(1119));
  rb_define_const(eXMLError, "XML_RNGP_VALUE_NO_CONTENT", INT2NUM(1120));
  rb_define_const(eXMLError, "XML_RNGP_XMLNS_NAME", INT2NUM(1121));
  rb_define_const(eXMLError, "XML_RNGP_XML_NS", INT2NUM(1122));
  rb_define_const(eXMLError, "XML_XPATH_EXPRESSION_OK", INT2NUM(1200));
  rb_define_const(eXMLError, "XML_XPATH_NUMBER_ERROR", INT2NUM(1201));
  rb_define_const(eXMLError, "XML_XPATH_UNFINISHED_LITERAL_ERROR", INT2NUM(1202));
  rb_define_const(eXMLError, "XML_XPATH_START_LITERAL_ERROR", INT2NUM(1203));
  rb_define_const(eXMLError, "XML_XPATH_VARIABLE_REF_ERROR", INT2NUM(1204));
  rb_define_const(eXMLError, "XML_XPATH_UNDEF_VARIABLE_ERROR", INT2NUM(1205));
  rb_define_const(eXMLError, "XML_XPATH_INVALID_PREDICATE_ERROR", INT2NUM(1206));
  rb_define_const(eXMLError, "XML_XPATH_EXPR_ERROR", INT2NUM(1207));
  rb_define_const(eXMLError, "XML_XPATH_UNCLOSED_ERROR", INT2NUM(1208));
  rb_define_const(eXMLError, "XML_XPATH_UNKNOWN_FUNC_ERROR", INT2NUM(1209));
  rb_define_const(eXMLError, "XML_XPATH_INVALID_OPERAND", INT2NUM(1210));
  rb_define_const(eXMLError, "XML_XPATH_INVALID_TYPE", INT2NUM(1211));
  rb_define_const(eXMLError, "XML_XPATH_INVALID_ARITY", INT2NUM(1212));
  rb_define_const(eXMLError, "XML_XPATH_INVALID_CTXT_SIZE", INT2NUM(1213));
  rb_define_const(eXMLError, "XML_XPATH_INVALID_CTXT_POSITION", INT2NUM(1214));
  rb_define_const(eXMLError, "XML_XPATH_MEMORY_ERROR", INT2NUM(1215));
  rb_define_const(eXMLError, "XML_XPTR_SYNTAX_ERROR", INT2NUM(1216));
  rb_define_const(eXMLError, "XML_XPTR_RESOURCE_ERROR", INT2NUM(1217));
  rb_define_const(eXMLError, "XML_XPTR_SUB_RESOURCE_ERROR", INT2NUM(1218));
  rb_define_const(eXMLError, "XML_XPATH_UNDEF_PREFIX_ERROR", INT2NUM(1219));
  rb_define_const(eXMLError, "XML_XPATH_ENCODING_ERROR", INT2NUM(1220));
  rb_define_const(eXMLError, "XML_XPATH_INVALID_CHAR_ERROR", INT2NUM(1221));
  rb_define_const(eXMLError, "XML_TREE_INVALID_HEX", INT2NUM(1300));
  rb_define_const(eXMLError, "XML_TREE_INVALID_DEC", INT2NUM(1301));
  rb_define_const(eXMLError, "XML_TREE_UNTERMINATED_ENTITY", INT2NUM(1302));
  rb_define_const(eXMLError, "XML_TREE_NOT_UTF8", INT2NUM(1303));
  rb_define_const(eXMLError, "XML_SAVE_NOT_UTF8", INT2NUM(1400));
  rb_define_const(eXMLError, "XML_SAVE_CHAR_INVALID", INT2NUM(1401));
  rb_define_const(eXMLError, "XML_SAVE_NO_DOCTYPE", INT2NUM(1402));
  rb_define_const(eXMLError, "XML_SAVE_UNKNOWN_ENCODING", INT2NUM(1403));
  rb_define_const(eXMLError, "XML_REGEXP_COMPILE_ERROR", INT2NUM(1450));
  rb_define_const(eXMLError, "XML_IO_UNKNOWN", INT2NUM(1500));
  rb_define_const(eXMLError, "XML_IO_EACCES", INT2NUM(1501));
  rb_define_const(eXMLError, "XML_IO_EAGAIN", INT2NUM(1502));
  rb_define_const(eXMLError, "XML_IO_EBADF", INT2NUM(1503));
  rb_define_const(eXMLError, "XML_IO_EBADMSG", INT2NUM(1504));
  rb_define_const(eXMLError, "XML_IO_EBUSY", INT2NUM(1505));
  rb_define_const(eXMLError, "XML_IO_ECANCELED", INT2NUM(1506));
  rb_define_const(eXMLError, "XML_IO_ECHILD", INT2NUM(1507));
  rb_define_const(eXMLError, "XML_IO_EDEADLK", INT2NUM(1508));
  rb_define_const(eXMLError, "XML_IO_EDOM", INT2NUM(1509));
  rb_define_const(eXMLError, "XML_IO_EEXIST", INT2NUM(1510));
  rb_define_const(eXMLError, "XML_IO_EFAULT", INT2NUM(1511));
  rb_define_const(eXMLError, "XML_IO_EFBIG", INT2NUM(1512));
  rb_define_const(eXMLError, "XML_IO_EINPROGRESS", INT2NUM(1513));
  rb_define_const(eXMLError, "XML_IO_EINTR", INT2NUM(1514));
  rb_define_const(eXMLError, "XML_IO_EINVAL", INT2NUM(1515));
  rb_define_const(eXMLError, "XML_IO_EIO", INT2NUM(1516));
  rb_define_const(eXMLError, "XML_IO_EISDIR", INT2NUM(1517));
  rb_define_const(eXMLError, "XML_IO_EMFILE", INT2NUM(1518));
  rb_define_const(eXMLError, "XML_IO_EMLINK", INT2NUM(1519));
  rb_define_const(eXMLError, "XML_IO_EMSGSIZE", INT2NUM(1520));
  rb_define_const(eXMLError, "XML_IO_ENAMETOOLONG", INT2NUM(1521));
  rb_define_const(eXMLError, "XML_IO_ENFILE", INT2NUM(1522));
  rb_define_const(eXMLError, "XML_IO_ENODEV", INT2NUM(1523));
  rb_define_const(eXMLError, "XML_IO_ENOENT", INT2NUM(1524));
  rb_define_const(eXMLError, "XML_IO_ENOEXEC", INT2NUM(1525));
  rb_define_const(eXMLError, "XML_IO_ENOLCK", INT2NUM(1526));
  rb_define_const(eXMLError, "XML_IO_ENOMEM", INT2NUM(1527));
  rb_define_const(eXMLError, "XML_IO_ENOSPC", INT2NUM(1528));
  rb_define_const(eXMLError, "XML_IO_ENOSYS", INT2NUM(1529));
  rb_define_const(eXMLError, "XML_IO_ENOTDIR", INT2NUM(1530));
  rb_define_const(eXMLError, "XML_IO_ENOTEMPTY", INT2NUM(1531));
  rb_define_const(eXMLError, "XML_IO_ENOTSUP", INT2NUM(1532));
  rb_define_const(eXMLError, "XML_IO_ENOTTY", INT2NUM(1533));
  rb_define_const(eXMLError, "XML_IO_ENXIO", INT2NUM(1534));
  rb_define_const(eXMLError, "XML_IO_EPERM", INT2NUM(1535));
  rb_define_const(eXMLError, "XML_IO_EPIPE", INT2NUM(1536));
  rb_define_const(eXMLError, "XML_IO_ERANGE", INT2NUM(1537));
  rb_define_const(eXMLError, "XML_IO_EROFS", INT2NUM(1538));
  rb_define_const(eXMLError, "XML_IO_ESPIPE", INT2NUM(1539));
  rb_define_const(eXMLError, "XML_IO_ESRCH", INT2NUM(1540));
  rb_define_const(eXMLError, "XML_IO_ETIMEDOUT", INT2NUM(1541));
  rb_define_const(eXMLError, "XML_IO_EXDEV", INT2NUM(1542));
  rb_define_const(eXMLError, "XML_IO_NETWORK_ATTEMPT", INT2NUM(1543));
  rb_define_const(eXMLError, "XML_IO_ENCODER", INT2NUM(1544));
  rb_define_const(eXMLError, "XML_IO_FLUSH", INT2NUM(1545));
  rb_define_const(eXMLError, "XML_IO_WRITE", INT2NUM(1546));
  rb_define_const(eXMLError, "XML_IO_NO_INPUT", INT2NUM(1547));
  rb_define_const(eXMLError, "XML_IO_BUFFER_FULL", INT2NUM(1548));
  rb_define_const(eXMLError, "XML_IO_LOAD_ERROR", INT2NUM(1549));
  rb_define_const(eXMLError, "XML_IO_ENOTSOCK", INT2NUM(1550));
  rb_define_const(eXMLError, "XML_IO_EISCONN", INT2NUM(1551));
  rb_define_const(eXMLError, "XML_IO_ECONNREFUSED", INT2NUM(1552));
  rb_define_const(eXMLError, "XML_IO_ENETUNREACH", INT2NUM(1553));
  rb_define_const(eXMLError, "XML_IO_EADDRINUSE", INT2NUM(1554));
  rb_define_const(eXMLError, "XML_IO_EALREADY", INT2NUM(1555));
  rb_define_const(eXMLError, "XML_IO_EAFNOSUPPORT", INT2NUM(1556));
  rb_define_const(eXMLError, "XML_XINCLUDE_RECURSION", INT2NUM(1600));
  rb_define_const(eXMLError, "XML_XINCLUDE_PARSE_VALUE", INT2NUM(1601));
  rb_define_const(eXMLError, "XML_XINCLUDE_ENTITY_DEF_MISMATCH", INT2NUM(1602));
  rb_define_const(eXMLError, "XML_XINCLUDE_NO_HREF", INT2NUM(1603));
  rb_define_const(eXMLError, "XML_XINCLUDE_NO_FALLBACK", INT2NUM(1604));
  rb_define_const(eXMLError, "XML_XINCLUDE_HREF_URI", INT2NUM(1605));
  rb_define_const(eXMLError, "XML_XINCLUDE_TEXT_FRAGMENT", INT2NUM(1606));
  rb_define_const(eXMLError, "XML_XINCLUDE_TEXT_DOCUMENT", INT2NUM(1607));
  rb_define_const(eXMLError, "XML_XINCLUDE_INVALID_CHAR", INT2NUM(1608));
  rb_define_const(eXMLError, "XML_XINCLUDE_BUILD_FAILED", INT2NUM(1609));
  rb_define_const(eXMLError, "XML_XINCLUDE_UNKNOWN_ENCODING", INT2NUM(1610));
  rb_define_const(eXMLError, "XML_XINCLUDE_MULTIPLE_ROOT", INT2NUM(1611));
  rb_define_const(eXMLError, "XML_XINCLUDE_XPTR_FAILED", INT2NUM(1612));
  rb_define_const(eXMLError, "XML_XINCLUDE_XPTR_RESULT", INT2NUM(1613));
  rb_define_const(eXMLError, "XML_XINCLUDE_INCLUDE_IN_INCLUDE", INT2NUM(1614));
  rb_define_const(eXMLError, "XML_XINCLUDE_FALLBACKS_IN_INCLUDE", INT2NUM(1615));
  rb_define_const(eXMLError, "XML_XINCLUDE_FALLBACK_NOT_IN_INCLUDE", INT2NUM(1616));
  rb_define_const(eXMLError, "XML_XINCLUDE_DEPRECATED_NS", INT2NUM(1617));
  rb_define_const(eXMLError, "XML_XINCLUDE_FRAGMENT_ID", INT2NUM(1618));
  rb_define_const(eXMLError, "XML_CATALOG_MISSING_ATTR", INT2NUM(1650));
  rb_define_const(eXMLError, "XML_CATALOG_ENTRY_BROKEN", INT2NUM(1651));
  rb_define_const(eXMLError, "XML_CATALOG_PREFER_VALUE", INT2NUM(1652));
  rb_define_const(eXMLError, "XML_CATALOG_NOT_CATALOG", INT2NUM(1653));
  rb_define_const(eXMLError, "XML_CATALOG_RECURSION", INT2NUM(1654));
  rb_define_const(eXMLError, "XML_SCHEMAP_PREFIX_UNDEFINED", INT2NUM(1700));
  rb_define_const(eXMLError, "XML_SCHEMAP_ATTRFORMDEFAULT_VALUE", INT2NUM(1701));
  rb_define_const(eXMLError, "XML_SCHEMAP_ATTRGRP_NONAME_NOREF", INT2NUM(1702));
  rb_define_const(eXMLError, "XML_SCHEMAP_ATTR_NONAME_NOREF", INT2NUM(1703));
  rb_define_const(eXMLError, "XML_SCHEMAP_COMPLEXTYPE_NONAME_NOREF", INT2NUM(1704));
  rb_define_const(eXMLError, "XML_SCHEMAP_ELEMFORMDEFAULT_VALUE", INT2NUM(1705));
  rb_define_const(eXMLError, "XML_SCHEMAP_ELEM_NONAME_NOREF", INT2NUM(1706));
  rb_define_const(eXMLError, "XML_SCHEMAP_EXTENSION_NO_BASE", INT2NUM(1707));
  rb_define_const(eXMLError, "XML_SCHEMAP_FACET_NO_VALUE", INT2NUM(1708));
  rb_define_const(eXMLError, "XML_SCHEMAP_FAILED_BUILD_IMPORT", INT2NUM(1709));
  rb_define_const(eXMLError, "XML_SCHEMAP_GROUP_NONAME_NOREF", INT2NUM(1710));
  rb_define_const(eXMLError, "XML_SCHEMAP_IMPORT_NAMESPACE_NOT_URI", INT2NUM(1711));
  rb_define_const(eXMLError, "XML_SCHEMAP_IMPORT_REDEFINE_NSNAME", INT2NUM(1712));
  rb_define_const(eXMLError, "XML_SCHEMAP_IMPORT_SCHEMA_NOT_URI", INT2NUM(1713));
  rb_define_const(eXMLError, "XML_SCHEMAP_INVALID_BOOLEAN", INT2NUM(1714));
  rb_define_const(eXMLError, "XML_SCHEMAP_INVALID_ENUM", INT2NUM(1715));
  rb_define_const(eXMLError, "XML_SCHEMAP_INVALID_FACET", INT2NUM(1716));
  rb_define_const(eXMLError, "XML_SCHEMAP_INVALID_FACET_VALUE", INT2NUM(1717));
  rb_define_const(eXMLError, "XML_SCHEMAP_INVALID_MAXOCCURS", INT2NUM(1718));
  rb_define_const(eXMLError, "XML_SCHEMAP_INVALID_MINOCCURS", INT2NUM(1719));
  rb_define_const(eXMLError, "XML_SCHEMAP_INVALID_REF_AND_SUBTYPE", INT2NUM(1720));
  rb_define_const(eXMLError, "XML_SCHEMAP_INVALID_WHITE_SPACE", INT2NUM(1721));
  rb_define_const(eXMLError, "XML_SCHEMAP_NOATTR_NOREF", INT2NUM(1722));
  rb_define_const(eXMLError, "XML_SCHEMAP_NOTATION_NO_NAME", INT2NUM(1723));
  rb_define_const(eXMLError, "XML_SCHEMAP_NOTYPE_NOREF", INT2NUM(1724));
  rb_define_const(eXMLError, "XML_SCHEMAP_REF_AND_SUBTYPE", INT2NUM(1725));
  rb_define_const(eXMLError, "XML_SCHEMAP_RESTRICTION_NONAME_NOREF", INT2NUM(1726));
  rb_define_const(eXMLError, "XML_SCHEMAP_SIMPLETYPE_NONAME", INT2NUM(1727));
  rb_define_const(eXMLError, "XML_SCHEMAP_TYPE_AND_SUBTYPE", INT2NUM(1728));
  rb_define_const(eXMLError, "XML_SCHEMAP_UNKNOWN_ALL_CHILD", INT2NUM(1729));
  rb_define_const(eXMLError, "XML_SCHEMAP_UNKNOWN_ANYATTRIBUTE_CHILD", INT2NUM(1730));
  rb_define_const(eXMLError, "XML_SCHEMAP_UNKNOWN_ATTR_CHILD", INT2NUM(1731));
  rb_define_const(eXMLError, "XML_SCHEMAP_UNKNOWN_ATTRGRP_CHILD", INT2NUM(1732));
  rb_define_const(eXMLError, "XML_SCHEMAP_UNKNOWN_ATTRIBUTE_GROUP", INT2NUM(1733));
  rb_define_const(eXMLError, "XML_SCHEMAP_UNKNOWN_BASE_TYPE", INT2NUM(1734));
  rb_define_const(eXMLError, "XML_SCHEMAP_UNKNOWN_CHOICE_CHILD", INT2NUM(1735));
  rb_define_const(eXMLError, "XML_SCHEMAP_UNKNOWN_COMPLEXCONTENT_CHILD", INT2NUM(1736));
  rb_define_const(eXMLError, "XML_SCHEMAP_UNKNOWN_COMPLEXTYPE_CHILD", INT2NUM(1737));
  rb_define_const(eXMLError, "XML_SCHEMAP_UNKNOWN_ELEM_CHILD", INT2NUM(1738));
  rb_define_const(eXMLError, "XML_SCHEMAP_UNKNOWN_EXTENSION_CHILD", INT2NUM(1739));
  rb_define_const(eXMLError, "XML_SCHEMAP_UNKNOWN_FACET_CHILD", INT2NUM(1740));
  rb_define_const(eXMLError, "XML_SCHEMAP_UNKNOWN_FACET_TYPE", INT2NUM(1741));
  rb_define_const(eXMLError, "XML_SCHEMAP_UNKNOWN_GROUP_CHILD", INT2NUM(1742));
  rb_define_const(eXMLError, "XML_SCHEMAP_UNKNOWN_IMPORT_CHILD", INT2NUM(1743));
  rb_define_const(eXMLError, "XML_SCHEMAP_UNKNOWN_LIST_CHILD", INT2NUM(1744));
  rb_define_const(eXMLError, "XML_SCHEMAP_UNKNOWN_NOTATION_CHILD", INT2NUM(1745));
  rb_define_const(eXMLError, "XML_SCHEMAP_UNKNOWN_PROCESSCONTENT_CHILD", INT2NUM(1746));
  rb_define_const(eXMLError, "XML_SCHEMAP_UNKNOWN_REF", INT2NUM(1747));
  rb_define_const(eXMLError, "XML_SCHEMAP_UNKNOWN_RESTRICTION_CHILD", INT2NUM(1748));
  rb_define_const(eXMLError, "XML_SCHEMAP_UNKNOWN_SCHEMAS_CHILD", INT2NUM(1749));
  rb_define_const(eXMLError, "XML_SCHEMAP_UNKNOWN_SEQUENCE_CHILD", INT2NUM(1750));
  rb_define_const(eXMLError, "XML_SCHEMAP_UNKNOWN_SIMPLECONTENT_CHILD", INT2NUM(1751));
  rb_define_const(eXMLError, "XML_SCHEMAP_UNKNOWN_SIMPLETYPE_CHILD", INT2NUM(1752));
  rb_define_const(eXMLError, "XML_SCHEMAP_UNKNOWN_TYPE", INT2NUM(1753));
  rb_define_const(eXMLError, "XML_SCHEMAP_UNKNOWN_UNION_CHILD", INT2NUM(1754));
  rb_define_const(eXMLError, "XML_SCHEMAP_ELEM_DEFAULT_FIXED", INT2NUM(1755));
  rb_define_const(eXMLError, "XML_SCHEMAP_REGEXP_INVALID", INT2NUM(1756));
  rb_define_const(eXMLError, "XML_SCHEMAP_FAILED_LOAD", INT2NUM(1757));
  rb_define_const(eXMLError, "XML_SCHEMAP_NOTHING_TO_PARSE", INT2NUM(1758));
  rb_define_const(eXMLError, "XML_SCHEMAP_NOROOT", INT2NUM(1759));
  rb_define_const(eXMLError, "XML_SCHEMAP_REDEFINED_GROUP", INT2NUM(1760));
  rb_define_const(eXMLError, "XML_SCHEMAP_REDEFINED_TYPE", INT2NUM(1761));
  rb_define_const(eXMLError, "XML_SCHEMAP_REDEFINED_ELEMENT", INT2NUM(1762));
  rb_define_const(eXMLError, "XML_SCHEMAP_REDEFINED_ATTRGROUP", INT2NUM(1763));
  rb_define_const(eXMLError, "XML_SCHEMAP_REDEFINED_ATTR", INT2NUM(1764));
  rb_define_const(eXMLError, "XML_SCHEMAP_REDEFINED_NOTATION", INT2NUM(1765));
  rb_define_const(eXMLError, "XML_SCHEMAP_FAILED_PARSE", INT2NUM(1766));
  rb_define_const(eXMLError, "XML_SCHEMAP_UNKNOWN_PREFIX", INT2NUM(1767));
  rb_define_const(eXMLError, "XML_SCHEMAP_DEF_AND_PREFIX", INT2NUM(1768));
  rb_define_const(eXMLError, "XML_SCHEMAP_UNKNOWN_INCLUDE_CHILD", INT2NUM(1769));
  rb_define_const(eXMLError, "XML_SCHEMAP_INCLUDE_SCHEMA_NOT_URI", INT2NUM(1770));
  rb_define_const(eXMLError, "XML_SCHEMAP_INCLUDE_SCHEMA_NO_URI", INT2NUM(1771));
  rb_define_const(eXMLError, "XML_SCHEMAP_NOT_SCHEMA", INT2NUM(1772));
  rb_define_const(eXMLError, "XML_SCHEMAP_UNKNOWN_MEMBER_TYPE", INT2NUM(1773));
  rb_define_const(eXMLError, "XML_SCHEMAP_INVALID_ATTR_USE", INT2NUM(1774));
  rb_define_const(eXMLError, "XML_SCHEMAP_RECURSIVE", INT2NUM(1775));
  rb_define_const(eXMLError, "XML_SCHEMAP_SUPERNUMEROUS_LIST_ITEM_TYPE", INT2NUM(1776));
  rb_define_const(eXMLError, "XML_SCHEMAP_INVALID_ATTR_COMBINATION", INT2NUM(1777));
  rb_define_const(eXMLError, "XML_SCHEMAP_INVALID_ATTR_INLINE_COMBINATION", INT2NUM(1778));
  rb_define_const(eXMLError, "XML_SCHEMAP_MISSING_SIMPLETYPE_CHILD", INT2NUM(1779));
  rb_define_const(eXMLError, "XML_SCHEMAP_INVALID_ATTR_NAME", INT2NUM(1780));
  rb_define_const(eXMLError, "XML_SCHEMAP_REF_AND_CONTENT", INT2NUM(1781));
  rb_define_const(eXMLError, "XML_SCHEMAP_CT_PROPS_CORRECT_1", INT2NUM(1782));
  rb_define_const(eXMLError, "XML_SCHEMAP_CT_PROPS_CORRECT_2", INT2NUM(1783));
  rb_define_const(eXMLError, "XML_SCHEMAP_CT_PROPS_CORRECT_3", INT2NUM(1784));
  rb_define_const(eXMLError, "XML_SCHEMAP_CT_PROPS_CORRECT_4", INT2NUM(1785));
  rb_define_const(eXMLError, "XML_SCHEMAP_CT_PROPS_CORRECT_5", INT2NUM(1786));
  rb_define_const(eXMLError, "XML_SCHEMAP_DERIVATION_OK_RESTRICTION_1", INT2NUM(1787));
  rb_define_const(eXMLError, "XML_SCHEMAP_DERIVATION_OK_RESTRICTION_2_1_1", INT2NUM(1788));
  rb_define_const(eXMLError, "XML_SCHEMAP_DERIVATION_OK_RESTRICTION_2_1_2", INT2NUM(1789));
  rb_define_const(eXMLError, "XML_SCHEMAP_DERIVATION_OK_RESTRICTION_2_2", INT2NUM(1790));
  rb_define_const(eXMLError, "XML_SCHEMAP_DERIVATION_OK_RESTRICTION_3", INT2NUM(1791));
  rb_define_const(eXMLError, "XML_SCHEMAP_WILDCARD_INVALID_NS_MEMBER", INT2NUM(1792));
  rb_define_const(eXMLError, "XML_SCHEMAP_INTERSECTION_NOT_EXPRESSIBLE", INT2NUM(1793));
  rb_define_const(eXMLError, "XML_SCHEMAP_UNION_NOT_EXPRESSIBLE", INT2NUM(1794));
  rb_define_const(eXMLError, "XML_SCHEMAP_SRC_IMPORT_3_1", INT2NUM(1795));
  rb_define_const(eXMLError, "XML_SCHEMAP_SRC_IMPORT_3_2", INT2NUM(1796));
  rb_define_const(eXMLError, "XML_SCHEMAP_DERIVATION_OK_RESTRICTION_4_1", INT2NUM(1797));
  rb_define_const(eXMLError, "XML_SCHEMAP_DERIVATION_OK_RESTRICTION_4_2", INT2NUM(1798));
  rb_define_const(eXMLError, "XML_SCHEMAP_DERIVATION_OK_RESTRICTION_4_3", INT2NUM(1799));
  rb_define_const(eXMLError, "XML_SCHEMAP_COS_CT_EXTENDS_1_3", INT2NUM(1800));
  rb_define_const(eXMLError, "XML_SCHEMAV_NOROOT", INT2NUM(1801));
  rb_define_const(eXMLError, "XML_SCHEMAV_UNDECLAREDELEM", INT2NUM(1802));
  rb_define_const(eXMLError, "XML_SCHEMAV_NOTTOPLEVEL", INT2NUM(1803));
  rb_define_const(eXMLError, "XML_SCHEMAV_MISSING", INT2NUM(1804));
  rb_define_const(eXMLError, "XML_SCHEMAV_WRONGELEM", INT2NUM(1805));
  rb_define_const(eXMLError, "XML_SCHEMAV_NOTYPE", INT2NUM(1806));
  rb_define_const(eXMLError, "XML_SCHEMAV_NOROLLBACK", INT2NUM(1807));
  rb_define_const(eXMLError, "XML_SCHEMAV_ISABSTRACT", INT2NUM(1808));
  rb_define_const(eXMLError, "XML_SCHEMAV_NOTEMPTY", INT2NUM(1809));
  rb_define_const(eXMLError, "XML_SCHEMAV_ELEMCONT", INT2NUM(1810));
  rb_define_const(eXMLError, "XML_SCHEMAV_HAVEDEFAULT", INT2NUM(1811));
  rb_define_const(eXMLError, "XML_SCHEMAV_NOTNILLABLE", INT2NUM(1812));
  rb_define_const(eXMLError, "XML_SCHEMAV_EXTRACONTENT", INT2NUM(1813));
  rb_define_const(eXMLError, "XML_SCHEMAV_INVALIDATTR", INT2NUM(1814));
  rb_define_const(eXMLError, "XML_SCHEMAV_INVALIDELEM", INT2NUM(1815));
  rb_define_const(eXMLError, "XML_SCHEMAV_NOTDETERMINIST", INT2NUM(1816));
  rb_define_const(eXMLError, "XML_SCHEMAV_CONSTRUCT", INT2NUM(1817));
  rb_define_const(eXMLError, "XML_SCHEMAV_INTERNAL", INT2NUM(1818));
  rb_define_const(eXMLError, "XML_SCHEMAV_NOTSIMPLE", INT2NUM(1819));
  rb_define_const(eXMLError, "XML_SCHEMAV_ATTRUNKNOWN", INT2NUM(1820));
  rb_define_const(eXMLError, "XML_SCHEMAV_ATTRINVALID", INT2NUM(1821));
  rb_define_const(eXMLError, "XML_SCHEMAV_VALUE", INT2NUM(1822));
  rb_define_const(eXMLError, "XML_SCHEMAV_FACET", INT2NUM(1823));
  rb_define_const(eXMLError, "XML_SCHEMAV_CVC_DATATYPE_VALID_1_2_1", INT2NUM(1824));
  rb_define_const(eXMLError, "XML_SCHEMAV_CVC_DATATYPE_VALID_1_2_2", INT2NUM(1825));
  rb_define_const(eXMLError, "XML_SCHEMAV_CVC_DATATYPE_VALID_1_2_3", INT2NUM(1826));
  rb_define_const(eXMLError, "XML_SCHEMAV_CVC_TYPE_3_1_1", INT2NUM(1827));
  rb_define_const(eXMLError, "XML_SCHEMAV_CVC_TYPE_3_1_2", INT2NUM(1828));
  rb_define_const(eXMLError, "XML_SCHEMAV_CVC_FACET_VALID", INT2NUM(1829));
  rb_define_const(eXMLError, "XML_SCHEMAV_CVC_LENGTH_VALID", INT2NUM(1830));
  rb_define_const(eXMLError, "XML_SCHEMAV_CVC_MINLENGTH_VALID", INT2NUM(1831));
  rb_define_const(eXMLError, "XML_SCHEMAV_CVC_MAXLENGTH_VALID", INT2NUM(1832));
  rb_define_const(eXMLError, "XML_SCHEMAV_CVC_MININCLUSIVE_VALID", INT2NUM(1833));
  rb_define_const(eXMLError, "XML_SCHEMAV_CVC_MAXINCLUSIVE_VALID", INT2NUM(1834));
  rb_define_const(eXMLError, "XML_SCHEMAV_CVC_MINEXCLUSIVE_VALID", INT2NUM(1835));
  rb_define_const(eXMLError, "XML_SCHEMAV_CVC_MAXEXCLUSIVE_VALID", INT2NUM(1836));
  rb_define_const(eXMLError, "XML_SCHEMAV_CVC_TOTALDIGITS_VALID", INT2NUM(1837));
  rb_define_const(eXMLError, "XML_SCHEMAV_CVC_FRACTIONDIGITS_VALID", INT2NUM(1838));
  rb_define_const(eXMLError, "XML_SCHEMAV_CVC_PATTERN_VALID", INT2NUM(1839));
  rb_define_const(eXMLError, "XML_SCHEMAV_CVC_ENUMERATION_VALID", INT2NUM(1840));
  rb_define_const(eXMLError, "XML_SCHEMAV_CVC_COMPLEX_TYPE_2_1", INT2NUM(1841));
  rb_define_const(eXMLError, "XML_SCHEMAV_CVC_COMPLEX_TYPE_2_2", INT2NUM(1842));
  rb_define_const(eXMLError, "XML_SCHEMAV_CVC_COMPLEX_TYPE_2_3", INT2NUM(1843));
  rb_define_const(eXMLError, "XML_SCHEMAV_CVC_COMPLEX_TYPE_2_4", INT2NUM(1844));
  rb_define_const(eXMLError, "XML_SCHEMAV_CVC_ELT_1", INT2NUM(1845));
  rb_define_const(eXMLError, "XML_SCHEMAV_CVC_ELT_2", INT2NUM(1846));
  rb_define_const(eXMLError, "XML_SCHEMAV_CVC_ELT_3_1", INT2NUM(1847));
  rb_define_const(eXMLError, "XML_SCHEMAV_CVC_ELT_3_2_1", INT2NUM(1848));
  rb_define_const(eXMLError, "XML_SCHEMAV_CVC_ELT_3_2_2", INT2NUM(1849));
  rb_define_const(eXMLError, "XML_SCHEMAV_CVC_ELT_4_1", INT2NUM(1850));
  rb_define_const(eXMLError, "XML_SCHEMAV_CVC_ELT_4_2", INT2NUM(1851));
  rb_define_const(eXMLError, "XML_SCHEMAV_CVC_ELT_4_3", INT2NUM(1852));
  rb_define_const(eXMLError, "XML_SCHEMAV_CVC_ELT_5_1_1", INT2NUM(1853));
  rb_define_const(eXMLError, "XML_SCHEMAV_CVC_ELT_5_1_2", INT2NUM(1854));
  rb_define_const(eXMLError, "XML_SCHEMAV_CVC_ELT_5_2_1", INT2NUM(1855));
  rb_define_const(eXMLError, "XML_SCHEMAV_CVC_ELT_5_2_2_1", INT2NUM(1856));
  rb_define_const(eXMLError, "XML_SCHEMAV_CVC_ELT_5_2_2_2_1", INT2NUM(1857));
  rb_define_const(eXMLError, "XML_SCHEMAV_CVC_ELT_5_2_2_2_2", INT2NUM(1858));
  rb_define_const(eXMLError, "XML_SCHEMAV_CVC_ELT_6", INT2NUM(1859));
  rb_define_const(eXMLError, "XML_SCHEMAV_CVC_ELT_7", INT2NUM(1860));
  rb_define_const(eXMLError, "XML_SCHEMAV_CVC_ATTRIBUTE_1", INT2NUM(1861));
  rb_define_const(eXMLError, "XML_SCHEMAV_CVC_ATTRIBUTE_2", INT2NUM(1862));
  rb_define_const(eXMLError, "XML_SCHEMAV_CVC_ATTRIBUTE_3", INT2NUM(1863));
  rb_define_const(eXMLError, "XML_SCHEMAV_CVC_ATTRIBUTE_4", INT2NUM(1864));
  rb_define_const(eXMLError, "XML_SCHEMAV_CVC_COMPLEX_TYPE_3_1", INT2NUM(1865));
  rb_define_const(eXMLError, "XML_SCHEMAV_CVC_COMPLEX_TYPE_3_2_1", INT2NUM(1866));
  rb_define_const(eXMLError, "XML_SCHEMAV_CVC_COMPLEX_TYPE_3_2_2", INT2NUM(1867));
  rb_define_const(eXMLError, "XML_SCHEMAV_CVC_COMPLEX_TYPE_4", INT2NUM(1868));
  rb_define_const(eXMLError, "XML_SCHEMAV_CVC_COMPLEX_TYPE_5_1", INT2NUM(1869));
  rb_define_const(eXMLError, "XML_SCHEMAV_CVC_COMPLEX_TYPE_5_2", INT2NUM(1870));
  rb_define_const(eXMLError, "XML_SCHEMAV_ELEMENT_CONTENT", INT2NUM(1871));
  rb_define_const(eXMLError, "XML_SCHEMAV_DOCUMENT_ELEMENT_MISSING", INT2NUM(1872));
  rb_define_const(eXMLError, "XML_SCHEMAV_CVC_COMPLEX_TYPE_1", INT2NUM(1873));
  rb_define_const(eXMLError, "XML_SCHEMAV_CVC_AU", INT2NUM(1874));
  rb_define_const(eXMLError, "XML_SCHEMAV_CVC_TYPE_1", INT2NUM(1875));
  rb_define_const(eXMLError, "XML_SCHEMAV_CVC_TYPE_2", INT2NUM(1876));
  rb_define_const(eXMLError, "XML_SCHEMAV_CVC_IDC", INT2NUM(1877));
  rb_define_const(eXMLError, "XML_SCHEMAV_CVC_WILDCARD", INT2NUM(1878));
  rb_define_const(eXMLError, "XML_SCHEMAV_MISC", INT2NUM(1879));
  rb_define_const(eXMLError, "XML_XPTR_UNKNOWN_SCHEME", INT2NUM(1900));
  rb_define_const(eXMLError, "XML_XPTR_CHILDSEQ_START", INT2NUM(1901));
  rb_define_const(eXMLError, "XML_XPTR_EVAL_FAILED", INT2NUM(1902));
  rb_define_const(eXMLError, "XML_XPTR_EXTRA_OBJECTS", INT2NUM(1903));
  rb_define_const(eXMLError, "XML_C14N_CREATE_CTXT", INT2NUM(1950));
  rb_define_const(eXMLError, "XML_C14N_REQUIRES_UTF8", INT2NUM(1951));
  rb_define_const(eXMLError, "XML_C14N_CREATE_STACK", INT2NUM(1952));
  rb_define_const(eXMLError, "XML_C14N_INVALID_NODE", INT2NUM(1953));
  rb_define_const(eXMLError, "XML_C14N_UNKNOW_NODE", INT2NUM(1954));
  rb_define_const(eXMLError, "XML_C14N_RELATIVE_NAMESPACE", INT2NUM(1955));
  rb_define_const(eXMLError, "XML_FTP_PASV_ANSWER", INT2NUM(2000));
  rb_define_const(eXMLError, "XML_FTP_EPSV_ANSWER", INT2NUM(2001));
  rb_define_const(eXMLError, "XML_FTP_ACCNT", INT2NUM(2002));
  rb_define_const(eXMLError, "XML_FTP_URL_SYNTAX", INT2NUM(2003));
  rb_define_const(eXMLError, "XML_HTTP_URL_SYNTAX", INT2NUM(2020));
  rb_define_const(eXMLError, "XML_HTTP_USE_IP", INT2NUM(2021));
  rb_define_const(eXMLError, "XML_HTTP_UNKNOWN_HOST", INT2NUM(2022));
  rb_define_const(eXMLError, "XML_SCHEMAP_SRC_SIMPLE_TYPE_1", INT2NUM(3000));
  rb_define_const(eXMLError, "XML_SCHEMAP_SRC_SIMPLE_TYPE_2", INT2NUM(3001));
  rb_define_const(eXMLError, "XML_SCHEMAP_SRC_SIMPLE_TYPE_3", INT2NUM(3002));
  rb_define_const(eXMLError, "XML_SCHEMAP_SRC_SIMPLE_TYPE_4", INT2NUM(3003));
  rb_define_const(eXMLError, "XML_SCHEMAP_SRC_RESOLVE", INT2NUM(3004));
  rb_define_const(eXMLError, "XML_SCHEMAP_SRC_RESTRICTION_BASE_OR_SIMPLETYPE", INT2NUM(3005));
  rb_define_const(eXMLError, "XML_SCHEMAP_SRC_LIST_ITEMTYPE_OR_SIMPLETYPE", INT2NUM(3006));
  rb_define_const(eXMLError, "XML_SCHEMAP_SRC_UNION_MEMBERTYPES_OR_SIMPLETYPES", INT2NUM(3007));
  rb_define_const(eXMLError, "XML_SCHEMAP_ST_PROPS_CORRECT_1", INT2NUM(3008));
  rb_define_const(eXMLError, "XML_SCHEMAP_ST_PROPS_CORRECT_2", INT2NUM(3009));
  rb_define_const(eXMLError, "XML_SCHEMAP_ST_PROPS_CORRECT_3", INT2NUM(3010));
  rb_define_const(eXMLError, "XML_SCHEMAP_COS_ST_RESTRICTS_1_1", INT2NUM(3011));
  rb_define_const(eXMLError, "XML_SCHEMAP_COS_ST_RESTRICTS_1_2", INT2NUM(3012));
  rb_define_const(eXMLError, "XML_SCHEMAP_COS_ST_RESTRICTS_1_3_1", INT2NUM(3013));
  rb_define_const(eXMLError, "XML_SCHEMAP_COS_ST_RESTRICTS_1_3_2", INT2NUM(3014));
  rb_define_const(eXMLError, "XML_SCHEMAP_COS_ST_RESTRICTS_2_1", INT2NUM(3015));
  rb_define_const(eXMLError, "XML_SCHEMAP_COS_ST_RESTRICTS_2_3_1_1", INT2NUM(3016));
  rb_define_const(eXMLError, "XML_SCHEMAP_COS_ST_RESTRICTS_2_3_1_2", INT2NUM(3017));
  rb_define_const(eXMLError, "XML_SCHEMAP_COS_ST_RESTRICTS_2_3_2_1", INT2NUM(3018));
  rb_define_const(eXMLError, "XML_SCHEMAP_COS_ST_RESTRICTS_2_3_2_2", INT2NUM(3019));
  rb_define_const(eXMLError, "XML_SCHEMAP_COS_ST_RESTRICTS_2_3_2_3", INT2NUM(3020));
  rb_define_const(eXMLError, "XML_SCHEMAP_COS_ST_RESTRICTS_2_3_2_4", INT2NUM(3021));
  rb_define_const(eXMLError, "XML_SCHEMAP_COS_ST_RESTRICTS_2_3_2_5", INT2NUM(3022));
  rb_define_const(eXMLError, "XML_SCHEMAP_COS_ST_RESTRICTS_3_1", INT2NUM(3023));
  rb_define_const(eXMLError, "XML_SCHEMAP_COS_ST_RESTRICTS_3_3_1", INT2NUM(3024));
  rb_define_const(eXMLError, "XML_SCHEMAP_COS_ST_RESTRICTS_3_3_1_2", INT2NUM(3025));
  rb_define_const(eXMLError, "XML_SCHEMAP_COS_ST_RESTRICTS_3_3_2_2", INT2NUM(3026));
  rb_define_const(eXMLError, "XML_SCHEMAP_COS_ST_RESTRICTS_3_3_2_1", INT2NUM(3027));
  rb_define_const(eXMLError, "XML_SCHEMAP_COS_ST_RESTRICTS_3_3_2_3", INT2NUM(3028));
  rb_define_const(eXMLError, "XML_SCHEMAP_COS_ST_RESTRICTS_3_3_2_4", INT2NUM(3029));
  rb_define_const(eXMLError, "XML_SCHEMAP_COS_ST_RESTRICTS_3_3_2_5", INT2NUM(3030));
  rb_define_const(eXMLError, "XML_SCHEMAP_COS_ST_DERIVED_OK_2_1", INT2NUM(3031));
  rb_define_const(eXMLError, "XML_SCHEMAP_COS_ST_DERIVED_OK_2_2", INT2NUM(3032));
  rb_define_const(eXMLError, "XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED", INT2NUM(3033));
  rb_define_const(eXMLError, "XML_SCHEMAP_S4S_ELEM_MISSING", INT2NUM(3034));
  rb_define_const(eXMLError, "XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED", INT2NUM(3035));
  rb_define_const(eXMLError, "XML_SCHEMAP_S4S_ATTR_MISSING", INT2NUM(3036));
  rb_define_const(eXMLError, "XML_SCHEMAP_S4S_ATTR_INVALID_VALUE", INT2NUM(3037));
  rb_define_const(eXMLError, "XML_SCHEMAP_SRC_ELEMENT_1", INT2NUM(3038));
  rb_define_const(eXMLError, "XML_SCHEMAP_SRC_ELEMENT_2_1", INT2NUM(3039));
  rb_define_const(eXMLError, "XML_SCHEMAP_SRC_ELEMENT_2_2", INT2NUM(3040));
  rb_define_const(eXMLError, "XML_SCHEMAP_SRC_ELEMENT_3", INT2NUM(3041));
  rb_define_const(eXMLError, "XML_SCHEMAP_P_PROPS_CORRECT_1", INT2NUM(3042));
  rb_define_const(eXMLError, "XML_SCHEMAP_P_PROPS_CORRECT_2_1", INT2NUM(3043));
  rb_define_const(eXMLError, "XML_SCHEMAP_P_PROPS_CORRECT_2_2", INT2NUM(3044));
  rb_define_const(eXMLError, "XML_SCHEMAP_E_PROPS_CORRECT_2", INT2NUM(3045));
  rb_define_const(eXMLError, "XML_SCHEMAP_E_PROPS_CORRECT_3", INT2NUM(3046));
  rb_define_const(eXMLError, "XML_SCHEMAP_E_PROPS_CORRECT_4", INT2NUM(3047));
  rb_define_const(eXMLError, "XML_SCHEMAP_E_PROPS_CORRECT_5", INT2NUM(3048));
  rb_define_const(eXMLError, "XML_SCHEMAP_E_PROPS_CORRECT_6", INT2NUM(3049));
  rb_define_const(eXMLError, "XML_SCHEMAP_SRC_INCLUDE", INT2NUM(3050));
  rb_define_const(eXMLError, "XML_SCHEMAP_SRC_ATTRIBUTE_1", INT2NUM(3051));
  rb_define_const(eXMLError, "XML_SCHEMAP_SRC_ATTRIBUTE_2", INT2NUM(3052));
  rb_define_const(eXMLError, "XML_SCHEMAP_SRC_ATTRIBUTE_3_1", INT2NUM(3053));
  rb_define_const(eXMLError, "XML_SCHEMAP_SRC_ATTRIBUTE_3_2", INT2NUM(3054));
  rb_define_const(eXMLError, "XML_SCHEMAP_SRC_ATTRIBUTE_4", INT2NUM(3055));
  rb_define_const(eXMLError, "XML_SCHEMAP_NO_XMLNS", INT2NUM(3056));
  rb_define_const(eXMLError, "XML_SCHEMAP_NO_XSI", INT2NUM(3057));
  rb_define_const(eXMLError, "XML_SCHEMAP_COS_VALID_DEFAULT_1", INT2NUM(3058));
  rb_define_const(eXMLError, "XML_SCHEMAP_COS_VALID_DEFAULT_2_1", INT2NUM(3059));
  rb_define_const(eXMLError, "XML_SCHEMAP_COS_VALID_DEFAULT_2_2_1", INT2NUM(3060));
  rb_define_const(eXMLError, "XML_SCHEMAP_COS_VALID_DEFAULT_2_2_2", INT2NUM(3061));
  rb_define_const(eXMLError, "XML_SCHEMAP_CVC_SIMPLE_TYPE", INT2NUM(3062));
  rb_define_const(eXMLError, "XML_SCHEMAP_COS_CT_EXTENDS_1_1", INT2NUM(3063));
  rb_define_const(eXMLError, "XML_SCHEMAP_SRC_IMPORT_1_1", INT2NUM(3064));
  rb_define_const(eXMLError, "XML_SCHEMAP_SRC_IMPORT_1_2", INT2NUM(3065));
  rb_define_const(eXMLError, "XML_SCHEMAP_SRC_IMPORT_2", INT2NUM(3066));
  rb_define_const(eXMLError, "XML_SCHEMAP_SRC_IMPORT_2_1", INT2NUM(3067));
  rb_define_const(eXMLError, "XML_SCHEMAP_SRC_IMPORT_2_2", INT2NUM(3068));
  rb_define_const(eXMLError, "XML_SCHEMAP_INTERNAL", INT2NUM(3069));
  rb_define_const(eXMLError, "XML_SCHEMAP_NOT_DETERMINISTIC", INT2NUM(3070));
  rb_define_const(eXMLError, "XML_SCHEMAP_SRC_ATTRIBUTE_GROUP_1", INT2NUM(3071));
  rb_define_const(eXMLError, "XML_SCHEMAP_SRC_ATTRIBUTE_GROUP_2", INT2NUM(3072));
  rb_define_const(eXMLError, "XML_SCHEMAP_SRC_ATTRIBUTE_GROUP_3", INT2NUM(3073));
  rb_define_const(eXMLError, "XML_SCHEMAP_MG_PROPS_CORRECT_1", INT2NUM(3074));
  rb_define_const(eXMLError, "XML_SCHEMAP_MG_PROPS_CORRECT_2", INT2NUM(3075));
  rb_define_const(eXMLError, "XML_SCHEMAP_SRC_CT_1", INT2NUM(3076));
  rb_define_const(eXMLError, "XML_SCHEMAP_DERIVATION_OK_RESTRICTION_2_1_3", INT2NUM(3077));
  rb_define_const(eXMLError, "XML_SCHEMAP_AU_PROPS_CORRECT_2", INT2NUM(3078));
  rb_define_const(eXMLError, "XML_SCHEMAP_A_PROPS_CORRECT_2", INT2NUM(3079));
  rb_define_const(eXMLError, "XML_SCHEMAP_C_PROPS_CORRECT", INT2NUM(3080));
  rb_define_const(eXMLError, "XML_SCHEMAP_SRC_REDEFINE", INT2NUM(3081));
  rb_define_const(eXMLError, "XML_SCHEMAP_SRC_IMPORT", INT2NUM(3082));
  rb_define_const(eXMLError, "XML_SCHEMAP_WARN_SKIP_SCHEMA", INT2NUM(3083));
  rb_define_const(eXMLError, "XML_SCHEMAP_WARN_UNLOCATED_SCHEMA", INT2NUM(3084));
  rb_define_const(eXMLError, "XML_SCHEMAP_WARN_ATTR_REDECL_PROH", INT2NUM(3085));
  rb_define_const(eXMLError, "XML_SCHEMAP_WARN_ATTR_POINTLESS_PROH", INT2NUM(3085));
  rb_define_const(eXMLError, "XML_SCHEMAP_AG_PROPS_CORRECT", INT2NUM(3086));
  rb_define_const(eXMLError, "XML_SCHEMAP_COS_CT_EXTENDS_1_2", INT2NUM(3087));
  rb_define_const(eXMLError, "XML_SCHEMAP_AU_PROPS_CORRECT", INT2NUM(3088));
  rb_define_const(eXMLError, "XML_SCHEMAP_A_PROPS_CORRECT_3", INT2NUM(3089));
  rb_define_const(eXMLError, "XML_SCHEMAP_COS_ALL_LIMITED", INT2NUM(3090));
  rb_define_const(eXMLError, "XML_SCHEMATRONV_ASSERT", INT2NUM(4000));
  rb_define_const(eXMLError, "XML_SCHEMATRONV_REPORT", INT2NUM(4001));
  rb_define_const(eXMLError, "XML_MODULE_OPEN", INT2NUM(4900));
  rb_define_const(eXMLError, "XML_MODULE_CLOSE", INT2NUM(4901));
  rb_define_const(eXMLError, "XML_CHECK_FOUND_ELEMENT", INT2NUM(5000));
  rb_define_const(eXMLError, "XML_CHECK_FOUND_ATTRIBUTE", INT2NUM(5001));
  rb_define_const(eXMLError, "XML_CHECK_FOUND_TEXT", INT2NUM(5002));
  rb_define_const(eXMLError, "XML_CHECK_FOUND_CDATA", INT2NUM(5003));
  rb_define_const(eXMLError, "XML_CHECK_FOUND_ENTITYREF", INT2NUM(5004));
  rb_define_const(eXMLError, "XML_CHECK_FOUND_ENTITY", INT2NUM(5005));
  rb_define_const(eXMLError, "XML_CHECK_FOUND_PI", INT2NUM(5006));
  rb_define_const(eXMLError, "XML_CHECK_FOUND_COMMENT", INT2NUM(5007));
  rb_define_const(eXMLError, "XML_CHECK_FOUND_DOCTYPE", INT2NUM(5008));
  rb_define_const(eXMLError, "XML_CHECK_FOUND_FRAGMENT", INT2NUM(5009));
  rb_define_const(eXMLError, "XML_CHECK_FOUND_NOTATION", INT2NUM(5010));
  rb_define_const(eXMLError, "XML_CHECK_UNKNOWN_NODE", INT2NUM(5011));
  rb_define_const(eXMLError, "XML_CHECK_ENTITY_TYPE", INT2NUM(5012));
  rb_define_const(eXMLError, "XML_CHECK_NO_PARENT", INT2NUM(5013));
  rb_define_const(eXMLError, "XML_CHECK_NO_DOC", INT2NUM(5014));
  rb_define_const(eXMLError, "XML_CHECK_NO_NAME", INT2NUM(5015));
  rb_define_const(eXMLError, "XML_CHECK_NO_ELEM", INT2NUM(5016));
  rb_define_const(eXMLError, "XML_CHECK_WRONG_DOC", INT2NUM(5017));
  rb_define_const(eXMLError, "XML_CHECK_NO_PREV", INT2NUM(5018));
  rb_define_const(eXMLError, "XML_CHECK_WRONG_PREV", INT2NUM(5019));
  rb_define_const(eXMLError, "XML_CHECK_NO_NEXT", INT2NUM(5020));
  rb_define_const(eXMLError, "XML_CHECK_WRONG_NEXT", INT2NUM(5021));
  rb_define_const(eXMLError, "XML_CHECK_NOT_DTD", INT2NUM(5022));
  rb_define_const(eXMLError, "XML_CHECK_NOT_ATTR", INT2NUM(5023));
  rb_define_const(eXMLError, "XML_CHECK_NOT_ATTR_DECL", INT2NUM(5024));
  rb_define_const(eXMLError, "XML_CHECK_NOT_ELEM_DECL", INT2NUM(5025));
  rb_define_const(eXMLError, "XML_CHECK_NOT_ENTITY_DECL", INT2NUM(5026));
  rb_define_const(eXMLError, "XML_CHECK_NOT_NS_DECL", INT2NUM(5027));
  rb_define_const(eXMLError, "XML_CHECK_NO_HREF", INT2NUM(5028));
  rb_define_const(eXMLError, "XML_CHECK_WRONG_PARENT", INT2NUM(5029));
  rb_define_const(eXMLError, "XML_CHECK_NS_SCOPE", INT2NUM(5030));
  rb_define_const(eXMLError, "XML_CHECK_NS_ANCESTOR", INT2NUM(5031));
  rb_define_const(eXMLError, "XML_CHECK_NOT_UTF8", INT2NUM(5032));
  rb_define_const(eXMLError, "XML_CHECK_NO_DICT", INT2NUM(5033));
  rb_define_const(eXMLError, "XML_CHECK_NOT_NCNAME", INT2NUM(5034));
  rb_define_const(eXMLError, "XML_CHECK_OUTSIDE_DICT", INT2NUM(5035));
  rb_define_const(eXMLError, "XML_CHECK_WRONG_NAME", INT2NUM(5036));
  rb_define_const(eXMLError, "XML_CHECK_NAME_NOT_NULL", INT2NUM(5037));
  rb_define_const(eXMLError, "XML_I18N_NO_NAME", INT2NUM(6000));
  rb_define_const(eXMLError, "XML_I18N_NO_HANDLER", INT2NUM(6001));
  rb_define_const(eXMLError, "XML_I18N_EXCESS_HANDLER", INT2NUM(6002));
  rb_define_const(eXMLError, "XML_I18N_CONV_FAILED", INT2NUM(6003));
  rb_define_const(eXMLError, "XML_I18N_NO_OUTPUT", INT2NUM(6004));
  #if 0
  rb_define_const(eXMLError, "XML_CHECK_", INT2NUM(5033));
  rb_define_const(eXMLError, "XML_CHECK_X", INT2NUM(503));
  #endif
}
