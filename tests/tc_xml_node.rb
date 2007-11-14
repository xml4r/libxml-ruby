# $Id$
require "libxml_test"
require 'test/unit'

class TC_XML_Node < Test::Unit::TestCase
  def setup()
    xp = XML::Parser.new()
    str = '<ruby_array uga="booga" foo="bar"><fixnum>one</fixnum><fixnum>two</fixnum></ruby_array>'
    assert_equal(str, xp.string = str)
    doc = xp.parse
    assert_instance_of(XML::Document, doc)
    @root = doc.root
    assert_instance_of(XML::Node, @root)
    set = doc.find('/ruby_array/fixnum')
    assert_instance_of(XML::XPath::Object, set)
    assert_raise(NoMethodError) {
      xpt = set.xpath
    }
    @nodes = []
    set.each do |n|
      @nodes.push(n)
    end
  end

  def teardown()
    @nodes = nil
    @root = nil
  end

  def test_libxml_node_class()
    for n in @nodes
      assert_instance_of(XML::Node, n)
    end
  end

  def test_libxml_node_child_get()
    assert_instance_of(TrueClass, @root.child?)
    assert_instance_of(XML::Node, @root.child)
    assert_equal('fixnum', @root.child.name)
  end

  def test_libxml_node_doc()
    for n in @nodes
      assert_instance_of(XML::Document, n.doc) if n.document?
    end
  end

  def test_libxml_node_type_name()
    assert_equal('element', @nodes[0].node_type_name)
    assert_equal('element', @nodes[1].node_type_name)
  end

  def test_libxml_node_find()
    set = @root.find('./fixnum').set
    assert_instance_of(XML::Node::Set, set)
    for node in set
      assert_instance_of(XML::Node, node)
    end
  end
end # TC_XML_Document
