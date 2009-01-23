require 'xml'
require 'test/unit'

class TestTextNode < Test::Unit::TestCase
  def test_content
    node = XML::Node.new_text('testdata')
    assert_instance_of(XML::Node, node)
    assert_equal('testdata', node.content)
  end

  def test_invalid_content
    error = assert_raise(TypeError) do
      node = XML::Node.new_text(nil)
    end
    assert_equal('wrong argument type nil (expected String)', error.to_s)
  end

	# Disabling output escaping can't work without this test passing.
	def test_text_node_name_sanity
		node = XML::Node.new_text('testdata')
    assert_equal XML::Node::XML_STRING_TEXT.object_id, node.name.object_id
    node.name = 'ignore-me'
    assert_equal XML::Node::XML_STRING_TEXT.object_id, node.name.object_id
    node.name = XML::Node::XML_STRING_TEXT_NOENC
    assert_equal XML::Node::XML_STRING_TEXT_NOENC.object_id, node.name.object_id
    node.name = 'ignore-me'
    assert_equal XML::Node::XML_STRING_TEXT_NOENC.object_id, node.name.object_id
  end

	def test_text_node_name_singletons_frozen
    assert XML::Node::XML_STRING_TEXT.frozen?
    assert XML::Node::XML_STRING_TEXT_NOENC.frozen?
  end

	# We use the same facility that libXSLT does here to disable output escaping.
	# This lets you specify that the node's content should be rendered unaltered
	# whenever it is being output.  This is useful for things like <script> and
	# <style> nodes in HTML documents if you don't want to be forced to wrap them
	# in CDATA nodes.  Or if you are sanitizing existing HTML documents and want
	# to preserve the content of any of the text nodes.
	#
	def test_output_escaping
    node = XML::Node.new_text('<my> "entity"')
		assert_equal '&lt;my&gt; "entity"', node.to_s
    node.name = XML::Node::XML_STRING_TEXT_NOENC
		assert_equal '<my> "entity"', node.to_s
    node.name = XML::Node::XML_STRING_TEXT
		assert_equal '&lt;my&gt; "entity"', node.to_s
  end

end
