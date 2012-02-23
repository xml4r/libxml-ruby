# encoding: UTF-8
require './test_helper'
require 'test/unit'

class TestDocument < Test::Unit::TestCase
  def setup
    xp = XML::Parser.string('<ruby_array uga="booga" foo="bar"><fixnum>one</fixnum><fixnum>two</fixnum></ruby_array>')
    assert_instance_of(XML::Parser, xp)
    @doc = xp.parse
    assert_instance_of(XML::Document, @doc)
  end

  def teardown
    @doc = nil
  end

  def test_klass
    assert_instance_of(XML::Document, @doc)
  end

  def test_context
    context = @doc.context
    assert_instance_of(XML::XPath::Context, context)
  end

  def test_find
    set = @doc.find('/ruby_array/fixnum')
    assert_instance_of(XML::XPath::Object, set)
    assert_raise(NoMethodError) {
      xpt = set.xpath
    }
  end

  # (www.w3.org) 3.1 PIs, Comments, and Outside of Document Element
  # http://www.w3.org/TR/xml-c14n#Example-OutsideDoc
  def test_canonicalize_with_w3c_c14n_3_1
    given_doc = XML::Document.file('c14n/given/example-1.xml')

    # With Comments
    expected_with_comments = IO.read('c14n/result/with-comments/example-1')
    assert_equal(expected_with_comments, given_doc.canonicalize(:comments => true))

    # Without Comments
    expected_without_comments = IO.read('c14n/result/without-comments/example-1')
    assert_equal(expected_without_comments, given_doc.canonicalize)
    assert_equal(expected_without_comments, given_doc.canonicalize(:comments => false))
    
    # Without Comments (XML_C14N_1_1)
    expected_1_1_without_comments = IO.read('c14n/result/1-1-without-comments/example-1')
    mode = XML::Document::XML_C14N_1_1
    assert_equal(expected_1_1_without_comments, given_doc.canonicalize(:mode => mode))
  end#test_canonicalize_with_w3c_c14n_3_1


  # (www.w3.org) 3.2 Whitespace in Document Content
  # http://www.w3.org/TR/xml-c14n#Example-WhitespaceInContent
  def test_canonicalize_with_w3c_c14n_3_2
    given_doc = XML::Document.file('c14n/given/example-2.xml')
    expected = IO.read('c14n/result/without-comments/example-2')
    assert_equal(expected, given_doc.canonicalize)

    expected_1_1_without_comments = IO.read('c14n/result/1-1-without-comments/example-2')
    mode = XML::Document::XML_C14N_1_1
    assert_equal(expected_1_1_without_comments, given_doc.canonicalize(:mode => mode))
  end#test_canonicalize_with_w3c_c14n_3_2


  # (www.w3.org) 3.3 Start and End Tags
  # http://www.w3.org/TR/xml-c14n#Example-SETags
  # (2012-02-20) Test failing due to missing 'attr' in 'e9' node.
  #   - Embedded DTD not parsed out of XML, therefore default attributes are 
  #     not applied to canonicalization.
  def test_canonicalize_with_w3c_c14n_3_3
    given_doc = XML::Document.file('c14n/given/example-3.xml')
    expected = IO.read('c14n/result/without-comments/example-3')
    assert_equal(expected, given_doc.canonicalize)

    expected_1_1_without_comments = IO.read('c14n/result/1-1-without-comments/example-3')
    mode = XML::Document::XML_C14N_1_1
    assert_equal(expected_1_1_without_comments, given_doc.canonicalize(:mode => mode))
  end#test_canonicalize_with_w3c_c14n_3_3


  # (www.w3.org) 3.4 Character Modifications and Character References
  # http://www.w3.org/TR/xml-c14n#Example-Chars
  def test_canonicalize_with_w3c_c14n_3_4
    given_doc = XML::Document.file('c14n/given/example-4.xml')
    expected = IO.read('c14n/result/without-comments/example-4')
    assert_equal(expected, given_doc.canonicalize)

    expected_1_1_without_comments = IO.read('c14n/result/1-1-without-comments/example-4')
    mode = XML::Document::XML_C14N_1_1
    assert_equal(expected_1_1_without_comments, given_doc.canonicalize(:mode => mode))
  end#test_canonicalize_with_w3c_c14n_3_4


  # (www.w3.org) 3.5 Entity References
  # http://www.w3.org/TR/xml-c14n#Example-Entities
  # (2012-02-20) Failing likely due to a logic error
  #   - libxml2(c14n.c:1788) XML_ENTITY_REF_NODE is invalid node for parsing. 
  #     CANNOT COMPLETE TEST unless libxml2 supports additional node types.
  #def test_canonicalize_with_w3c_c14n_3_5
  #  given_doc = XML::Document.file('c14n/given/example-5.xml')
  #  # With Comments
  #  expected_with_comments = IO.read('c14n/result/with-comments/example-5')
  #  assert_equal(expected_with_comments, 
  #               given_doc.canonicalize(:comments => true))
  #  # Without Comments
  #  expected_without_comments = IO.read('c14n/result/without-comments/example-5')
  #  assert_equal(expected_without_comments, 
  #               given_doc.canonicalize(:comments => false))
  #  expected_1_1_without_comments = IO.read('c14n/result/1-1-without-comments/example-5')
  #  mode = XML::Document::XML_C14N_1_1
  #  assert_equal(expected_1_1_without_comments, given_doc.canonicalize(:mode => mode))
  #end#test_canonicalize_with_w3c_c14n_3_5


  # (www.w3.org) 3.6 UTF-8 Encoding
  # http://www.w3.org/TR/xml-c14n#Example-UTF8
  def test_canonicalize_with_w3c_c14n_3_6
    given_doc = XML::Document.file('c14n/given/example-6.xml')
    expected = IO.read('c14n/result/without-comments/example-6')
    assert_equal(expected, given_doc.canonicalize)

    expected_1_1_without_comments = IO.read('c14n/result/1-1-without-comments/example-6')
    mode = XML::Document::XML_C14N_1_1
    assert_equal(expected_1_1_without_comments, given_doc.canonicalize(:mode => mode))
  end#test_canonicalize_with_w3c_c14n_3_6


  # (www.w3.org) 3.7 Document Subsets
  # http://www.w3.org/TR/xml-c14n#Example-DocSubsets
  #   - Cannot run against XPath expression stored in file.
  #     - Unless I'm doing something wrong
  #def test_canonicalize_with_w3c_c14n_3_7
  #  # Non Canonicalized Document
  #  given_doc = XML::Document.file('c14n/given/example-7.xml')
  #  expected = IO.read('c14n/result/without-comments/example-7')
  #  subdoc_nodes = given_doc.find(
  #    "(//.|//@*|//namespace::*)",
  #    [ "self::ietf:e1 or (parent::ietf:e1 and not(self::test() or self::e2))", 
  #      'count(id("E3")|ancestor-or-self::node()) = count(ancestor-or-self::node())'
  #    ]
  #  )
  #  # Assertions
  #  assert_equal(expected, given_doc.canonicalize(:nodes => subdoc_nodes))
  #end#test_canonicalize_with_w3c_c14n_3_7


  def test_compression
    if XML.enabled_zlib?
      0.upto(9) do |i|
        assert_equal(i, @doc.compression = i)
        assert_equal(i, @doc.compression)
      end

      9.downto(0) do |i|
        assert_equal(i, @doc.compression = i)
        assert_equal(i, @doc.compression)
      end

      10.upto(20) do |i|
        # assert_equal(9, @doc.compression = i)
        assert_equal(i, @doc.compression = i) # This works around a bug in Ruby 1.8
        assert_equal(9, @doc.compression)
      end

      -1.downto(-10) do |i|
        # assert_equal(0, @doc.compression = i)
        assert_equal(i, @doc.compression = i) # FIXME This bug should get fixed ASAP
        assert_equal(0, @doc.compression)
      end
    end
  end

  def test_version
    assert_equal('1.0', @doc.version)

    doc = XML::Document.new('6.9')
    assert_equal('6.9', doc.version)
  end

  def test_write_root
    @doc.root = XML::Node.new('rubynet')
    assert_instance_of(XML::Node, @doc.root)
    assert_instance_of(XML::Document, @doc.root.doc)
    assert_equal("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<rubynet/>\n",
                 @doc.to_s(:indent => false))
  end

  def test_doc_node_type
    assert_equal(XML::Node::DOCUMENT_NODE, XML::Document.new.node_type)
  end

  def test_doc_node_type_name
    assert_equal('document_xml', XML::Document.new.node_type_name)
  end

  def test_xhtml
		doc = XML::Document.new
		assert(!doc.xhtml?)
    XML::Dtd.new "-//W3C//DTD XHTML 1.0 Transitional//EN", "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd", nil, doc, true
		assert(doc.xhtml?)
	end

  def test_document_root
    doc1 = LibXML::XML::Document.string("<one/>")
    doc2 = LibXML::XML::Document.string("<two/>")

    error = assert_raise(XML::Error) do
      doc1.root = doc2.root
    end
    assert_equal(" Nodes belong to different documents.  You must first import the node by calling XML::Document.import.",
                 error.to_s)

    doc2.root << doc2.import(doc1.root)
    assert_equal('<one/>', doc1.root.to_s)
    assert_equal('<two><one/></two>', doc2.root.to_s(:indent => false))

    assert(!doc1.root.equal?(doc2.root))
    assert(doc1.root.doc != doc2.root.doc)
  end

  def test_import_node
    doc1 = XML::Parser.string('<nums><one></one></nums>').parse
    doc2 = XML::Parser.string('<nums><two></two></nums>').parse

    node = doc1.root.child

    error = assert_raise(XML::Error) do
      doc2.root << node
    end
    assert_equal(" Nodes belong to different documents.  You must first import the node by calling XML::Document.import.",
                 error.to_s)

    doc2.root << doc2.import(node)

    assert_equal("<nums><two/><one/></nums>",
                 doc2.root.to_s(:indent => false))
  end
end
