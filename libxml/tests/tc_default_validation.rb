# $Id$

require 'libxml' unless defined?(XML)
require 'test/unit'

class TC_XML_Default_Validatity_Check_Regression < Test::Unit::TestCase
  def test_xml_parser_default_validation
    old = XML::Parser.default_validity_checking
    
    XML::Parser.default_validity_checking = !old    
    assert_not_equal(old, XML::Parser.default_validity_checking)
    
    XML::Parser.default_validity_checking = !XML::Parser.default_validity_checking
    assert_equal(old, XML::Parser.default_validity_checking)
  end
end
