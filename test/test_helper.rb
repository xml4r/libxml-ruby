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

# Moves every movable object on the heap.
#
# Tests that use this method must keep the object under test in a heap
# container, for example an Array. An object in a live local variable is
# pinned by the conservative machine stack scan, so it never moves and the
# test cannot detect a stale address.
def compact_heap
  if GC.respond_to?(:verify_compaction_references)
    begin
      GC.verify_compaction_references(expand_heap: true, toward: :empty)
    rescue NotImplementedError => e
      skip("GC compaction is not available: #{e.message}")
    end
  else
    skip('GC compaction is not available')
  end
end

STDOUT.write "\nlibxml2: #{LibXML::XML::LIBXML_VERSION}\n#{RUBY_DESCRIPTION}\n\n"

require 'minitest/autorun'
Minitest.load_plugins

