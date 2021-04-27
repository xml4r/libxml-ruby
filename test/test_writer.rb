# encoding: UTF-8

require_relative './test_helper'
require 'stringio'

class TestWriter < Minitest::Test
  XSL_PREFIX = 'xsl'
  XSL_URI = 'http://www.w3.org/1999/XSL/Transform'

  def test_generic_failure
    writer = LibXML::XML::Writer.string
    writer.start_document
    assert(!writer.end_element)
    writer.end_document
  end

  def test_empty_doc
    writer = LibXML::XML::Writer.string
    document(writer)
    assert_equal(writer.result.strip!, '<?xml version="1.0"?>')

    writer = LibXML::XML::Writer.string
    document(writer, :encoding => LibXML::XML::Encoding::ISO_8859_1)
    assert_equal(writer.result.strip!, '<?xml version="1.0" encoding="ISO-8859-1"?>')

    writer = LibXML::XML::Writer.string
    document(writer, :standalone => 1)
    assert_equal(writer.result.strip!, '<?xml version="1.0" standalone="yes"?>')

    writer = LibXML::XML::Writer.string
    document(writer, :standalone => 1, :encoding => LibXML::XML::Encoding::ISO_8859_1, :foo => :bar)
    assert_equal(writer.result.strip!, '<?xml version="1.0" encoding="ISO-8859-1" standalone="yes"?>')
  end

  def test_file_encoding
    value = "François".encode(Encoding::UTF_8)

    File.open('test.xml', 'wb', encoding: 'UTF-8') do |file|
      writer = LibXML::XML::Writer::io(file)
      document(writer, encoding: LibXML::XML::Encoding::UTF_8) do
        writer.write_element('Name', value)
      end
    end
  end

  def test_io_encoding
    value = "François".encode(Encoding::UTF_8)
    expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<Name>François</Name>".encode(Encoding::UTF_8)

    io = StringIO.new
    writer = LibXML::XML::Writer::io(io)
    document(writer, encoding: LibXML::XML::Encoding::UTF_8) do
      writer.write_element('Name', value)
    end
    assert_equal(expected, io.string.strip)
  end

  def test_single_root
    writer = LibXML::XML::Writer.string
    document(writer) do
      element writer, 'root'
    end

    assert_equal(writer.result.strip!, "<?xml version=\"1.0\"?>\n<root/>")
  end

  def test_pi
    expected = "<?xml version=\"1.0\"?>\n<?php echo \"foo\";?>"

    writer = LibXML::XML::Writer.string
    document(writer) do
      assert(writer.start_pi('php'))
      assert(writer.write_string('echo "foo";'))
      assert(writer.end_pi)
    end
    assert_equal(writer.result.strip!, expected)

    writer = LibXML::XML::Writer.string
    document(writer) do
      assert(writer.write_pi('php', 'echo "foo";'))
    end
    assert_equal(writer.result.strip!, expected)
  end

  def test_comment
    expected = "<?xml version=\"1.0\"?>\n<!--foo-->"

    writer = LibXML::XML::Writer.string
    document(writer) do
      assert(writer.start_comment)
      assert(writer.write_string 'foo')
      assert(writer.end_comment)
    end
    assert_equal(writer.result.strip!, expected)

    writer = LibXML::XML::Writer.string
    document(writer) do
      assert(writer.write_comment 'foo')
    end
    assert_equal(writer.result.strip!, expected)
  end

  def test_cdata
    expected = "<?xml version=\"1.0\"?>\n<root><![CDATA[<?php echo $foo; ?>]]></root>"

    writer = LibXML::XML::Writer.string
    document(writer) do
      element writer, 'root' do
        assert(writer.start_cdata)
        assert(writer.write_string '<?php echo $foo; ?>')
        assert(writer.end_cdata)
      end
    end
    assert_equal(writer.result.strip!, expected)

    writer = LibXML::XML::Writer.string
    document(writer) do
      element writer, 'root' do
        assert(writer.write_cdata '<?php echo $foo; ?>')
      end
    end
    assert_equal(writer.result.strip!, expected)
  end

  def test_write_empty_elements
    writer = LibXML::XML::Writer.string
    document(writer) do
      assert(writer.write_element 'foo')
    end
    assert_equal(writer.result.strip!, "<?xml version=\"1.0\"?>\n<foo/>")

    writer = LibXML::XML::Writer.string
    document(writer) do
      assert(writer.write_element_ns XSL_PREFIX, 'stylesheet', XSL_URI)
    end
    assert_equal(writer.result.strip!, "<?xml version=\"1.0\"?>\n<" + XSL_PREFIX + ":stylesheet xmlns:xsl=\"" + XSL_URI + "\"/>")
  end

  def test_valued_element
    expected = "<?xml version=\"1.0\"?>\n<abc>123<def>456<ghi>789</ghi>cueillir des cerises</def>nous irons au bois</abc>"

    writer = LibXML::XML::Writer.string
    document(writer) do
      assert(writer.start_element 'abc')
        assert(writer.write_string '123')
        assert(writer.start_element 'def')
          assert(writer.write_string '456')
          assert(writer.start_element 'ghi')
            assert(writer.write_string '789')
          assert(writer.end_element)
          assert(writer.write_string 'cueillir des cerises')
        assert(writer.end_element)
        assert(writer.write_string 'nous irons au bois')
      assert(writer.end_element)
    end
    assert_equal(writer.result.strip!, expected)

    writer = LibXML::XML::Writer.string
    document(writer) do
      assert(writer.start_element 'abc')
        assert(writer.write_string '123')
        assert(writer.start_element 'def')
          assert(writer.write_string '456')
          assert(writer.write_element 'ghi', '789')
          assert(writer.write_string 'cueillir des cerises')
        assert(writer.end_element)
        assert(writer.write_string 'nous irons au bois')
      assert(writer.end_element)
    end
    assert_equal(writer.result.strip!, expected)
  end

  def test_valued_element_ns
    expected = "<?xml version=\"1.0\"?>\n" +
      "<xsl:stylesheet xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\">" +
      "<xsl:attribute-set>" +
      "<xsl:attribute>20px</xsl:attribute>" +
      "<xsl:attribute>solid</xsl:attribute>" +
      "</xsl:attribute-set>" +
      "</xsl:stylesheet>"

    writer = LibXML::XML::Writer.string
    document(writer) do
      assert(writer.start_element_ns XSL_PREFIX, 'stylesheet', XSL_URI)
        assert(writer.start_element_ns XSL_PREFIX, 'attribute-set')
          assert(writer.start_element_ns XSL_PREFIX, 'attribute')
            assert(writer.write_string '20px')
          assert(writer.end_element)
          assert(writer.start_element_ns XSL_PREFIX, 'attribute')
            assert(writer.write_string 'solid')
          assert(writer.end_element)
        assert(writer.end_element)
      assert(writer.end_element)
    end
    assert_equal(writer.result.strip!, expected)

    writer = LibXML::XML::Writer.string
    document(writer) do
      assert(writer.start_element_ns XSL_PREFIX, 'stylesheet', XSL_URI)
        assert(writer.start_element_ns XSL_PREFIX, 'attribute-set')
          assert(writer.write_element_ns XSL_PREFIX, 'attribute', nil, '20px')
          assert(writer.write_element_ns XSL_PREFIX, 'attribute', nil, 'solid')
        assert(writer.end_element)
      assert(writer.end_element)
    end
    assert_equal(writer.result.strip!, expected)
  end

  def test_attribute
    writer = LibXML::XML::Writer.string
    document(writer) do
      element writer, 'root' do
        element writer, 'child' do
          assert(writer.start_attribute 'foo')
          assert(writer.write_string 'bar')
          assert(writer.end_attribute)
        end
      end
    end
    assert_equal(writer.result.strip!, "<?xml version=\"1.0\"?>\n<root><child foo=\"bar\"/></root>")

    writer = LibXML::XML::Writer.string
    document(writer) do
      element writer, 'root' do
        element writer, 'child' do
          assert(writer.write_attribute 'abc', 'def')
          assert(writer.write_string 'ghi') # must be done after attributes
        end
      end
    end
    assert_equal(writer.result.strip!, "<?xml version=\"1.0\"?>\n<root><child abc=\"def\">ghi</child></root>")
  end

  def test_attribute_ns
    expected = "<?xml version=\"1.0\"?>\n<root><link xlink:href=\"abc\" xhtml:class=\"def\"/></root>"

    writer = LibXML::XML::Writer.string
    document(writer) do
      element writer, 'root' do
        element writer, 'link' do
          assert(writer.write_attribute_ns 'xlink', 'href', nil, 'abc')
          assert(writer.write_attribute_ns 'xhtml', 'class', nil, 'def')
        end
      end
    end
    assert_equal(writer.result.strip!, expected)

    writer = LibXML::XML::Writer.string
    document(writer) do
      element writer, 'root' do
        element writer, 'link' do
          assert(writer.start_attribute_ns 'xlink', 'href')
            assert(writer.write_string 'abc')
          assert(writer.end_attribute)
          assert(writer.start_attribute_ns 'xhtml', 'class')
            assert(writer.write_string 'def')
          assert(writer.end_attribute)
        end
      end
    end
    assert_equal(writer.result.strip!, expected)
  end

  def test_quote_char
    if LibXML::XML::Writer.method_defined? :set_quote_char
      writer = LibXML::XML::Writer.string
      writer.set_quote_char "'"
      document(writer) do
        element writer, 'root' do
          assert(writer.start_attribute 'abc')
          assert(writer.write_string 'def')
          assert(writer.end_attribute)
        end
      end
      assert(writer.result.strip!.end_with? "<root abc='def'/>")
    end
  end

  def test_indentation_on
    if LibXML::XML::Writer.method_defined? :set_indent
      writer = LibXML::XML::Writer.string
      assert(writer.set_indent true)
      document(writer) do
        element writer, 'root' do
          element writer, 'child' do
            assert(writer.start_attribute 'abc')
            assert(writer.write_string 'def')
            assert(writer.end_attribute)
          end
        end
      end
      assert_equal(writer.result.strip!, "<?xml version=\"1.0\"?>\n<root>\n <child abc=\"def\"/>\n</root>")
    end
  end

  def test_indentation_string
    if LibXML::XML::Writer.method_defined? :set_indent_string
      writer = LibXML::XML::Writer.string
      assert(writer.set_indent true)
      assert(writer.set_indent_string ' ' * 4)
      document(writer) do
        element writer, 'root' do
          element writer, 'child' do
            assert(writer.start_attribute 'abc')
            assert(writer.write_string 'def')
            assert(writer.end_attribute)
          end
        end
      end
      assert_equal(writer.result.strip!, "<?xml version=\"1.0\"?>\n<root>\n    <child abc=\"def\"/>\n</root>")
    end
  end

  def test_dtd_declaration
    writer = LibXML::XML::Writer.string
    dtd writer, 'html'
    assert_equal(writer.result, '<!DOCTYPE html>')

    writer = LibXML::XML::Writer.string
    dtd writer, 'html', '-//W3C//DTD XHTML 1.0 Strict//EN', 'http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd'
    assert_equal(writer.result, '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">')
  end

  def test_dtd_attlist
    expected = '<!DOCTYPE http [<!ATTLIST method (get|post) "get">]>'

    writer = LibXML::XML::Writer.string
    dtd writer, 'http' do
      assert(writer.start_dtd_attlist 'method')
        assert(writer.write_string '(get|post) "get"')
      assert(writer.end_dtd_attlist)
    end
    assert_equal(writer.result, expected)

    writer = LibXML::XML::Writer.string
    dtd writer, 'http' do
      assert(writer.write_dtd_attlist 'method', '(get|post) "get"')
    end
    assert_equal(writer.result, expected)
  end

  def test_dtd_element
    expected = '<!DOCTYPE html [<!ELEMENT dl (dt|dd)+>]>'

    writer = LibXML::XML::Writer.string
    dtd writer, 'html' do
      assert(writer.start_dtd_element 'dl')
        assert(writer.write_string '(dt|dd)+')
      assert(writer.end_dtd_element)
    end
    assert_equal(writer.result, expected)

    writer = LibXML::XML::Writer.string
    dtd writer, 'html' do
      assert(writer.write_dtd_element 'dl', '(dt|dd)+')
    end
    assert_equal(writer.result, expected)
  end

  def test_dtd_entity
    # parameterized entity
    expected = '<!DOCTYPE html [<!ENTITY % special.pre "br | span | bdo | map"><!ENTITY % special "%special.pre; | object | img">]>'

    writer = LibXML::XML::Writer.string
    dtd writer, 'html' do
      assert(writer.start_dtd_entity 'special.pre', true)
        assert(writer.write_string 'br | span | bdo | map')
      assert(writer.end_dtd_entity)
      assert(writer.start_dtd_entity 'special', true)
        assert(writer.write_string '%special.pre; | object | img')
      assert(writer.end_dtd_entity)
    end
    assert_equal(writer.result, expected)

    writer = LibXML::XML::Writer.string
    dtd writer, 'html' do
      assert(writer.write_dtd_internal_entity 'special.pre', 'br | span | bdo | map', true)
      assert(writer.write_dtd_internal_entity 'special', '%special.pre; | object | img', true)
    end
    assert_equal(writer.result, expected)

    # non parameterized entity
    expected = '<!DOCTYPE html [<!ENTITY Alpha "&#913;">]>'

    writer = LibXML::XML::Writer.string
    dtd writer, 'html' do
      assert(writer.start_dtd_entity 'Alpha')
        assert(writer.write_string '&#913;')
      assert(writer.end_dtd_entity)
    end
    assert_equal(writer.result, expected)

    writer = LibXML::XML::Writer.string
    dtd writer, 'html' do
      assert(writer.start_dtd_entity 'Alpha', false)
        assert(writer.write_string '&#913;')
      assert(writer.end_dtd_entity)
    end
    assert_equal(writer.result, expected)

    writer = LibXML::XML::Writer.string
    dtd writer, 'html' do
      assert(writer.write_dtd_internal_entity 'Alpha', '&#913;', false)
    end
    assert_equal(writer.result, expected)
  end

  def test_dtd_notation
    writer = LibXML::XML::Writer.string
    dtd writer, 'pictures' do
      assert(writer.write_dtd_notation 'GIF89a', '-//Compuserve//NOTATION Graphics Interchange Format 89a//EN', nil)
      assert(writer.write_dtd_external_entity 'pictures', nil, 'images/plage.gif', 'GIF89a', false)
    end
    assert_equal(writer.result, '<!DOCTYPE pictures [<!NOTATION GIF89a PUBLIC "-//Compuserve//NOTATION Graphics Interchange Format 89a//EN"><!ENTITY pictures SYSTEM "images/plage.gif" NDATA GIF89a>]>')
  end

  def test_encoding
    iso = 'éloïse'.encode 'ISO-8859-1'

    writer = LibXML::XML::Writer.string
    document(writer) do
      assert(writer.write_element iso)
    end
    assert_equal(writer.result.strip!, "<?xml version=\"1.0\"?>\n<éloïse/>")
  end

  def test_flush
    writer = LibXML::XML::Writer.string
    assert(writer.start_document)
    assert_equal(writer.flush.strip!, '<?xml version="1.0"?>')
    assert(writer.start_element 'foo')
    assert(writer.end_element)
    assert(writer.end_document)
    writer.flush false
    assert_equal(writer.result.strip, '<foo/>')
  end

  def test_nil_pe_issue
    expected = '<!DOCTYPE html [<!ENTITY special.pre "br | span | bdo | map"><!ENTITY special "%special.pre; | object | img">]>'

    writer = LibXML::XML::Writer.string
    dtd writer, 'html' do
      assert(writer.write_dtd_internal_entity 'special.pre', 'br | span | bdo | map', nil)
      assert(writer.write_dtd_internal_entity 'special', '%special.pre; | object | img', nil)
    end
    assert_equal(writer.result, expected)
  end

  private

  def document(writer, options = {})
    assert(writer.start_document options)
    yield if block_given?
    assert(writer.end_document)
  end

  def dtd(writer, name, pubid = nil, sysid = nil)
    assert(writer.start_dtd name, pubid, sysid)
    yield if block_given?
    assert(writer.end_dtd)
  end

  def element(writer, localname)
    assert(writer.start_element localname)
    yield if block_given?
    assert(writer.end_element)
  end
end
