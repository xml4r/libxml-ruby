# $Id$
require "libxml_test"
require 'test/unit'

class TC_XML_Node7 < Test::Unit::TestCase
  def setup()
    xp = XML::Parser.string('<ruby_array uga="booga" foo="bar"><fixnum>one</fixnum><fixnum>two</fixnum></ruby_array>')
    doc = xp.parse
    @root = doc.root
    assert_equal 'bar', @root['foo']
  end

  def teardown()
    @root = nil
  end

  def test_xml_node_properties_traversal_bug()
    prop = @root.properties
    assert_instance_of XML::Attr, prop
    assert_equal 'uga', prop.name
    assert_equal 'booga', prop.value
    
    prop = prop.next
    assert_instance_of XML::Attr, prop
    assert_equal 'foo', prop.name
    assert_equal 'bar', prop.value    
  end
  
  def test_xml_node_properties_when_no_attributes
    xp = XML::Parser.string("<root></root>")
    doc = xp.parse
    root = doc.root
    
    assert_nil root.properties
  end
end
