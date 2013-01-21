# encoding: UTF-8

require './test_helper'
require 'stringio'
require 'test/unit'

class TestWriter < Test::Unit::TestCase

    XSL_PREFIX = 'xsl'
    XSL_URI = 'http://www.w3.org/1999/XSL/Transform'

    def test_generic_failure
        writer = XML::Writer.string
        writer.start_document
        assert(!writer.end_element)
        writer.end_document
    end

    def test_empty_doc
        writer = XML::Writer.string
        document writer
        assert_equal(writer.result.strip!, '<?xml version="1.0"?>')

        writer = XML::Writer.string
        document writer, { :encoding => XML::Encoding::ISO_8859_1 }
        assert_equal(writer.result.strip!, '<?xml version="1.0" encoding="ISO-8859-1"?>')

        writer = XML::Writer.string
        document writer, { :standalone => 1 }
        assert_equal(writer.result.strip!, '<?xml version="1.0" standalone="yes"?>')

        writer = XML::Writer.string
        document writer, { :standalone => 1, :encoding => XML::Encoding::ISO_8859_1, :foo => :bar }
        assert_equal(writer.result.strip!, '<?xml version="1.0" encoding="ISO-8859-1" standalone="yes"?>')
    end

    def test_single_root
        writer = XML::Writer.string
        document writer do
            element writer, 'root'
        end

        assert_equal(writer.result.strip!, "<?xml version=\"1.0\"?>\n<root/>")
    end

    def test_pi
        expected = "<?xml version=\"1.0\"?>\n<?php echo \"foo\";?>"

        writer = XML::Writer.string
        document writer do
            assert(writer.start_pi('php'))
            assert(writer.write_string('echo "foo";'))
            assert(writer.end_pi)
        end
        assert_equal(writer.result.strip!, expected)

        writer = XML::Writer.string
        document writer do
            assert(writer.write_pi('php', 'echo "foo";'))
        end
        assert_equal(writer.result.strip!, expected)
    end

    def test_comment
        expected = "<?xml version=\"1.0\"?>\n<!--foo-->"

        writer = XML::Writer.string
        document writer do
            assert(writer.start_comment)
            assert(writer.write_string 'foo')
            assert(writer.end_comment)
        end
        assert_equal(writer.result.strip!, expected)

        writer = XML::Writer.string
        document writer do
            assert(writer.write_comment 'foo')
        end
        assert_equal(writer.result.strip!, expected)
    end

    def test_cdata
        expected = "<?xml version=\"1.0\"?>\n<root><![CDATA[<?php echo $foo; ?>]]></root>"

        writer = XML::Writer.string
        document writer do
            element writer, 'root' do
                assert(writer.start_cdata)
                assert(writer.write_string '<?php echo $foo; ?>')
                assert(writer.end_cdata)
            end
        end
        assert_equal(writer.result.strip!, expected)

        writer = XML::Writer.string
        document writer do
            element writer, 'root' do
                assert(writer.write_cdata '<?php echo $foo; ?>')
            end
        end
        assert_equal(writer.result.strip!, expected)
    end

    def test_write_empty_elements
        writer = XML::Writer.string
        document writer do
            assert(writer.write_element 'foo')
        end
        assert_equal(writer.result.strip!, "<?xml version=\"1.0\"?>\n<foo/>")

        writer = XML::Writer.string
        document writer do
            assert(writer.write_element_ns XSL_PREFIX, 'stylesheet', XSL_URI)
        end
        assert_equal(writer.result.strip!, "<?xml version=\"1.0\"?>\n<" + XSL_PREFIX + ":stylesheet xmlns:xsl=\"" + XSL_URI + "\"/>")
    end

    def test_valued_element
        expected = "<?xml version=\"1.0\"?>\n<abc>123<def>456<ghi>789</ghi>cueillir des cerises</def>nous irons au bois</abc>"

        writer = XML::Writer.string
        document writer do
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

        writer = XML::Writer.string
        document writer do
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

        writer = XML::Writer.string
        document writer do
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

        writer = XML::Writer.string
        document writer do
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
        writer = XML::Writer.string
        document writer do
            element writer, 'root' do
                element writer, 'child' do
                    assert(writer.start_attribute 'foo')
                    assert(writer.write_string 'bar')
                    assert(writer.end_attribute)
                end
            end
        end
        assert_equal(writer.result.strip!, "<?xml version=\"1.0\"?>\n<root><child foo=\"bar\"/></root>")

        writer = XML::Writer.string
        document writer do
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

        writer = XML::Writer.string
        document writer do
            element writer, 'root' do
                element writer, 'link' do
                    assert(writer.write_attribute_ns 'xlink', 'href', nil, 'abc')
                    assert(writer.write_attribute_ns 'xhtml', 'class', nil, 'def')
                end
            end
        end
        assert_equal(writer.result.strip!, expected)

        writer = XML::Writer.string
        document writer do
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
        if XML::Writer.method_defined? :set_quote_char
            writer = XML::Writer.string
            writer.set_quote_char "'"
            document writer do
                element writer, 'root' do
                    assert(writer.start_attribute 'abc')
                    assert(writer.write_string 'def')
                    assert(writer.end_attribute)
                end
            end
            assert_equal(writer.result.strip!, "<?xml version=\"1.0\"?>\n<root><child abc='def'>ghi</child></root>")
        end
    end

    def test_indentation_on
        if XML::Writer.method_defined? :set_indent
            writer = XML::Writer.string
            assert(writer.set_indent true)
            document writer do
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
        if XML::Writer.method_defined? :set_indent_string
            writer = XML::Writer.string
            assert(writer.set_indent true)
            assert(writer.set_indent_string ' ' * 4)
            document writer do
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
        writer = XML::Writer.string
        writer.start_dtd 'html'
        writer.end_dtd
        writer.flush
        assert_equal(writer.result, '<!DOCTYPE html>')

        writer = XML::Writer.string
        writer.start_dtd 'html', '-//W3C//DTD XHTML 1.0 Strict//EN', 'http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd'
        writer.end_dtd
        writer.flush
        assert_equal(writer.result, '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">')
    end

private

    def document(writer, options = {})
        assert(writer.start_document options)
        yield if block_given?
        assert(writer.end_document)
    end

    def element(writer, localname)
        assert(writer.start_element localname)
        yield if block_given?
        assert(writer.end_element)
    end

end
