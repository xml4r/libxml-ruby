# $Id$ 
# Please see the LICENSE file for copyright and distribution information 

# Load the C-based binding.
require 'libxml_ruby'

# Load Ruby supporting code.
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
# 
# DEPRECATED: Use require 'xml' instead!
#
# include LibXML

