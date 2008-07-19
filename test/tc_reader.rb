require 'xml'
require 'test/unit'

class TC_XML_Reader < Test::Unit::TestCase

  SIMPLE_XML = File.join(File.dirname(__FILE__), 'model/simple.xml')

  def test_new_file
    reader = XML::Reader.file(SIMPLE_XML)
    verify_simple(reader)
    assert_raises(RuntimeError) { XML::Reader.file('/does/not/exist') }
  end

  def test_new_data
    reader = XML::Reader.new(File.read(SIMPLE_XML))
    verify_simple(reader)
  end

  def test_new_walker
    reader = XML::Reader.walker(XML::Document.file(SIMPLE_XML))
    verify_simple(reader)
  end

  def test_reader_error
    reader = XML::Reader.new('<foo/>')
    assert_raises(RuntimeError) { reader.set_error_handler }
    called = false
    reader.set_error_handler { |*a| called = true }
    while reader.read > 0; end
    assert(!called)
    reader = XML::Reader.new('<foo blah')
    reader.set_error_handler do |*a|
      assert_equal(5, a.size)
      assert_equal(reader, a[0])
      assert_equal(XML::Reader::SEVERITY_ERROR, a[2])
      assert_nil(a[3])
      assert_equal(1, a[4])
      called = true
    end
    while reader.read > 0; end
    assert(called)
  end

  def test_attr
    parser = XML::Reader.new("<foo x='1' y='2'/>")
    assert_equal(1, parser.read)
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
    assert_equal(1, parser.read)
    assert_equal('foo', parser.name)
    assert_equal(nil, parser.value)
    3.times do |i|
      assert_equal(1, parser.read)
      assert_equal(XML::Reader::TYPE_ELEMENT, parser.node_type)
      assert_equal('bar', parser.name)
      assert_equal(1, parser.read)
      assert_equal(XML::Reader::TYPE_TEXT, parser.node_type)
      assert_equal((i + 1).to_s, parser.value)
      assert_equal(1, parser.read)
      assert_equal(XML::Reader::TYPE_END_ELEMENT, parser.node_type)
    end
  end

  def test_expand
    reader = XML::Reader.file(SIMPLE_XML)
    reader.read
    node = reader.expand
    doc = node.doc
    reader.close
    GC.start
    
    doc.standalone?
  end

  def verify_simple(reader)
    node_types = []
    19.times do
      assert_equal(1, reader.read)
      node_types << reader.node_type
    end
    assert_equal(0, reader.read)
    assert_equal(node_types,
      [XML::Reader::TYPE_ELEMENT,
       XML::Reader::TYPE_SIGNIFICANT_WHITESPACE,
       XML::Reader::TYPE_ELEMENT,
       XML::Reader::TYPE_TEXT,
       XML::Reader::TYPE_END_ELEMENT,
       XML::Reader::TYPE_SIGNIFICANT_WHITESPACE,
       XML::Reader::TYPE_ELEMENT,
       XML::Reader::TYPE_SIGNIFICANT_WHITESPACE,
       XML::Reader::TYPE_ELEMENT,
       XML::Reader::TYPE_TEXT,
       XML::Reader::TYPE_END_ELEMENT,
       XML::Reader::TYPE_SIGNIFICANT_WHITESPACE,
       XML::Reader::TYPE_ELEMENT,
       XML::Reader::TYPE_TEXT,
       XML::Reader::TYPE_END_ELEMENT,
       XML::Reader::TYPE_SIGNIFICANT_WHITESPACE,
       XML::Reader::TYPE_END_ELEMENT,
       XML::Reader::TYPE_SIGNIFICANT_WHITESPACE,
       XML::Reader::TYPE_END_ELEMENT])
  end
end
