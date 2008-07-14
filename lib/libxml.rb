# $Id$ 
# Please see the LICENSE file for copyright and distribution information 

require 'libxml_ruby'

require 'libxml/sibling_enum'
require 'libxml/parser'
require 'libxml/parser_options'
require 'libxml/document'
require 'libxml/node'
require 'libxml/node_set'
require 'libxml/attributes'
require 'libxml/attr'

# Deprecated
require 'libxml/properties'

# Deprecated.  Map to XML module for backwards compatibility
module XML
  include LibXML
end