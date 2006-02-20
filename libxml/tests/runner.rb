#!/usr/bin/env ruby
#
# Allsuite for Eclipse and GEM.

ALL_TESTS = true
TESTS = File.expand_path(File.dirname(__FILE__))
HOME = File.expand_path(File.join(TESTS,'..'))

#$LOAD_PATH.unshift(HOME)

glob = File.join(TESTS, ENV['TESTS'] || 'tc_*.rb')
Dir[glob].each { |fn| require fn }
