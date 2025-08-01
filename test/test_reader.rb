# encoding: UTF-8

require_relative './test_helper'
require 'stringio'

class TestReader < Minitest::Test
  XML_FILE = File.join(File.dirname(__FILE__), 'model/atom.xml')

  def verify_simple(reader)
    node_types = []

    # Read each node
    26.times do
      assert(reader.read)
      node_types << reader.node_type
    end

    # There are no more nodes
    assert(!reader.read)

    # Check what was read
    expected = [LibXML::XML::Reader::TYPE_PROCESSING_INSTRUCTION,
                LibXML::XML::Reader::TYPE_ELEMENT,
                LibXML::XML::Reader::TYPE_SIGNIFICANT_WHITESPACE,
                LibXML::XML::Reader::TYPE_COMMENT,
                LibXML::XML::Reader::TYPE_SIGNIFICANT_WHITESPACE,
                LibXML::XML::Reader::TYPE_ELEMENT,
                LibXML::XML::Reader::TYPE_SIGNIFICANT_WHITESPACE,
                LibXML::XML::Reader::TYPE_ELEMENT,
                LibXML::XML::Reader::TYPE_CDATA,
                LibXML::XML::Reader::TYPE_END_ELEMENT,
                LibXML::XML::Reader::TYPE_SIGNIFICANT_WHITESPACE,
                LibXML::XML::Reader::TYPE_ELEMENT,
                LibXML::XML::Reader::TYPE_SIGNIFICANT_WHITESPACE,
                LibXML::XML::Reader::TYPE_ELEMENT,
                LibXML::XML::Reader::TYPE_SIGNIFICANT_WHITESPACE,
                LibXML::XML::Reader::TYPE_ELEMENT,
                LibXML::XML::Reader::TYPE_TEXT,
                LibXML::XML::Reader::TYPE_END_ELEMENT,
                LibXML::XML::Reader::TYPE_SIGNIFICANT_WHITESPACE,
                LibXML::XML::Reader::TYPE_END_ELEMENT,
                LibXML::XML::Reader::TYPE_SIGNIFICANT_WHITESPACE,
                LibXML::XML::Reader::TYPE_END_ELEMENT,
                LibXML::XML::Reader::TYPE_SIGNIFICANT_WHITESPACE,
                LibXML::XML::Reader::TYPE_END_ELEMENT,
                LibXML::XML::Reader::TYPE_SIGNIFICANT_WHITESPACE,
                LibXML::XML::Reader::TYPE_END_ELEMENT]

    assert_equal(expected, node_types)
  end

  def test_document
    reader = LibXML::XML::Reader.document(LibXML::XML::Document.file(XML_FILE))
    verify_simple(reader)
  end

  def test_file
    reader = LibXML::XML::Reader.file(XML_FILE)
    verify_simple(reader)
  end

  def test_invalid_file
    error = assert_raises(Errno::ENOENT) do
      LibXML::XML::Reader.file('/does/not/exist')
    end
    assert_equal("No such file or directory - /does/not/exist", error.message)
  end

  def test_string
    reader = LibXML::XML::Reader.string(File.read(XML_FILE))
    verify_simple(reader)
  end

  def test_io
    File.open(XML_FILE, 'rb') do |io|
      reader = LibXML::XML::Reader.io(io)
      verify_simple(reader)
    end
  end

  def test_io_gc
    # Test that the reader keeps a reference
    # to the io object
    file = File.open(XML_FILE, 'rb')
    reader = LibXML::XML::Reader.io(file)
    file = nil
    GC.start
    assert(reader.read)
  end

  def test_string_io
    data = File.read(XML_FILE)
    string_io = StringIO.new(data)
    reader = LibXML::XML::Reader.io(string_io)
    verify_simple(reader)
  end

  def test_error
    reader = LibXML::XML::Reader.string('<foo blah')

    error = assert_raises(LibXML::XML::Error) do
      reader.read
    end
    assert_equal("Fatal error: Couldn't find end of Start Tag foo at :1.", error.to_s)
  end

  def test_attr
    parser = LibXML::XML::Reader.string("<foo x='1' y='2'/>")
    assert(parser.read)
    assert_equal('foo', parser.name)
    assert_equal('1', parser['x'])
    assert_equal('1', parser[0])
    assert_equal('2', parser['y'])
    assert_equal('2', parser[1])
    assert_nil(parser['z'])
    assert_nil(parser[2])
  end

  def test_move_attr
      reader = LibXML::XML::Reader.string('<root xmlns:xlink="http://www.w3.org/1999/xlink" xmlns:xhtml="http://www.w3.org/1999/xhtml"><link xml:id="abc" xlink:href="def" xhtml:class="ghi" bar="jkl" /></root>')
      assert(reader.read) # <root/>
      assert(reader.read) # <link/>

      assert(reader.move_to_attribute_no(1))
      assert_equal(reader.value, 'def')
      assert(reader.move_to_attribute_ns('id', 'http://www.w3.org/XML/1998/namespace'))
      assert_equal(reader.value, 'abc')
      assert(reader.move_to_attribute('bar'))
      assert_equal(reader.value, 'jkl')

      # 1 in case of success, -1 in case of error, 0 if not found
      assert_equal(reader.move_to_attribute_no(12), 0)
      assert_equal(reader.move_to_attribute('baz'), 0)
      assert_equal(reader.move_to_attribute_ns('baz', 'http://ruby/namespace'), 0)
  end

  def test_get_attr
      reader = LibXML::XML::Reader.string('<root xmlns:xlink="http://www.w3.org/1999/xlink" xmlns:xhtml="http://www.w3.org/1999/xhtml"><link xml:id="abc" xlink:href="def" xhtml:class="ghi" bar="jkl" /></root>')
      assert(reader.read) # <root/>
      assert(reader.read) # <link/>

      assert_equal(reader.get_attribute_no(1), 'def')
      assert_equal(reader.get_attribute_ns('id', 'http://www.w3.org/XML/1998/namespace'), 'abc')
      assert_equal(reader.get_attribute('bar'), 'jkl')

      assert_nil(reader.get_attribute_no(12))
      assert_nil(reader.get_attribute('baz'))
      assert_nil(reader.get_attribute_ns('baz', 'http://ruby/namespace'))
  end

  def test_value
    parser = LibXML::XML::Reader.string("<foo><bar>1</bar><bar>2</bar><bar>3</bar></foo>")
    assert(parser.read)
    assert_equal('foo', parser.name)
    assert_nil(parser.value)
    3.times do |i|
      assert(parser.read)
      assert_equal(LibXML::XML::Reader::TYPE_ELEMENT, parser.node_type)
      assert_equal('bar', parser.name)
      assert(parser.read)
      assert_equal(LibXML::XML::Reader::TYPE_TEXT, parser.node_type)
      assert_equal((i + 1).to_s, parser.value)
      assert(parser.read)
      assert_equal(LibXML::XML::Reader::TYPE_END_ELEMENT, parser.node_type)
    end
  end

  def test_expand
    reader = LibXML::XML::Reader.file(XML_FILE)
    reader.read.to_s
    reader.read

    # Read a node
    node = reader.expand
    refute_nil(node.doc)
    assert_equal('feed', node.name)
    assert_equal(::Encoding::UTF_8, node.name.encoding) if defined?(::Encoding)
  end

  def test_expand_find
    reader = LibXML::XML::Reader.file(XML_FILE)
    reader.read.to_s
    reader.read

    # Read first node which
    node = reader.expand
    assert_equal('feed', node.name)

    # We need to create document to use xpath
    reader.doc

    # Search for entries
    entries = node.find('atom:entry', 'atom:http://www.w3.org/2005/Atom')

    assert_equal(1, entries.length)
  end

  def test_expand_invalid
    reader = LibXML::XML::Reader.file(XML_FILE)

    # Expand a node before one has been read
    node = reader.expand
    assert_nil(node)
  end

  def test_expand_should_be_invalid
    reader = LibXML::XML::Reader.file(XML_FILE)

    # Read a couple of nodes
    reader.read
    reader.read

    # Expand the node
    node = reader.expand
    assert_equal('feed', node.name)

    # Read another node, this makes the last node invalid
    reader.next

    # The previous node is now invalid - this should be an error but isn't
    assert_equal('feed', node.name)
  end

  def test_expand_incorrectly_use_returned_node
    file = File.join(File.dirname(__FILE__), 'model/cwm_1_0.xml')
    reader = LibXML::XML::Reader.file(file)

    nodes = Array.new
    while reader.read
      node = reader.expand
      refute_nil(node)
      refute_nil(node.doc)

      # NOTE - DO NOT do this in real code, these nodes are invalid after the next read. This *will* cause
      # a segmentation fault next time the garbage collector runs.  The reason is the parent node will be
      # called in the mark phase, but its underlying xml node will be gone.  Same goes for calling children,
      # attributes, etc.  You must let go of the expanded node *before* calling xml reader again and
      # call the garbage collector to be safe.
      #refute_nil(node.parent)
      nodes << node
    end
    assert(true)
  end

  def test_mode
    reader = LibXML::XML::Reader.string('<xml/>')
    assert_equal(LibXML::XML::Reader::MODE_INITIAL, reader.read_state)
    reader.read
    assert_equal(LibXML::XML::Reader::MODE_EOF, reader.read_state)
  end

  def test_bytes_consumed
    ending_are_rn = File.binread(XML_FILE).include? "\r\n"
    reader = LibXML::XML::Reader.file(XML_FILE)
    reader.read
    assert_equal(ending_are_rn ? 428 : 416, reader.byte_consumed)
  end

  def test_node
    reader = LibXML::XML::Reader.file(XML_FILE)

    # first try to get a node
    assert_nil(reader.node)

    reader.read
    assert_instance_of(LibXML::XML::Node, reader.node)
  end

  def test_base_uri
    # UTF8:
    # ö - c3 b6 in hex, \303\266 in octal
    # ü - c3 bc in hex, \303\274 in octal
    xml = "<?xml version=\"1.0\" encoding=\"utf-8\"?><bands genre=\"metal\">\n  <m\303\266tley_cr\303\274e country=\"us\">An American heavy metal band formed in Los Angeles, California in 1981.</m\303\266tley_cr\303\274e>\n  <iron_maiden country=\"uk\">British heavy metal band formed in 1975.</iron_maiden>\n</bands>"
    reader = LibXML::XML::Reader.string(xml, :base_uri => "http://libxml.rubyforge.org")

    reader.read
    assert_equal(reader.base_uri, "http://libxml.rubyforge.org")
    assert_equal(::Encoding::UTF_8, reader.base_uri.encoding) if defined?(::Encoding)
  end

  def test_options
    xml = <<-EOS
      <!DOCTYPE foo [<!ENTITY foo 'bar'>]>
      <test>
        <cdata><![CDATA[something]]></cdata>
        <entity>&foo;</entity>
      </test>
    EOS

    # Parse normally
    reader = LibXML::XML::Reader.string(xml)
    reader.read # foo
    reader.read # test
    reader.read # text
    reader.read # cdata
    reader.read # cdata-section
    assert_equal(LibXML::XML::Node::CDATA_SECTION_NODE, reader.node_type)

    # Convert cdata section to text
    reader = LibXML::XML::Reader.string(xml, :options => LibXML::XML::Parser::Options::NOCDATA)
    reader.read # foo
    reader.read # test
    reader.read # text
    reader.read # cdata
    reader.read # cdata-section
    assert_equal(LibXML::XML::Node::TEXT_NODE, reader.node_type)
  end

  def test_encoding
    # ISO_8859_1:
    # ö - f6 in hex, \366 in octal
    # ü - fc in hex, \374 in octal
    xml = "<?xml version=\"1.0\" encoding=\"iso-8859-1\"?><bands genre=\"metal\">\n  <m\366tley_cr\374e country=\"us\">An American heavy metal band formed in Los Angeles, California in 1981.</m\366tley_cr\374e>\n  <iron_maiden country=\"uk\">British heavy metal band formed in 1975.</iron_maiden>\n</bands>"

    reader = LibXML::XML::Reader.string(xml, :encoding => LibXML::XML::Encoding::ISO_8859_1)
    reader.read

    assert_equal(Encoding::ISO8859_1, reader.read_outer_xml.encoding)
    assert_equal(Encoding::ISO8859_1, reader.read_inner_xml.encoding)
    assert_equal(Encoding::ISO8859_1, reader.read_string.encoding)

    assert_equal("<bands genre=\"metal\">\n  <m\xC3\xB6tley_cr\xC3\xBCe country=\"us\">An American heavy metal band formed in Los Angeles, California in 1981.</m\xC3\xB6tley_cr\xC3\xBCe>\n  <iron_maiden country=\"uk\">British heavy metal band formed in 1975.</iron_maiden>\n</bands>".force_encoding(Encoding::ISO8859_1),
                 reader.read_outer_xml)
    assert_equal("\n  <m\xC3\xB6tley_cr\xC3\xBCe country=\"us\">An American heavy metal band formed in Los Angeles, California in 1981.</m\xC3\xB6tley_cr\xC3\xBCe>\n  <iron_maiden country=\"uk\">British heavy metal band formed in 1975.</iron_maiden>\n".force_encoding(Encoding::ISO8859_1),
                 reader.read_inner_xml)
    assert_equal("\n  An American heavy metal band formed in Los Angeles, California in 1981.\n  British heavy metal band formed in 1975.\n".force_encoding(Encoding::ISO8859_1),
                 reader.read_string)
  end

  def test_invalid_encoding
    # ISO_8859_1:
    # ö - f6 in hex, \366 in octal
    # ü - fc in hex, \374 in octal
    xml = "<bands genre=\"metal\">\n  <m\366tley_cr\374e country=\"us\">An American heavy metal band formed in Los Angeles, California in 1981.</m\366tley_cr\374e>\n  <iron_maiden country=\"uk\">British heavy metal band formed in 1975.</iron_maiden>\n</bands>"

    reader = LibXML::XML::Reader.string(xml)
    error = assert_raises(LibXML::XML::Error) do
      reader.read
    end

    assert_equal("Fatal error: Input is not proper UTF-8, indicate encoding !\nBytes: 0xF6 0x74 0x6C 0x65 at :2.",
                 error.to_s)

  end

  def test_file_encoding
    reader = LibXML::XML::Reader.file(XML_FILE)
    reader.read
    assert_equal(LibXML::XML::Encoding::UTF_8, reader.encoding)
    assert_equal(Encoding::UTF_8, reader.value.encoding)
  end

  def test_string_encoding
    # ISO_8859_1:
    # ö - f6 in hex, \366 in octal
    # ü - fc in hex, \374 in octal
    xml = "<bands genre=\"metal\">\n  <m\366tley_cr\374e country=\"us\">An American heavy metal band formed in Los Angeles, California in 1981.</m\366tley_cr\374e>\n  <iron_maiden country=\"uk\">British heavy metal band formed in 1975.</iron_maiden>\n</bands>"
    reader = LibXML::XML::Reader.string(xml, :encoding => LibXML::XML::Encoding::ISO_8859_1)
    reader.read

    encoding = windows? ? LibXML::XML::Encoding::ISO_8859_1 : LibXML::XML::Encoding::NONE
    assert_equal(reader.encoding, encoding)
  end
end
