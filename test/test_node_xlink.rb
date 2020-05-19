# encoding: UTF-8

# $Id$
require_relative './test_helper'


class TC_XML_Node_XLink < Minitest::Test
  def setup()
    xp = LibXML::XML::Parser.string('<ruby_array xmlns:xlink="http://www.w3.org/1999/xlink/namespace/"><fixnum xlink:type="simple">one</fixnum></ruby_array>')
    doc = xp.parse
    assert_instance_of(LibXML::XML::Document, doc)
    @root = doc.root
    assert_instance_of(LibXML::XML::Node, @root)
  end

  def teardown()
    @root = nil
  end

  def test_xml_node_xlink()
    for elem in @root.find('fixnum')
      assert_instance_of(LibXML::XML::Node, elem)
      assert_instance_of(TrueClass, elem.xlink?)
      assert_equal("simple", elem.xlink_type_name)
      assert_equal(LibXML::XML::Node::XLINK_TYPE_SIMPLE, elem.xlink_type)
    end
  end
end
