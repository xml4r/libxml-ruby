# $Id$
require "libxml_test"
require 'test/unit'

class TC_XML_Parser2 < Test::Unit::TestCase
  def setup()
    @xp = XML::Parser.new()
  end

  def teardown()
    @xp = nil
  end

  def test_ruby_xml_parser_new()
    assert_instance_of(XML::Parser, @xp)
  end
end # TC_XML_Document
