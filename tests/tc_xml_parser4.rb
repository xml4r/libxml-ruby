# $Id$
require "libxml_test"
require 'test/unit'

class TC_XML_Parser4 < Test::Unit::TestCase
  def setup()
    @xp1 = XML::Parser.new()
    assert_instance_of(XML::Parser, @xp1)
    s1 = @xp1.string = '<u>a</u>'
    assert_instance_of(String, s1)
    assert_instance_of(String, @xp1.string)
    assert_equal(s1, @xp1.string)

    @xp2 = XML::Parser.string('<u>a</u>')
    assert_instance_of(XML::Parser, @xp2)
    assert_instance_of(String, @xp2.string)
    assert_equal(s1, @xp2.string)
    assert_equal(@xp1.string, @xp2.string)
  end

  def teardown()
    @xp1 = nil
    @xp2 = nil
  end

  def test_ruby_xml_parser_new()
    assert_instance_of(XML::Parser, @xp1)
    assert_instance_of(String, @xp1.string)
    assert_instance_of(XML::Parser, @xp2)
    assert_instance_of(String, @xp2.string)
    assert_equal(@xp1.string, @xp2.string)
  end
end # TC_XML_Document
