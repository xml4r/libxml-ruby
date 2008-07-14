require "libxml"
require 'test/unit'

class TC_XML_Node_Set < Test::Unit::TestCase
  def setup()
    xp = LibXML::Parser.new()
    str = '<ruby_array uga="booga" foo="bar"><fixnum>one</fixnum><fixnum>two</fixnum></ruby_array>'
    assert_equal(str, xp.string = str)
    doc = xp.parse
    assert_instance_of(LibXML::Document, doc)
    @set = doc.find('/ruby_array/fixnum').set
    assert_instance_of(LibXML::Node::Set, @set)
  end

  def teardown()
    @set = nil
  end

  def test_libxml_nodeset_each()
    @set.each do |n|
      assert_instance_of(LibXML::Node, n)
    end
  end
end # TC_XML_Document
