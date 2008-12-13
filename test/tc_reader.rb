require 'xml'
require 'test/unit'

class TestReader < Test::Unit::TestCase
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
    expected = [XML::Reader::TYPE_PROCESSING_INSTRUCTION,
                XML::Reader::TYPE_ELEMENT,
                XML::Reader::TYPE_SIGNIFICANT_WHITESPACE,
                XML::Reader::TYPE_COMMENT,
                XML::Reader::TYPE_SIGNIFICANT_WHITESPACE,
                XML::Reader::TYPE_ELEMENT,
                XML::Reader::TYPE_SIGNIFICANT_WHITESPACE,
                XML::Reader::TYPE_ELEMENT,
                XML::Reader::TYPE_CDATA,
                XML::Reader::TYPE_END_ELEMENT,
                XML::Reader::TYPE_SIGNIFICANT_WHITESPACE,
                XML::Reader::TYPE_ELEMENT,
                XML::Reader::TYPE_SIGNIFICANT_WHITESPACE,
                XML::Reader::TYPE_ELEMENT,
                XML::Reader::TYPE_SIGNIFICANT_WHITESPACE,
                XML::Reader::TYPE_ELEMENT,
                XML::Reader::TYPE_TEXT,
                XML::Reader::TYPE_END_ELEMENT,
                XML::Reader::TYPE_SIGNIFICANT_WHITESPACE,
                XML::Reader::TYPE_END_ELEMENT,
                XML::Reader::TYPE_SIGNIFICANT_WHITESPACE,
                XML::Reader::TYPE_END_ELEMENT,
                XML::Reader::TYPE_SIGNIFICANT_WHITESPACE,
                XML::Reader::TYPE_END_ELEMENT,
                XML::Reader::TYPE_SIGNIFICANT_WHITESPACE,
                XML::Reader::TYPE_END_ELEMENT]

    assert_equal(expected, node_types)
  end

  def test_file

    reader = XML::Reader.file(XML_FILE)
    verify_simple(reader)
  end

  def test_invalid_file
    assert_raise(XML::Error) do
      XML::Reader.file('/does/not/exist')
    end
  end

  def test_string
    reader = XML::Reader.string(File.read(XML_FILE))
    verify_simple(reader)
  end

  def test_io
    File.open(XML_FILE, 'rb') do |io|
      reader = XML::Reader.io(io)
      verify_simple(reader)
    end
  end

  def test_string_io
    data = File.read(XML_FILE)
    string_io = StringIO.new(data)
    reader = XML::Reader.io(string_io)
    verify_simple(reader)
  end

  def test_new_walker
    reader = XML::Reader.walker(XML::Document.file(XML_FILE))
    verify_simple(reader)
  end

  def test_error
    reader = XML::Reader.new('<foo blah')

    error = assert_raise(XML::Error) do
      reader.read
    end
    assert_equal("Fatal error: Couldn't find end of Start Tag foo at :1.", error.to_s)
  end

  def test_deprecated_error_handler
    called = false
    reader = XML::Reader.new('<foo blah')
    reader.set_error_handler do |error|
      called = true
    end

    assert_raise(XML::Error) do
      reader.read
    end

    assert(called)
  end

  def test_deprecated_reset_error_handler
    called = false
    reader = XML::Reader.new('<foo blah')
    reader.set_error_handler do |error|
      called = true
    end
    reader.reset_error_handler

    assert_raise(XML::Error) do
      reader.read
    end
    
    assert(!called)
  end

  def test_attr
    parser = XML::Reader.new("<foo x='1' y='2'/>")
    assert(parser.read)
    assert_equal('foo', parser.name)
    assert_equal('1', parser['x'])
    assert_equal('1', parser[0])
    assert_equal('2', parser['y'])
    assert_equal('2', parser[1])
    assert_equal(nil, parser['z'])
    assert_equal(nil, parser[2])
  end

  def test_value
    parser = XML::Reader.new("<foo><bar>1</bar><bar>2</bar><bar>3</bar></foo>")
    assert(parser.read)
    assert_equal('foo', parser.name)
    assert_equal(nil, parser.value)
    3.times do |i|
      assert(parser.read)
      assert_equal(XML::Reader::TYPE_ELEMENT, parser.node_type)
      assert_equal('bar', parser.name)
      assert(parser.read)
      assert_equal(XML::Reader::TYPE_TEXT, parser.node_type)
      assert_equal((i + 1).to_s, parser.value)
      assert(parser.read)
      assert_equal(XML::Reader::TYPE_END_ELEMENT, parser.node_type)
    end
  end

  def test_expand
    reader = XML::Reader.file(XML_FILE)
    reader.read
    node = reader.expand
    doc = node.doc
    reader.close
    GC.start

    doc.standalone?
  end

  def test_mode
    reader = XML::Reader.string('<xml/>')
    assert_equal(XML::Reader::MODE_INITIAL, reader.read_state)
    reader.read
    assert_equal(XML::Reader::MODE_EOF, reader.read_state)
  end

  def test_bytes_consumed
    reader = XML::Reader.file(XML_FILE)
    reader.read
    assert_equal(428, reader.byte_consumed)
  end

  def test_node
    XML.default_line_numbers = true
    reader = XML::Reader.file(XML_FILE)
    reader.read
    assert_instance_of(XML::Node, reader.node)
  end
end