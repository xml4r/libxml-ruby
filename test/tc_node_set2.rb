require "xml"
require 'test/unit'

class TC_XML_Node_Set2 < Test::Unit::TestCase
  def setup()
    xp = XML::Parser.new()
    str = '<ruby_array uga="booga" foo="bar"><fixnum>one</fixnum><fixnum>two</fixnum></ruby_array>'
    assert_equal(str, xp.string = str)
    doc = xp.parse
    assert_instance_of(XML::Document, doc)
    @one = doc.root.child
    @two = @one.next
    @set = doc.find('/ruby_array/fixnum').set
    @emptyset = doc.find('/fixnum').set
    assert_instance_of(XML::Node::Set, @set)
  end

  def teardown()
    @set, @emptyset, @one, @two = [nil] * 4
  end

  def test_libxml_nodeset_to_a()
    assert_equal [@one, @two], @set.to_a
    assert_equal [], @emptyset.to_a
  end

  def test_libxml_nodeset_empty?()
    assert ! @set.empty?
    assert @emptyset.empty?
  end

  def test_libxml_nodeset_first()
    assert_equal @one.to_s, @set.first.to_s
    assert_equal nil, @emptyset.first
  end

end # TC_XML_Document
