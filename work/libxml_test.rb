$LOAD_PATH.unshift("#{File.dirname(__FILE__)}/../ext")
require "xml/libxml_so" unless defined?(XML)
puts "VERSION: #{XML::Parser::VERSION}"
