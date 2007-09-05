# $Id$
require "libxml_test"
require 'test/unit'

class TC_XML_Document_Write < Test::Unit::TestCase
  def setup()
    @doc = XML::Document.new('1.0')
    assert_instance_of(XML::Document, @doc)
  end

  def teardown()
    @doc = nil
  end

  def test_ruby_xml_document_write_initialize()
    assert_instance_of(XML::Document, XML::Document.new())
    assert_equal('1.0', @doc.version)
  end # def test_ruby_xml_document_initialize()

  def test_ruby_xml_document_write_initialize2()
    doc = XML::Document.new('6.9')
    assert_instance_of(XML::Document, doc)
    assert_equal('6.9', doc.version)
  end # def test_ruby_xml_document_initialize2()
end # TC_XML_Document_Write
