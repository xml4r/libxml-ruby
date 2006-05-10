# $Id$
require "libxml_test"
require 'test/unit'

class TC_XML_Node5 < Test::Unit::TestCase
  def setup()
    xp = XML::Parser.new()
    str = '<test><num>one</num><num>two</num><num>three</num></test>'
    assert_equal(str, xp.string = str)
    @doc = xp.parse
    assert_instance_of(XML::Document, @doc)
    @root = @doc.root
    @num1 = @root.child
    @num2 = @num1.next
    @num3 = @num2.next
  end
  
  def test_libxml_node_add_next_01
    @num1.next = XML::Node.new('num', 'one-and-a-half')
    assert_equal '<test><num>one</num><num>one-and-a-half</num><num>two</num><num>three</num></test>',
      @root.to_s.gsub(/\n\s*/,'')
  end
  
  def test_libxml_node_add_next_02
    @num2.next = XML::Node.new('num', 'two-and-a-half')
    assert_equal '<test><num>one</num><num>two</num><num>two-and-a-half</num><num>three</num></test>',
      @root.to_s.gsub(/\n\s*/,'')
  end
  
  def test_libxml_node_add_next_03
    @num3.next = XML::Node.new('num', 'four')
    assert_equal '<test><num>one</num><num>two</num><num>three</num><num>four</num></test>',
      @root.to_s.gsub(/\n\s*/,'')
  end
  
  def test_libxml_node_add_prev_01
    @num1.prev = XML::Node.new('num', 'half')
    assert_equal '<test><num>half</num><num>one</num><num>two</num><num>three</num></test>',
      @root.to_s.gsub(/\n\s*/,'')
  end
  
  def test_libxml_node_add_prev_02
    @num2.prev = XML::Node.new('num', 'one-and-a-half')
    assert_equal '<test><num>one</num><num>one-and-a-half</num><num>two</num><num>three</num></test>',
      @root.to_s.gsub(/\n\s*/,'')
  end
  
  def test_libxml_node_add_prev_03
    @num3.prev = XML::Node.new('num', 'two-and-a-half')
    assert_equal '<test><num>one</num><num>two</num><num>two-and-a-half</num><num>three</num></test>',
      @root.to_s.gsub(/\n\s*/,'')
  end  
end
