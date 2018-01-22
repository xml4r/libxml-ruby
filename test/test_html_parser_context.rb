# encoding: UTF-8

require File.expand_path('../test_helper', __FILE__)


class TestHtmlParserContext < Minitest::Test
  def test_default_options
    context = XML::HTMLParser::Context.new
    assert_equal(0, context.options)
  end

  def test_no_options
    context = XML::HTMLParser::Context.new
    context.options = 0
    assert_equal(0, context.options)
  end

  def test_options
    context = XML::HTMLParser::Context.new
    context.options = XML::HTMLParser::Options::NOERROR
    assert_equal(XML::HTMLParser::Options::NOERROR, context.options)
  end
end