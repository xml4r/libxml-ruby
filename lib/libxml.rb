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

# Map the LibXML module into the XML module for both backwards
# compatibility and ease of use.
module XML
  include LibXML::XML
end