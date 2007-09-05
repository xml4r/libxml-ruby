# $Id$
require 'test/unit'
require "libxml_test"

class TC_XML_Document < Test::Unit::TestCase
  def setup()
    xp = XML::Parser.new()
    assert_instance_of(XML::Parser, xp)
    str = '<ruby_array uga="booga" foo="bar"><fixnum>one</fixnum><fixnum>two</fixnum></ruby_array>'
    assert_equal(str, xp.string = str)
    @doc = xp.parse
    assert_instance_of(XML::Document, @doc)
  end

  def teardown()
    @doc = nil
  end

  def test_libxml_document_find()
    set = @doc.find('/ruby_array/fixnum')
    assert_instance_of(XML::Node::Set, set)
    xpt = set.xpath
    assert_instance_of(XML::XPath, xpt)
  end

  def test_ruby_xml_document_compression()
    if XML::Parser::enabled_zlib?
      0.upto(9) do |i|
	assert_equal(i, @doc.compression = i)
	assert_equal(i, @doc.compression)
      end

      9.downto(0) do |i|
	assert_equal(i, @doc.compression = i)
	assert_equal(i, @doc.compression)
      end

      10.upto(20) do |i|
	# assert_equal(9, @doc.compression = i)
	assert_equal(i, @doc.compression = i) # This works around a bug in Ruby 1.8
	assert_equal(9, @doc.compression)
      end

      -1.downto(-10) do |i|
	# assert_equal(0, @doc.compression = i)
	assert_equal(i, @doc.compression = i) # FIXME This bug should get fixed ASAP
	assert_equal(0, @doc.compression)
      end
    end # if ...enabled_zlib?
  end # test_ruby_xml_document_compression()
end # TC_XML_Document
