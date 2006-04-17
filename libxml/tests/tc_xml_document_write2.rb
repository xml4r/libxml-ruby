# $Id$
require "libxml_test"
require 'test/unit'

class TC_XML_Document_Write2 < Test::Unit::TestCase
  def setup()
    @doc = XML::Document.new('1.0')
    assert_instance_of(XML::Document, @doc)
  end

  def teardown()
    @doc = nil
  end

  def test_ruby_xml_document_write_root()
    xml = "<?xml version=\"1.0\"?>\n<rubynet/>\n"
    @doc.root = XML::Node.new('rubynet')
    assert_instance_of(XML::Node, @doc.root)
    assert_instance_of(XML::Document, @doc.root.doc)
    assert_equal(xml, @doc.to_s)
  end # def test_ruby_xml_document_write_root()

  def test_ruby_xml_document_write_root2()
    xml = "<?xml version=\"1.0\"?>\n<rubynet/>\n"
    @doc.root = XML::Node.new('rubynet')
    assert_instance_of(XML::Node, @doc.root)
    assert_instance_of(XML::Document, @doc.root.doc)
    assert_equal(xml, @doc.to_s(false))
  end # def test_ruby_xml_document_write_root()

  def test_ruby_xml_document_write_root3()
    xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<rubynet/>\n"
    @doc.root = XML::Node.new('rubynet')
    @doc.encoding = 'UTF-8'
    assert_instance_of(XML::Node, @doc.root)
    assert_instance_of(XML::Document, @doc.root.doc)
    assert_equal(xml, @doc.to_s)
  end # def test_ruby_xml_document_write_root()

  def test_ruby_xml_document_write_root4()
    xml = "<?xml version=\"1.0\"?>\n<rubynet/>\n"
    @doc.root = XML::Node.new('rubynet')
    assert_instance_of(XML::Node, @doc.root)
    assert_instance_of(XML::Document, @doc.root.doc)
    assert_equal(xml, @doc.to_s(false))
  end # def test_ruby_xml_document_write_root()

  def test_ruby_xml_document_write_root5()
    xml = "<?xml version=\"1.0\"?>\n<rubynet/>\n"
    @doc.root = XML::Node.new('rubynet')
    assert_instance_of(XML::Node, @doc.root)
    assert_instance_of(XML::Document, @doc.root.doc)
    assert_equal(xml, @doc.to_s(true))
  end # def test_ruby_xml_document_write_root()
end
