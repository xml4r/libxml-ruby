# encoding: UTF-8

require_relative './test_helper'

class CDataCommentTest < Minitest::Test
  def setup
    xp = LibXML::XML::Parser.string('<root></root>')
    @doc = xp.parse
    assert_instance_of(LibXML::XML::Document, @doc)
    @root = @doc.root
  end

  def test_node_type
    cnode = LibXML::XML::Node.new_cdata('test cdata')
    assert_equal(LibXML::XML::Node::CDATA_SECTION_NODE, cnode.node_type)
  end

  def test_add_cdata
    @root << LibXML::XML::Node.new_cdata('mycdata')
    assert_equal '<root><![CDATA[mycdata]]></root>',
      @root.to_s.gsub(/\n\s*/,'')
  end

  def test_add_cdata_2
    @root << LibXML::XML::Node.new_cdata('mycdata')
    assert_equal 'cdata',
    @root.child.node_type_name
  end

  def test_add_cdata_3
    @root << el = LibXML::XML::Node.new_cdata('mycdata')
    el << "_this_is_added"
    assert_equal '<root><![CDATA[mycdata_this_is_added]]></root>',
      @root.to_s.gsub(/\n\s*/,'')
  end
  
  def test_attributes
    cnode = LibXML::XML::Node.new_cdata('test cdata')
    assert_equal(0, cnode.attributes.length)
  end
  
  def test_set_cdata_attribute
    cnode = LibXML::XML::Node.new_cdata('test cdata')
    
    # Can't create attributes on non-element nodes
    assert_raises(ArgumentError) do
      cnode['attr'] = '123'
    end
  end
end
