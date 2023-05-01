# encoding: UTF-8

require_relative './test_helper'
require 'stringio'

class TestError < Minitest::Test
  def test_error_codes
    assert_equal(4, LibXML::XML::Error::DTD)
    assert_equal(4, LibXML::XML::Error.const_get('DTD'))

    assert_equal(4, LibXML::XML::Error::DOCUMENT_EMPTY)
    assert_equal(4, LibXML::XML::Error.const_get('DOCUMENT_EMPTY'))
  end

  def test_invalid_handler
    assert_raises(RuntimeError) do
      LibXML::XML::Error.set_handler
    end
  end

  def test_handler
    exception = nil
    LibXML::XML::Error.set_handler do |error|
      exception = error
    end

    # Raise the error
    error = assert_raises(LibXML::XML::Error) do
      LibXML::XML::Reader.string('<foo').read
    end
    assert_equal(exception, error)

    # Check the handler worked
    refute_nil(exception)
    assert_kind_of(LibXML::XML::Error, exception)
    assert_equal("Fatal error: Couldn't find end of Start Tag foo at :1.", exception.message)
    assert_equal(LibXML::XML::Error::PARSER, exception.domain)
    assert_equal(LibXML::XML::Error::GT_REQUIRED, exception.code)
    assert_equal(LibXML::XML::Error::FATAL, exception.level)
    assert_nil(exception.file)
    assert_equal(1, exception.line)
    assert_equal('foo', exception.str1)
    assert_nil(exception.str2)
    assert_nil(exception.str3)
    assert_equal(0, exception.int1)
    assert_equal(5, exception.int2)
    assert_nil(exception.node)
  end

  def test_reset_handler
    exception = nil
    LibXML::XML::Error.set_handler do |error|
      exception = error
    end

    LibXML::XML::Error.reset_handler
    LibXML::XML::Reader.string('<foo')
    assert_nil(exception)
  end

  def test_get_handler
    assert_respond_to(LibXML::XML::Error, :get_handler)
    assert_equal(0, LibXML::XML::Error.method(:get_handler).arity)

    saved_handler = LibXML::XML::Error.get_handler
    LibXML::XML::Error.set_handler { puts "New handler" }
    refute_equal(LibXML::XML::Error.get_handler, saved_handler)

    if saved_handler
      LibXML::XML::Error.set_handler &saved_handler
    else
      LibXML::XML::Error.reset_handler
    end

    assert_equal(LibXML::XML::Error.get_handler, saved_handler)
  end

  def test_verbose_handler
    LibXML::XML::Error.set_handler(&LibXML::XML::Error::VERBOSE_HANDLER)
    output = StringIO.new
    original_stderr = Object::STDERR

    Object.const_set(:STDERR, output)
    begin
      assert_raises(LibXML::XML::Error) do
        LibXML::XML::Parser.string('<foo><bar/></foz>').parse
      end
    ensure
      Object.const_set(:STDERR, original_stderr)
    end
    assert_equal("Fatal error: Opening and ending tag mismatch: foo line 1 and foz at :1.\n", output.string)
  end

  def test_no_handler
    LibXML::XML::Error.reset_handler
    output = StringIO.new
    original_stderr = Object::STDERR

    Object.const_set(:STDERR, output)
    begin
      assert_raises(LibXML::XML::Error) do
        LibXML::XML::Parser.string('<foo><bar/></foz>').parse
      end
    ensure
      Object.const_set(:STDERR, original_stderr)
    end
    assert_equal('', output.string)
  end

  def test_parse_error
    exception = assert_raises(LibXML::XML::Error) do
      LibXML::XML::Parser.string('<foo><bar/></foz>').parse
    end

    assert_instance_of(LibXML::XML::Error, exception)
    assert_equal("Fatal error: Opening and ending tag mismatch: foo line 1 and foz at :1.", exception.message)
    assert_equal(LibXML::XML::Error::PARSER, exception.domain)
    assert_equal(LibXML::XML::Error::TAG_NAME_MISMATCH, exception.code)
    assert_equal(LibXML::XML::Error::FATAL, exception.level)
    assert_nil(exception.file)
    assert_equal(1, exception.line)
  end

  def test_xpath_error
    doc = LibXML::XML::Document.file(File.join(File.dirname(__FILE__), 'model/soap.xml'))

    exception = assert_raises(LibXML::XML::Error) do
      doc.find('/foo[bar=test')
    end

    assert_instance_of(LibXML::XML::Error, exception)
    assert_equal("Error: Invalid predicate.", exception.message)
    assert_equal(LibXML::XML::Error::XPATH, exception.domain)
    assert_equal(LibXML::XML::Error::XPATH_INVALID_PREDICATE_ERROR, exception.code)
    assert_equal(LibXML::XML::Error::ERROR, exception.level)
    assert_nil(exception.file)
    assert_nil(nil)
  end

  def test_double_parse
    LibXML::XML::Error.set_handler {|msg| nil }
    parser = LibXML::XML::Parser.string("<test>something</test>")
    parser.parse

    error = assert_raises(LibXML::XML::Error) do
      # Try parsing a second time
      parser.parse
    end

    assert_equal(" LibXML::XML::Error.", error.to_s)
  end

  def test_double_parse_register_handler
    LibXML::XML::Parser.register_error_handler(lambda {|msg| nil })
    parser = LibXML::XML::Parser.string("<test>something</test>")
    parser.parse

    error = assert_raises(LibXML::XML::Error) do
      # Try parsing a second time
      parser.parse
    end

    assert_equal(" LibXML::XML::Error.", error.to_s)
  end

  def test_libxml_parser_empty_string
    error = assert_raises(TypeError) do
      LibXML::XML::Parser.string(nil)
    end
    assert_equal('wrong argument type nil (expected String)', error.to_s)

    error = assert_raises(ArgumentError) do
      LibXML::XML::Parser.string("")
    end
    assert_equal('Must specify a string with one or more characters', error.to_s)
  end

  def test_error_domain_to_s
    exception = assert_raises(LibXML::XML::Error) do
      LibXML::XML::Parser.string('<foo href="http://example.org/cgi?k1=v1&k2=v2"></foo>').parse
    end

    assert_equal(LibXML::XML::Error::PARSER, exception.domain)
    assert_equal("PARSER", exception.domain_to_s)
  end

  def test_error_code_to_s
    exception = assert_raises(LibXML::XML::Error) do
      LibXML::XML::Parser.string('<foo href="http://example.org/cgi?k1=v1&k2=v2"></foo>').parse
    end
    assert_equal(LibXML::XML::Error::ENTITYREF_SEMICOL_MISSING, exception.code)
    assert_equal("ENTITYREF_SEMICOL_MISSING", exception.code_to_s)
  end
end
