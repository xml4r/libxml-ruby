require 'libxml'
require 'test/unit'

class TC_XML_Node_text < Test::Unit::TestCase
  def setup
    @t=LibXML::Node.new_text("testdata")
    assert_instance_of(LibXML::Node,@t)
  end

  def test_content
    assert_equal("testdata",@t.content)
  end

  def test_type
    assert_equal(true,@t.text?)
  end
end
