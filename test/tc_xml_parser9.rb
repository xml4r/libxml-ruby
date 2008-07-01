require "libxml"
require 'test/unit'

class TC_XML_Parser2 < Test::Unit::TestCase
  def test_double_parse
    parser = XML::Parser.string("<test>something</test>")
    doc = parser.parse
    parser.parse
  end
end

