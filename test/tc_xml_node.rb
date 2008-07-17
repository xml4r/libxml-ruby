require "xml"
require 'test/unit'

class TestNode < Test::Unit::TestCase
  def setup
    xp = XML::Parser.new()
    str = '<ruby_array uga="booga" foo="bar"><fixnum>one</fixnum><fixnum>two</fixnum></ruby_array>'
    assert_equal(str, xp.string = str)
    @doc = xp.parse
  end
  
  def teardown
    @doc = nil
  end
  
  def nodes
    @doc.find('/ruby_array/fixnum')
  end

  def test_doc_class
    assert_instance_of(XML::Document, @doc)
  end
  
  def test_root_class
    assert_instance_of(XML::Node, @doc.root)
  end
  
  def test_node_class
    for n in nodes
      assert_instance_of(XML::Node, n)
    end
  end

  def test_find_class
    set = @doc.find('/ruby_array/fixnum')
    assert_instance_of(XML::XPath::Object, set)
  end

  def test_node_child_get
    assert_instance_of(TrueClass, @doc.root.child?)
    assert_instance_of(XML::Node, @doc.root.child)
    assert_equal('fixnum', @doc.root.child.name)
  end

  def test_node_doc
    for n in nodes
      assert_instance_of(XML::Document, n.doc) if n.document?
    end
  end

  def test_node_type_name
    assert_equal('element', nodes[0].node_type_name)
    assert_equal('element', nodes[1].node_type_name)
  end

  def test_node_find
    set = @doc.root.find('./fixnum').set
    assert_instance_of(XML::Node::Set, set)
    for node in set
      assert_instance_of(XML::Node, node)
    end
  end
  
  def test_equality
    node_a = @doc.find('/ruby_array/fixnum').first
    node_b = @doc.root.child
    assert(node_a == node_b)
    assert(node_a.eql?(node_b))
    assert(node_a.equal?(node_b))
    
    xp2 = XML::Parser.new()
    xp2.string = '<ruby_array uga="booga" foo="bar"><fixnum>one</fixnum><fixnum>two</fixnum></ruby_array>'
    doc2 = xp2.parse

    node_a2 = doc2.find('/ruby_array/fixnum').first
    
    assert(node_a.to_s == node_a2.to_s)
    assert(node_a == node_a2)
    assert(node_a.eql?(node_a2))
    assert(!node_a.equal?(node_a2))
  end
      
  def test_equality_nil
    node = @doc.root
    assert(node != nil)
  end
  
  def test_equality_wrong_type
    node = @doc.root
    
    assert_raises(TypeError) do
      assert(node != 'abc')
    end
  end

  def test_content()
    assert_equal('onetwo', @doc.root.content)
    
    first = @doc.root.child
    assert_equal('one', first.content)
    assert_equal('two', first.next.content)
  end
  
  def test_base
    doc = XML::Parser.string('<person />').parse
    assert_nil(doc.root.base)
  end
end
