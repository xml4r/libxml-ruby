# encoding: UTF-8

require_relative './test_helper'


class TestHtmlParserContext < Minitest::Test
  def test_default_options
    context = LibXML::XML::HTMLParser::Context.new
    assert_equal(0, context.options)
  end

  def test_no_options
    context = LibXML::XML::HTMLParser::Context.new
    context.options = 0
    assert_equal(0, context.options)
  end

  def test_options
    context = LibXML::XML::HTMLParser::Context.new
    context.options = LibXML::XML::HTMLParser::Options::NOERROR
    assert_equal(LibXML::XML::HTMLParser::Options::NOERROR, context.options)
  end
end