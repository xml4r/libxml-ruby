require "xml"
require 'test/unit'

class TestNS < Test::Unit::TestCase
  def test_ns
    node = XML::Node.new('foo')
    ns = XML::NS.new(node, 'my_namepace', 'http://www.mynamespace.com')
    assert_equal(ns.prefix, 'my_namepace')
    assert_equal(ns.href, 'http://www.mynamespace.com')
  end
  
  def test_default_ns
    node = XML::Node.new('foo')
    ns = XML::NS.new(node, nil, 'http://www.mynamespace.com')
    assert_equal(ns.prefix, nil)
    assert_equal(ns.href, 'http://www.mynamespace.com')
  end
end