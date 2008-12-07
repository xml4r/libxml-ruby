require "xml"
require 'test/unit'

class TestNamespaces < Test::Unit::TestCase
  def setup
    file = File.join(File.dirname(__FILE__), 'model/soap.xml')
    @doc = XML::Document.file(file)
  end

  def teardown
    @doc = nil
  end

  def test_namespace
    node = @doc.root
    ns = node.namespaces.namespace
    assert_equal('soap', ns.prefix)
    assert_equal('http://schemas.xmlsoap.org/soap/envelope/', ns.href)
  end

  def test_namespace_set
    node = @doc.root

    ns = node.namespaces.find do |namespace|
      namespace.prefix == 'xsi'
    end

    node.namespaces.namespace = ns
    ns = node.namespaces.namespace

    assert_equal('xsi', ns.prefix)
    assert_equal('http://www.w3.org/2001/XMLSchema-instance', ns.href)
  end

  def test_namespaces
    node = @doc.find_first('//ns1:IdAndName',
                           :ns1 => 'http://domain.somewhere.com')

    namespaces = node.namespaces.sort
    assert_equal(5, namespaces.length)

    namespace = namespaces[0]
    assert_instance_of(XML::Namespace, namespace)
    assert_equal(nil, namespace.prefix)
    assert_equal('http://services.somewhere.com', namespace.href)

    namespace = namespaces[1]
    assert_instance_of(XML::Namespace, namespace)
    assert_equal('ns1', namespace.prefix)
    assert_equal('http://domain.somewhere.com', namespace.href)

    namespace = namespaces[2]
    assert_instance_of(XML::Namespace, namespace)
    assert_equal('soap', namespace.prefix)
    assert_equal('http://schemas.xmlsoap.org/soap/envelope/', namespace.href)

    namespace = namespaces[3]
    assert_instance_of(XML::Namespace, namespace)
    assert_equal('xsd', namespace.prefix)
    assert_equal('http://www.w3.org/2001/XMLSchema', namespace.href)

    namespace = namespaces[4]
    assert_instance_of(XML::Namespace, namespace)
    assert_equal('xsi', namespace.prefix)
    assert_equal('http://www.w3.org/2001/XMLSchema-instance', namespace.href)
  end

  def test_namespaces
    node = @doc.find_first('//ns1:IdAndName',
                           :ns1 => 'http://domain.somewhere.com')

    namespaces = node.namespaces.each do |namespace|
      assert_instance_of(XML::Namespace, namespace)
    end
  end

  def test_namespace_definitions
    ns_defs = @doc.root.namespaces.definitions
    assert_equal(3, ns_defs.size)

    namespace = ns_defs[0]
    assert_instance_of(XML::Namespace, namespace)
    assert_equal('soap', namespace.prefix)
    assert_equal('http://schemas.xmlsoap.org/soap/envelope/', namespace.href)

    namespace = ns_defs[1]
    assert_instance_of(XML::Namespace, namespace)
    assert_equal('xsd', namespace.prefix)
    assert_equal('http://www.w3.org/2001/XMLSchema', namespace.href)

    namespace = ns_defs[2]
    assert_instance_of(XML::Namespace, namespace)
    assert_equal('xsi', namespace.prefix)
    assert_equal('http://www.w3.org/2001/XMLSchema-instance', namespace.href)

    node = @doc.root.find_first('//ns:getManufacturerNamesResponse',
                                :ns => 'http://services.somewhere.com')
    ns_defs = node.namespaces.definitions
    assert_equal(1, ns_defs.size)

    namespace = ns_defs[0]
    assert_instance_of(XML::Namespace, namespace)
    assert_equal(nil, namespace.prefix)
    assert_equal('http://services.somewhere.com', namespace.href)
  end

  def test_find_by_prefix
    node = @doc.find_first('//ns1:getManufacturerNamesResponse',
                           :ns1 => 'http://services.somewhere.com')

    namespace = @doc.root.namespaces.find_by_prefix('soap')

    assert_instance_of(XML::Namespace, namespace)
    assert_equal('soap', namespace.prefix)
    assert_equal('http://schemas.xmlsoap.org/soap/envelope/', namespace.href)
  end

  def test_find_default_ns
    namespace = @doc.root.namespaces.find_by_prefix(nil)
    assert_nil(namespace)

    node = @doc.find_first('//ns1:getManufacturerNamesResponse',
                           :ns1 => 'http://services.somewhere.com')
    namespace = node.namespaces.find_by_prefix(nil)

    assert_instance_of(XML::Namespace, namespace)
    assert_equal(nil, namespace.prefix)
    assert_equal('http://services.somewhere.com', namespace.href)
  end

  def test_find_ns_by_href
    node = @doc.find_first('//ns1:getManufacturerNamesResponse',
                           :ns1 => 'http://services.somewhere.com')

    namespace = node.namespaces.find_by_href('http://schemas.xmlsoap.org/soap/envelope/')

    assert_instance_of(XML::Namespace, namespace)
    assert_equal('soap', namespace.prefix)
    assert_equal('http://schemas.xmlsoap.org/soap/envelope/', namespace.href)
  end
end