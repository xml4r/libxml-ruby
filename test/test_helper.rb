# encoding: UTF-8

# To make testing/debugging easier, test within this source tree versus an installed gem
require 'bundler/setup'

# Add ext directory to load path to make it easier to test locally built extensions
ext_path = File.expand_path(File.join(__dir__, '..', 'ext', 'libxml'))
$LOAD_PATH.unshift(File.expand_path(ext_path))

# Now load code
require 'libxml-ruby'

def windows?
  !(RbConfig::CONFIG['host_os'] =~ /mswin|mingw/).nil?
end

STDOUT.write "\nlibxml2: #{LibXML::XML::LIBXML_VERSION}\n#{RUBY_DESCRIPTION}\n\n"

require 'minitest/autorun'

