require 'xml/libxml'
require 'test/unit'

class TestDeprecatedRequire < Test::Unit::TestCase
  def test_basic
    xp = LibXML::Parser.new
    assert_instance_of(LibXML::Parser, xp)
    str = '<ruby_array uga="booga" foo="bar"><fixnum>one</fixnum><fixnum>two</fixnum></ruby_array>'
    assert_equal(str, xp.string = str)
    @doc = xp.parse
    assert_instance_of(LibXML::Document, @doc)
  end
end
