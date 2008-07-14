# $Id$
require "libxml"
require 'test/unit'

class TC_XML_Node_XLink < Test::Unit::TestCase
  def setup()
    xp = LibXML::Parser.new()
    str = '<ruby_array xmlns:xlink="http://www.w3.org/1999/xlink/namespace/"><fixnum xlink:type="simple">one</fixnum></ruby_array>'
    assert_equal(str, xp.string = str)
    doc = xp.parse
    assert_instance_of(LibXML::Document, doc)
    @root = doc.root
    assert_instance_of(LibXML::Node, @root)
  end

  def teardown()
    @root = nil
  end

  def test_xml_node_xlink()
    for elem in @root.find('fixnum')
      assert_instance_of(LibXML::Node, elem)
      assert_instance_of(TrueClass, elem.xlink?)
      assert_equal("simple", elem.xlink_type_name)
      assert_equal(LibXML::Node::XLINK_TYPE_SIMPLE, elem.xlink_type)
    end
  end
end
