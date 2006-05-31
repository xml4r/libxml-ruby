# $Id$
require "libxml_test"
require 'test/unit'

class TC_XML_Node6 < Test::Unit::TestCase
  def setup()
    xp = XML::Parser.string('<ruby_array uga="booga" foo="bar"><fixnum>one</fixnum><fixnum>two</fixnum></ruby_array>')
    doc = xp.parse
    @root = doc.root
    assert_equal 'bar', @root['foo']
  end

  def teardown()
    @root = nil
  end

  def test_xml_node_prop_get_str_sym()
    assert_equal @root['foo'], @root[:foo]
    assert_equal @root['uga'], @root[:uga]
  end

  def test_xml_node_prop_set_str_sym()
    @root[:foo] = 'boo'
    @root[:uga] = 'zoo'
    assert_equal 'boo', @root['foo']
    assert_equal 'zoo', @root['uga']
  end
end
