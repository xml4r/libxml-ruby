# encoding: UTF-8

# To make testing/debugging easier, test within this source tree versus an installed gem

require 'bundler/setup'
require 'minitest/autorun'
require 'libxml-ruby'

STDOUT.write "\nlibxml2: #{LibXML::XML::LIBXML_VERSION}\n#{RUBY_DESCRIPTION}\n\n"
