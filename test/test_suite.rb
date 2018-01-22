# encoding: UTF-8

# Change to current directory so relative
# requires work.
dir = File.dirname(__FILE__)
Dir.chdir(dir)

require './test_attr'
require './test_attr_decl'
require './test_attributes'
require './test_canonicalize'
require './test_document'
require './test_document_write'
require './test_dtd'
require './test_error'
require './test_html_parser'
require './test_html_parser_context'
require './test_namespace'
require './test_namespaces'
require './test_node'
require './test_node_cdata'
require './test_node_comment'
require './test_node_copy'
require './test_node_edit'
require './test_node_pi'
require './test_node_text'
require './test_node_write'
require './test_node_xlink'
require './test_parser'
require './test_parser_context'
require './test_reader'
require './test_relaxng'
require './test_sax_parser'
require './test_schema'
require './test_traversal'
require './test_writer'
require './test_xinclude'
require './test_xpath'
require './test_xpath_context'
require './test_xpath_expression'
require './test_xpointer'

if defined?(Encoding)
  require './test_encoding'
  require './test_encoding_sax'
end
# Compatibility
require './test_properties'
require './test_deprecated_require'