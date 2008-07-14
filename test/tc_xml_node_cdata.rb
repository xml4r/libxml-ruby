require "libxml"
require 'test/unit'

class CDataCommentTest < Test::Unit::TestCase
  def setup()
    xp = LibXML::Parser.new()
    str = '<root></root>'
    assert_equal(str, xp.string = str)
    @doc = xp.parse
    assert_instance_of(LibXML::Document, @doc)
    @root = @doc.root
  end

  def test_libxml_node_add_cdata_01
    @root << LibXML::Node.new_cdata('mycdata')
    assert_equal '<root><![CDATA[mycdata]]></root>',
      @root.to_s.gsub(/\n\s*/,'')
  end

  def test_libxml_node_add_cdata_02
    @root << LibXML::Node.new_cdata('mycdata')
    assert_equal 'cdata',
    @root.child.node_type_name
  end

  def test_libxml_node_add_cdata_03
    @root << el = LibXML::Node.new_cdata('mycdata')
    el << "_this_is_added"
    assert_equal '<root><![CDATA[mycdata_this_is_added]]></root>',
      @root.to_s.gsub(/\n\s*/,'')
  end
end
