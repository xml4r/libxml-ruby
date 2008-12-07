require "xml"
require 'test/unit'

class TestNS < Test::Unit::TestCase
  def setup
    file = File.join(File.dirname(__FILE__), 'model/soap.xml')
    @doc = XML::Document.file(file)
  end

  def teardown
    @doc = nil
  end

  def test_create_ns
    node = XML::Node.new('foo')
    ns = XML::Namespace.new(node, 'my_namepace', 'http://www.mynamespace.com')
    assert_equal(ns.prefix, 'my_namepace')
    assert_equal(ns.href, 'http://www.mynamespace.com')
  end

  def test_create_default_ns
    node = XML::Node.new('foo')
    ns = XML::Namespace.new(node, nil, 'http://www.mynamespace.com')
    assert_equal(ns.prefix, nil)
    assert_equal(ns.href, 'http://www.mynamespace.com')
  end

  def test_create_unbound_ns
    error = assert_raise(TypeError) do
      XML::Namespace.new(nil, 'my_namepace', 'http://www.mynamespace.com')
    end
    assert_equal('wrong argument type nil (expected Data)', error.to_s)
  end

  def test_duplicate_ns
    node = XML::Node.new('foo')
    XML::Namespace.new(node, 'myname', 'http://www.mynamespace.com')
    error = assert_raises(XML::Error) do
      XML::Namespace.new(node, 'myname', 'http://www.mynamespace.com')
    end
    assert_equal(' LibXML::XML::Error at :0.', error.to_s)
  end
end