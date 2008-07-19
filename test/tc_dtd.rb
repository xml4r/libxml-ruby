require "xml"
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
  
  def test_external_dtd
    xml = <<-EOS
      <!DOCTYPE test PUBLIC "-//TEST" "test.dtd" []>
      <test>
        <title>T1</title>
      </test>
    EOS
    
    messages = Array.new
    XML::Parser.register_error_handler(lambda { |msg| messages << msg })
    
    XML::Parser.default_load_external_dtd = false
    doc = XML::Parser.string(xml).parse
    assert_equal(Array.new, messages)
    
    XML::Parser.default_load_external_dtd = true
    doc = XML::Parser.string(xml).parse
    assert_equal('I/O warning : failed to load external entity "test.dtd" <!DOCTYPE test PUBLIC "-//TEST" "test.dtd" []> ^',
                 messages.map{|msg| msg.strip}.join(' ')) 
  end
end
