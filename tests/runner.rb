#!/usr/bin/env ruby
#
# Allsuite for Eclipse and GEM.
require "libxml_test"

ALL_TESTS = true
TESTS = File.expand_path(File.dirname(__FILE__))

glob = File.join(TESTS, ENV['TESTS'] || 'tc_*.rb')
Dir[glob].each { |fn| require fn }
