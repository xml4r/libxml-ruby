# $Id$ 
# Please see the LICENSE file for copyright and distribution information 

require 'libxml_ruby'

require 'libxml/parser'
require 'libxml/parser_options'
require 'libxml/document'
require 'libxml/node'
require 'libxml/node_set'
require 'libxml/attributes'
require 'libxml/attr'
require 'libxml/tree'

# Deprecated
require 'libxml/properties'

# The XML namespace is deprecated and should no longer be used.  Instead,
# please change code to use the LibXML namespace.
module XML
  include LibXML
end