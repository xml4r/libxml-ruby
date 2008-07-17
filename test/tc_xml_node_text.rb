require 'xml'
require 'test/unit'

class TC_XML_Node_text < Test::Unit::TestCase
  def setup
    @t=XML::Node.new_text("testdata")
    assert_instance_of(XML::Node,@t)
  end

  def test_content
    assert_equal("testdata",@t.content)
  end

  def test_type
    assert_equal(true,@t.text?)
  end
end
