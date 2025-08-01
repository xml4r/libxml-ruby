# encoding: UTF-8

require_relative './test_helper'


class TestNS < Minitest::Test
  def setup
    file = File.join(File.dirname(__FILE__), 'model/soap.xml')
    @doc = LibXML::XML::Document.file(file)
  end

  def teardown
    @doc = nil
  end

  def test_create_ns
    node = LibXML::XML::Node.new('foo')
    ns = LibXML::XML::Namespace.new(node, 'my_namepace', 'http://www.mynamespace.com')
    assert_equal(ns.prefix, 'my_namepace')
    assert_equal(ns.href, 'http://www.mynamespace.com')
  end

  def test_create_default_ns
    node = LibXML::XML::Node.new('foo')
    ns = LibXML::XML::Namespace.new(node, nil, 'http://www.mynamespace.com')
    assert_nil(ns.prefix)
    assert_equal(ns.href, 'http://www.mynamespace.com')
  end

  def test_create_unbound_ns
    error = assert_raises(TypeError) do
      LibXML::XML::Namespace.new(nil, 'my_namepace', 'http://www.mynamespace.com')
    end
    assert_equal('wrong argument type nil (expected Data)', error.to_s)
  end

  def test_duplicate_ns
    node = LibXML::XML::Node.new('foo')
    LibXML::XML::Namespace.new(node, 'myname', 'http://www.mynamespace.com')
    LibXML::XML::Namespace.new(node, 'myname', 'http://www.mynamespace.com')
  end

  def test_eql
    node = LibXML::XML::Node.new('Envelope')

    assert(node.namespaces.namespace.eql?(node.namespaces.namespace))
  end

  def test_equal
    node1 = LibXML::XML::Node.new('Envelope')
    ns1 = LibXML::XML::Namespace.new(node1, 'soap', 'http://schemas.xmlsoap.org/soap/envelope/')

    node2 = LibXML::XML::Node.new('Envelope')
    ns2 = LibXML::XML::Namespace.new(node2, 'soap', 'http://schemas.xmlsoap.org/soap/envelope/')

    assert(ns1 == ns2)
  end
end
