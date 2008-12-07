require 'xml'
require 'test/unit'

class TestNode < Test::Unit::TestCase
  def setup
    # Strip spaces to make testing easier
    XML.default_keep_blanks = false
    file = File.join(File.dirname(__FILE__), 'model/bands.xml')
    @doc = XML::Document.file(file)
  end
  
  def teardown
    XML.default_keep_blanks = true
    @doc = nil
  end
  
  def nodes
    # Find all nodes with a country attributes
    @doc.find('*[@country]')
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

  def test_context
    node = @doc.root
    context = node.context
    assert_instance_of(XML::XPath::Context, context)
  end

  def test_find
    assert_instance_of(XML::XPath::Object, self.nodes)
  end

  def test_node_child_get
    assert_instance_of(TrueClass, @doc.root.child?)
    assert_instance_of(XML::Node, @doc.root.child)
    assert_equal("m\303\266tley_cr\303\274e", @doc.root.child.name)
  end

  def test_node_doc
    for n in nodes
      assert_instance_of(XML::Document, n.doc) if n.document?
    end
  end

  def test_name
    assert_equal("m\303\266tley_cr\303\274e", nodes[0].name)
    assert_equal("iron_maiden", nodes[1].name)
  end

  def test_node_find
    nodes = @doc.root.find('./fixnum')
    for node in nodes
      assert_instance_of(XML::Node, node)
    end
  end

  def test_equality
    node_a = @doc.find_first('*[@country]')
    node_b = @doc.root.child

    assert(node_a == node_b)
    assert(node_a.eql?(node_b))
    assert(node_a.equal?(node_b))

    file = File.join(File.dirname(__FILE__), 'model/bands.xml')
    doc2 = XML::Document.file(file)

    node_a2 = doc2.find_first('*[@country]')

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

  def test_content
    assert_equal("An American heavy metal band formed in Los Angeles, California in 1981.British heavy metal band formed in 1975.",
                 @doc.root.content)

    first = @doc.root.child
    assert_equal('An American heavy metal band formed in Los Angeles, California in 1981.', first.content)
    assert_equal('British heavy metal band formed in 1975.', first.next.content)
  end

  def test_base
    doc = XML::Parser.string('<person />').parse
    assert_nil(doc.root.base)
  end

  def test_to_s_default
    # Default to_s has indentation
    node = @doc.root
    assert_equal("<bands genre=\"metal\">\n  <m\303\266tley_cr\303\274e country=\"us\">An American heavy metal band formed in Los Angeles, California in 1981.</m\303\266tley_cr\303\274e>\n  <iron_maiden country=\"uk\">British heavy metal band formed in 1975.</iron_maiden>\n</bands>",
                 node.to_s)
  end

  def test_to_s_no_global_indentation
    # No indentation due to global setting
    node = @doc.root
    XML.indent_tree_output = false
    assert_equal("<bands genre=\"metal\">\n<m\303\266tley_cr\303\274e country=\"us\">An American heavy metal band formed in Los Angeles, California in 1981.</m\303\266tley_cr\303\274e>\n<iron_maiden country=\"uk\">British heavy metal band formed in 1975.</iron_maiden>\n</bands>",
                 node.to_s)
  ensure
    XML.indent_tree_output = true
  end

  def test_to_s_no_indentation
    # No indentation due to local setting
    node = @doc.root
    assert_equal("<bands genre=\"metal\"><m\303\266tley_cr\303\274e country=\"us\">An American heavy metal band formed in Los Angeles, California in 1981.</m\303\266tley_cr\303\274e><iron_maiden country=\"uk\">British heavy metal band formed in 1975.</iron_maiden></bands>",
                 node.to_s(:indent => false))
  end

  def test_to_s_level
    # No indentation due to local setting
    node = @doc.root
    assert_equal("<bands genre=\"metal\">\n    <m\303\266tley_cr\303\274e country=\"us\">An American heavy metal band formed in Los Angeles, California in 1981.</m\303\266tley_cr\303\274e>\n    <iron_maiden country=\"uk\">British heavy metal band formed in 1975.</iron_maiden>\n  </bands>",
                 node.to_s(:level => 1))
  end

  def test_to_s_encoding
    # Test encodings
    node = @doc.root

    # UTF8:
    # ö - c3 b6 in hex, \303\266 in octal
    # ü - c3 bc in hex, \303\274 in octal
    assert_equal("<bands genre=\"metal\">\n  <m\303\266tley_cr\303\274e country=\"us\">An American heavy metal band formed in Los Angeles, California in 1981.</m\303\266tley_cr\303\274e>\n  <iron_maiden country=\"uk\">British heavy metal band formed in 1975.</iron_maiden>\n</bands>",
                 node.to_s(:encoding => XML::Input::UTF8))

    # ISO_8859_1:
    # ö - f6 in hex, \366 in octal
    # ü - fc in hex, \374 in octal
    assert_equal("<bands genre=\"metal\">\n  <m\366tley_cr\374e country=\"us\">An American heavy metal band formed in Los Angeles, California in 1981.</m\366tley_cr\374e>\n  <iron_maiden country=\"uk\">British heavy metal band formed in 1975.</iron_maiden>\n</bands>",
                 node.to_s(:encoding => XML::Input::ISO_8859_1))


    # Invalid encoding
    error = assert_raise(ArgumentError) do
      node.to_s(:encoding => -9999)
    end
    assert_equal('Unknown encoding constant', error.to_s)
  end
end