# encoding: UTF-8

require 'xml/libxml'

class TestDeprecatedRequire < Minitest::Test
  def test_basic
    xp = LibXML::XML::Parser.string('<ruby_array uga="booga" foo="bar"><fixnum>one</fixnum><fixnum>two</fixnum></ruby_array>')
    assert_instance_of(LibXML::XML::Parser, xp)
    @doc = xp.parse
    assert_instance_of(LibXML::XML::Document, @doc)
  end
end
