require 'xml'
require "tempfile"
require "test/unit"

class TestXPath < Test::Unit::TestCase
  def setup
    @doc = XML::Document.file(File.join(File.dirname(__FILE__), 'model/soap.xml'))
  end
  
  def teardown
    @doc = nil
  end
  
  def test_doc_find
    nodes = @doc.find('/soap:Envelope')
    assert_instance_of(XML::XPath::Object, nodes)
    assert_equal(1, nodes.length)
    assert_equal(nodes.xpath_type, XML::XPath::NODESET)
    assert_instance_of(XML::Node::Set, nodes.set)
    assert_instance_of(XML::XPath::Context, nodes.context)
  end    

  def test_doc_find_first
    node = @doc.find_first('/soap:Envelope/soap:Body')
    assert_instance_of(XML::Node, node)
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

  def test_node_find
    nodes = @doc.find('//ns1:IdAndName', 'ns1:http://domain.somewhere.com')
    node = nodes.first

    # Since we are searching on the node, don't have to register namespace
    nodes = node.find('ns1:name')
    assert_equal(1, nodes.length)
    assert_equal('name', nodes.first.name)
    assert_equal('man1', nodes.first.content)
  end
    
  def test_node_find_first
    node = @doc.find_first('//ns1:IdAndName', 'ns1:http://domain.somewhere.com')
    
    # Since we are searching on the node, don't have to register namespace
    node = node.find_first('ns1:name')
    assert_equal('name', node.name)
    assert_equal('man1', node.content)
  end
  
  def test_node_no_doc
    node = XML::Node.new('header', 'some content')
    assert_raise(TypeError) do
      node = node.find_first('/header')
    end
  end

  def test_nodes_debug
   # nodes = @doc.find('//ns1:IdAndName', 'ns1:http://domain.somewhere.com')
    #nodes.debug
  end
  
  #def test_custom_function
    #xml = Tempfile.new("xxx")
    #xml.puts("<a/>")
    #xml.close

    #doc = XML::Document.file(xml.path)
    #assert_nil(doc.find("//*[name(.)=normalize_space(' a ')]"))
  #end
  
  #def test_memory
    ## This sometimes causes a segmentation fault because 
    ## an xml document is sometimes freed before the
    ## xpath_object used to query it.  When the xpath_object
    ## is free, it iterates over its results which are pointers
    ## to the document's nodes. A segmentation fault then happens.

    #100.times do 
      #doc = XML::Document.new('1.0')
      #doc.root = XML::Node.new("header")

      #1000.times do 
        #doc.root << XML::Node.new("footer")
      #end
  
      #nodes = doc.find('/header/footer')
      #nodes.length
      #nodes = nil
    #end
  #end
end
