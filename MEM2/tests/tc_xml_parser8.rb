# $Id$
require "libxml_test"
require 'test/unit'

class TC_XML_Parser8 < Test::Unit::TestCase
  def test_ruby_xml_parser_error_handler  
    $stderr.puts "\nEXPECTING: TWO ERRORS:"
    # this will send message to stderr
    assert_raise(XML::Parser::ParseError) {
      d = XML::Parser.string('<foo><bar/></foz>').parse    
    }
  
    ary = []  
    assert_nil XML::Parser.register_error_handler(lambda { |msg| ary << msg })
    
    # this will use our error handler
    assert_raise(XML::Parser::ParseError) {
      d = XML::Parser.string('<foo><bar/></foz>').parse         
    }
    
    assert (first_len = ary.length) > 0

    assert_instance_of Proc, XML::Parser.register_error_handler(nil)
    
    # this will go to stderr again
    assert_raise(XML::Parser::ParseError) {
      d = XML::Parser.string('<foo><bar/></foz>').parse    
    }
      
    assert_equal first_len, ary.length    
  end
end
