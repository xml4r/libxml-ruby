# $Id$
require "libxml_test"
require 'test/unit'

class TC_XML_Node2 < Test::Unit::TestCase
  def setup()
    xp = XML::Parser.new()
    str = '<ruby_array uga="booga" foo="bar"><fixnum>one</fixnum><fixnum>two</fixnum></ruby_array>'
    assert_equal(str, xp.string = str)
    doc = xp.parse
    assert_instance_of(XML::Document, doc)
    assert_instance_of(XML::Node, doc.root)
    @root = doc.root
    assert_instance_of(XML::Node, @root)
  end

  def teardown()
    @root = nil
  end

  def test_xml_node_doc_get()
    GC.start
    assert_instance_of(XML::Node, @root)
    assert_instance_of(XML::Document, @root.doc)
  end
end
