# encoding: UTF-8

require File.expand_path('../test_helper', __FILE__)
require 'stringio'

class TestXml < Minitest::Test
  # -----  Constants  ------
  def test_lib_versions
    assert(XML.check_lib_versions)
  end

  def test_debug_entities
    original = XML.debug_entities

    XML.debug_entities = false
    refute(XML.debug_entities)

    XML.debug_entities = true
    assert(XML.debug_entities)

    XML.debug_entities = false
    refute(XML.debug_entities)

    XML.debug_entities = original
  end

  def test_default_compression
    return unless XML.default_compression

    original = XML.default_compression

    0.upto(9) do |i|
      XML.default_compression = i
      assert_equal(i, XML.default_compression)
    end

    9.downto(0) do |i|
      assert_equal(i, XML.default_compression = i)
      assert_equal(i, XML.default_compression)
    end

    0.downto(-10) do |i|
      assert_equal(i, XML.default_compression = i)
      assert_equal(0, XML.default_compression)
    end

    10.upto(20) do |i|
      assert_equal(i, XML.default_compression = i)
      assert_equal(9, XML.default_compression)
    end

    XML.default_compression = original
  end

  def test_default_keep_blanks
    original = XML.default_keep_blanks

    XML.default_keep_blanks = false
    refute(XML.default_keep_blanks)
    assert_equal(XML::Parser::Options::NOBLANKS, XML.default_options)

    XML.default_keep_blanks = true
    assert(XML.default_keep_blanks)
    assert_equal(0, XML.default_options)

    XML.default_keep_blanks = original
  end

  def test_default_line_numbers
    original = XML.default_line_numbers

    XML.default_line_numbers = false
    refute(XML.default_line_numbers)

    XML.default_line_numbers = true
    assert(XML.default_line_numbers)

    XML.default_line_numbers = false
    refute(XML.default_line_numbers)

    XML.default_line_numbers = original
  end

  def test_default_substitute_entities
    original = XML.default_substitute_entities

    XML.default_substitute_entities = false
    refute(XML.default_substitute_entities)
    assert_equal(0, XML.default_options)

    XML.default_substitute_entities = true
    assert(XML.default_substitute_entities)
    assert_equal(XML::Parser::Options::NOENT, XML.default_options)

    XML.default_substitute_entities = false
    refute(XML.default_substitute_entities)

    XML.default_substitute_entities = original
  end

  def test_default_tree_indent_string
    original = XML.default_tree_indent_string

    s = XML.default_tree_indent_string
    assert_instance_of(String, s)
    assert_equal('  ', s)
    XML.default_tree_indent_string = 'uga'
    s = XML.default_tree_indent_string
    assert_instance_of(String, s)
    assert_equal('uga', s)
    XML.default_tree_indent_string = '  '
    s = XML.default_tree_indent_string
    assert_instance_of(String, s)
    assert_equal('  ', s)

    XML.default_tree_indent_string = original
  end

  def test_default_validity_checking
    original = XML.default_validity_checking

    XML.default_validity_checking = false
    refute(XML.default_validity_checking)
    assert_equal(0, XML.default_options)

    XML.default_validity_checking = true
    assert(XML.default_validity_checking)
    assert_equal(XML::Parser::Options::DTDVALID, XML.default_options)

    XML.default_validity_checking = false
    refute(XML.default_validity_checking)

    XML.default_validity_checking = original
  end

  def test_default_warnings
    original = XML.default_warnings

    XML.default_warnings = false
    refute(XML.default_warnings)
    assert_equal(XML::Parser::Options::NOWARNING, XML.default_options)

    XML.default_warnings = true
    assert(XML.default_warnings)
    assert_equal(0, XML.default_options)

    XML.default_warnings = false
    refute(XML.default_warnings)

    XML.default_warnings = original
  end

  def test_enabled_automata
    assert(XML.enabled_automata?)
  end

  def test_enabled_c14n
    assert(XML.enabled_c14n?)
  end

  def test_enabled_catalog
    assert(XML.enabled_catalog?)
  end

  def test_enabled_debug
    assert(XML.enabled_debug?)
  end

  def test_enabled_docbook
    assert(XML.enabled_docbook?)
  end

  def test_enabled_ftp
    assert(XML.enabled_ftp?)
  end

  def test_enabled_http
    assert(XML.enabled_http?)
  end

  def test_enabled_html
    assert(XML.enabled_html?)
  end

  def test_enabled_iconv
    assert(XML.enabled_iconv?)
  end

  def test_enabled_memory_debug
    assert_equal(false, XML.enabled_memory_debug?)
  end

  def test_enabled_regexp
    assert(XML.enabled_regexp?)
  end

  def test_enabled_schemas
    assert(XML.enabled_schemas?)
  end

  def test_enabled_thread
    assert(XML.enabled_thread?)
  end

  def test_enabled_unicode
    assert(XML.enabled_unicode?)
  end

  def test_enabled_xinclude
    assert(XML.enabled_xinclude?)
  end

  def test_enabled_xpath
    assert(XML.enabled_xpath?)
  end

  def test_enabled_xpointer
    assert(XML.enabled_xpointer?)
  end

  def test_enabled_zlib
    assert(XML.enabled_zlib?.is_a?(TrueClass) || XML.enabled_zlib?.is_a?(FalseClass))
  end

  def test_intent_tree_output
    assert(TrueClass, XML.indent_tree_output)

    XML.indent_tree_output = false
    assert(FalseClass, XML.indent_tree_output)

    XML.indent_tree_output = true
    assert(TrueClass, XML.indent_tree_output)
  end

  def test_version
    assert_instance_of(String, XML::VERSION)
  end

  def test_vernum
    assert_instance_of(Integer, XML::VERNUM)
  end

  def test_libxml_parser_features
    assert_instance_of(Array, XML.features)
  end

  def test_default_options
    assert_equal(0, XML.default_options)
  end
end