# encoding: UTF-8
require_relative './test_helper'

class TestDocument < Minitest::Test
  def setup
    xp = LibXML::XML::Parser.string('<ruby_array uga="booga" foo="bar"><fixnum>one</fixnum><fixnum>two</fixnum></ruby_array>')
    assert_instance_of(LibXML::XML::Parser, xp)
    @doc = xp.parse
    assert_instance_of(LibXML::XML::Document, @doc)
  end

  def teardown
    @doc = nil
  end

  def test_klass
    assert_instance_of(LibXML::XML::Document, @doc)
  end

  def test_context
    context = @doc.context
    assert_instance_of(LibXML::XML::XPath::Context, context)
  end

  def test_find
    set = @doc.find('/ruby_array/fixnum')
    assert_instance_of(LibXML::XML::XPath::Object, set)
    assert_raises(NoMethodError) {
      set.xpath
    }
  end

  def test_compression
    if LibXML::XML.enabled_zlib?
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

    doc = LibXML::XML::Document.new('6.9')
    assert_equal('6.9', doc.version)
  end

  def test_write_root
    @doc.root = LibXML::XML::Node.new('rubynet')
    assert_instance_of(LibXML::XML::Node, @doc.root)
    assert_instance_of(LibXML::XML::Document, @doc.root.doc)
    assert_equal("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<rubynet/>\n",
                 @doc.to_s(:indent => false))
  end

  def test_doc_node_type
    assert_equal(LibXML::XML::Node::DOCUMENT_NODE, LibXML::XML::Document.new.node_type)
  end

  def test_doc_node_type_name
    assert_equal('document_xml', LibXML::XML::Document.new.node_type_name)
  end

  def test_xhtml
		doc = LibXML::XML::Document.new
		assert(!doc.xhtml?)
    LibXML::XML::Dtd.new("-//W3C//DTD XHTML 1.0 Transitional//EN", "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd", nil, doc, true)
		assert(doc.xhtml?)
  end

  def test_document_root
    doc1 = LibXML::XML::Document.string("<one/>")
    doc2 = LibXML::XML::Document.string("<two/>")

    error = assert_raises(LibXML::XML::Error) do
      doc1.root = doc2.root
    end
    assert_equal(" Nodes belong to different documents.  You must first import the node by calling LibXML::XML::Document.import.",
                 error.to_s)

    doc2.root << doc2.import(doc1.root)
    assert_equal('<one/>', doc1.root.to_s)
    assert_equal('<two><one/></two>', doc2.root.to_s(:indent => false))

    assert(!doc1.root.equal?(doc2.root))
    assert(doc1.root.doc != doc2.root.doc)
  end

  def test_import_node
    doc1 = LibXML::XML::Parser.string('<nums><one></one></nums>').parse
    doc2 = LibXML::XML::Parser.string('<nums><two></two></nums>').parse

    node = doc1.root.child

    error = assert_raises(LibXML::XML::Error) do
      doc2.root << node
    end

    assert_equal(" Nodes belong to different documents.  You must first import the node by calling LibXML::XML::Document.import.",
                 error.to_s)

    doc2.root << doc2.import(node)

    assert_equal("<nums><two/><one/></nums>",
                 doc2.root.to_s(:indent => false))
  end

  def test_nonet
    xml_string = '<ruby_array uga="booga" foo="bar"><fixnum>one</fixnum><fixnum>two</fixnum></ruby_array>'
    xml = LibXML::XML::Document.string(xml_string, options: LibXML::XML::Parser::Options::NONET)
    file = File.join(File.dirname(__FILE__), 'model/atom.xml')
    schema_document = LibXML::XML::Document.file(file, options: LibXML::XML::Parser::Options::NONET)
  end

  def test_io
    File.open(File.join(File.dirname(__FILE__), 'model/rubynet.xml')) do |io|
      doc = LibXML::XML::Document.io(io)
      assert_instance_of(LibXML::XML::Document, doc)
    end
  end

end
