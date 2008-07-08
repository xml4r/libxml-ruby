require "libxml"
require 'test/unit'

class TC_XML_Node4 < Test::Unit::TestCase
  def setup()
    xp = XML::Parser.new()
    @str = '<ruby_array uga="booga" foo="bar"><fixnum>one</fixnum><fixnum>two</fixnum></ruby_array>'
    assert_equal(@str, xp.string = @str)
    doc = xp.parse
    xp2=XML::Parser.new()
    xp2.string=@str
    @doc2 = xp2.parse
    assert_instance_of(XML::Document, doc)
    assert_instance_of(XML::Node, doc.root)
    @root = doc.root
    assert_instance_of(XML::Node, @root)
  end

  def teardown()
    @root = nil
  end

  def test_01_xml_node_remove!()
    first = @root.child

    assert_equal 'fixnum', first.name
    assert_equal 'fixnum', first.next.name

    first.next.remove!

    assert_equal 'fixnum', first.name
    assert_equal nil, first.next
  end

  def test_02_xml_node_remove!()
    first = @root.child

    assert_equal 'fixnum', first.name
    assert_equal 'fixnum', first.next.name
    assert_equal 'one', first.content

    first.remove!

    first = @root.child
    assert_equal 'fixnum', first.name
    assert_equal nil, first.next
    assert_equal 'two', first.content
  end

  # TODO need to test namespace support
  def test_xml_node_find_first()
    first = @root.find_first('//fixnum')

    assert_equal 'one', first.content
  end
end
