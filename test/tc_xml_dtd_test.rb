require "libxml"
require 'test/unit'

class TestDtd < Test::Unit::TestCase
  def setup
    xp = XML::Parser.string(<<-EOS)
      <root>
        <head a="ee" id="1">Colorado</head>
        <descr>Lots of nice mountains</descr>
      </root>
    EOS
    @doc = xp.parse
  end
  
  def teardown
    @doc = nil
  end
  
  def dtd
    XML::Dtd.new(<<-EOS)
      <!ELEMENT root (head, descr)>
      <!ELEMENT head (#PCDATA)>
      <!ATTLIST head
        id NMTOKEN #REQUIRED
        a CDATA #IMPLIED
      >
      <!ELEMENT descr (#PCDATA)>
    EOS
  end
  
  def test_valid
    assert(@doc.validate(dtd))
  end
  
  def test_invalid
    new_node = XML::Node.new('invalid', 'this will mess up validation')
    @doc.root.child_add(new_node)
    
    messages = Hash.new
    assert(!@doc.validate(dtd) do |message, error|
      messages[message] = error
    end)

    expected = {"No declaration for element invalid\n" => true,
                "Element root content does not follow the DTD, expecting (head , descr), got (head descr invalid)\n" => true}
      
    assert_equal(expected, messages)
  end
end