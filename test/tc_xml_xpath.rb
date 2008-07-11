require "libxml"
require "tempfile"
require "test/unit"

class TextXPath < Test::Unit::TestCase
  def setup()
    @doc = XML::Document.file('model/soap.xml')
  end
  
  def teardown()
    @doc = nil
  end
  
  def test_basic
    nodes = @doc.find('/soap:Envelope')
    assert_instance_of(XML::XPath::Object, nodes)
    assert_equal(1, nodes.length)
    assert_equal(nodes.xpath_type, XML::XPath::NODESET)
    assert_instance_of(XML::Node::Set, nodes.set)
    # Why doesn't this work?
    # assert_equal('/soap:Envelope', nodes.string)
  end    

  def test_ns
    nodes = @doc.find('//ns1:IdAndName', 'ns1:http://domain.somewhere.com')
    assert_equal(3, nodes.length)
  end
  
  def test_ns_array
    nodes = @doc.find('//ns1:IdAndName', ['ns1:http://domain.somewhere.com'])
    assert_equal(3, nodes.length)
  end

  def test_default_ns
    # Pull all nodes with http://services.somewhere.com namespace
    nodes = @doc.find('//*[namespace-uri()="http://services.somewhere.com"]')
    assert_equal(2, nodes.length)
    assert_equal('getManufacturerNamesResponse', nodes[0].name)
    assert_equal('IDAndNameList', nodes[1].name)
    
    # Pull all nodes with http://services.somewhere.com namespace
    nodes = @doc.find('//ns:*', 'ns:http://services.somewhere.com')
    assert_equal(2, nodes.length)
    assert_equal('getManufacturerNamesResponse', nodes[0].name)
    assert_equal('IDAndNameList', nodes[1].name)
    
    # Get getManufacturerNamesResponse node
    nodes = @doc.find('//ns:getManufacturerNamesResponse', 'ns:http://services.somewhere.com')
    assert_equal(1, nodes.length)
    
    # Get IdAndName node
    nodes = @doc.find('/soap:Envelope/soap:Body/ns0:getManufacturerNamesResponse/ns0:IDAndNameList/ns1:IdAndName', 
                      ['ns0:http://services.somewhere.com', 'ns1:http://domain.somewhere.com'])
    assert_equal(3, nodes.length)
  end
 
  def test_attribute_ns
    # Pull all nodes with http://services.somewhere.com namespace
    nodes = @doc.find('@soap:encodingStyle')
    assert_equal(1, nodes.length)
    assert_equal('encodingStyle', nodes.first.name)
    assert_equal('http://www.w3.org/2001/12/soap-encoding', nodes.first.value)
  end
  
  #def test_custom_function
    #xml = Tempfile.new("xxx")
    #xml.puts("<a/>")
    #xml.close

    #doc = XML::Document.file(xml.path)
    #assert_nil(doc.find("//*[name(.)=normalize_space(' a ')]"))
  #end
end
