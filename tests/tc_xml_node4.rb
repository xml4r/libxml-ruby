# $Id$
require "libxml_test"
require 'test/unit'

class TC_XML_Node4 < Test::Unit::TestCase
  def setup()
    xp = XML::Parser.new()
    str = '<ruby_array uga="booga" foo="bar"><fixnum>one</fixnum><fixnum>two</fixnum></ruby_array>'
    assert_equal(str, xp.string = str)
    doc = xp.parse
    assert_instance_of(XML::Document, doc)
    assert_instance_of(XML::Node, doc.root)
    @root = doc.root
    assert_instance_of(XML::Node, @root)
  end

  def teardown()
    @root = nil
  end
  
  def test_xml_node_eql?()
    first1 = @root.child
    first2 = @root.child
    
    assert_not_equal first1.object_id, first2.object_id
    assert first1.eql?(first2)
    assert first2.eql?(first1)
    
    assert_equal first1, first2
  end

  def test_xml_node_hash()
    first1 = @root.child
    first2 = @root.child
    
    assert_not_equal first1.object_id, first2.object_id
    assert_equal first1.hash, first2.hash
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
  
  def test_xml_node_property_set_remove()
    assert_equal 'booga', @root['uga']
    @root['uga'] = nil
    assert_equal nil , @root['uga']
    assert_equal "<ruby_array foo=\"bar\">\n  <fixnum>one</fixnum>\n  <fixnum>two</fixnum>\n</ruby_array>",
        @root.to_s   
  end
  
  
  
end
