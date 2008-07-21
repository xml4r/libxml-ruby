require "xml"
require 'test/unit'

class TestRelaxNG < Test::Unit::TestCase
  def setup
    xp = XML::Parser.new
    @doc = XML::Document.file(File.join(File.dirname(__FILE__), 'model/shiporder.xml'))
  end
  
  def teardown
    @doc = nil
  end
  
  def relaxng
    document = XML::Document.file(File.join(File.dirname(__FILE__), 'model/shiporder.rng'))
    relaxng = XML::RelaxNG.document(document)
  end

  def test_from_doc
    assert_instance_of(XML::RelaxNG, relaxng)
  end
  
  def test_valid
    assert(@doc.validate_relaxng(relaxng))
  end
  
  def test_invalid
    new_node = XML::Node.new('invalid', 'this will mess up validation')
    @doc.root.child_add(new_node)
    
    messages = Hash.new
    assert(!@doc.validate_relaxng(relaxng) do |message, error|
      messages[message] = error
    end)

    expected = {"Did not expect element invalid there\n" => true}
    assert_equal(expected, messages)
  end
end
