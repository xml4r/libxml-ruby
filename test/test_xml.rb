# encoding: UTF-8

require_relative './test_helper'
require 'stringio'

class TestXml < Minitest::Test
  # -----  Constants  ------
  def test_lib_versions
    assert(LibXML::XML.check_lib_versions)
  end

  def test_default_compression
    return unless LibXML::XML.default_compression

    original = LibXML::XML.default_compression

    0.upto(9) do |i|
      LibXML::XML.default_compression = i
      assert_equal(i, LibXML::XML.default_compression)
    end

    9.downto(0) do |i|
      assert_equal(i, LibXML::XML.default_compression = i)
      assert_equal(i, LibXML::XML.default_compression)
    end

    0.downto(-10) do |i|
      assert_equal(i, LibXML::XML.default_compression = i)
      assert_equal(0, LibXML::XML.default_compression)
    end

    10.upto(20) do |i|
      assert_equal(i, LibXML::XML.default_compression = i)
      assert_equal(9, LibXML::XML.default_compression)
    end

    LibXML::XML.default_compression = original
  end

  def test_default_tree_indent_string
    original = LibXML::XML.default_tree_indent_string

    s = LibXML::XML.default_tree_indent_string
    assert_instance_of(String, s)
    assert_equal('  ', s)
    LibXML::XML.default_tree_indent_string = 'uga'
    s = LibXML::XML.default_tree_indent_string
    assert_instance_of(String, s)
    assert_equal('uga', s)
    LibXML::XML.default_tree_indent_string = '  '
    s = LibXML::XML.default_tree_indent_string
    assert_instance_of(String, s)
    assert_equal('  ', s)

    LibXML::XML.default_tree_indent_string = original
  end

  def test_enabled_automata
    assert(LibXML::XML.enabled_automata?)
  end

  def test_enabled_c14n
    assert(LibXML::XML.enabled_c14n?)
  end

  def test_enabled_catalog
    assert(LibXML::XML.enabled_catalog?)
  end

  def test_enabled_debug
    assert(LibXML::XML.enabled_debug?)
  end

  def test_enabled_docbook
    if windows?
      refute(LibXML::XML.enabled_docbook?)
    else
      assert(LibXML::XML.enabled_docbook?)
    end
  end

  def test_enabled_ftp
    if windows?
      refute(LibXML::XML.enabled_ftp?)
    else
      assert(LibXML::XML.enabled_ftp?)
    end
  end

  def test_enabled_http
    assert(LibXML::XML.enabled_http?)
  end

  def test_enabled_html
    assert(LibXML::XML.enabled_html?)
  end

  def test_enabled_iconv
    iconv_enabled = RUBY_PLATFORM !~ /darwin/
    assert_equal(iconv_enabled, LibXML::XML.enabled_iconv?)
  end

  def test_enabled_memory_debug
    assert_equal(false, LibXML::XML.enabled_memory_debug?)
  end

  def test_enabled_regexp
    assert(LibXML::XML.enabled_regexp?)
  end

  def test_enabled_schemas
    assert(LibXML::XML.enabled_schemas?)
  end

  def test_enabled_thread
    assert(LibXML::XML.enabled_thread?)
  end

  def test_enabled_unicode
    assert(LibXML::XML.enabled_unicode?)
  end

  def test_enabled_xinclude
    assert(LibXML::XML.enabled_xinclude?)
  end

  def test_enabled_xpath
    assert(LibXML::XML.enabled_xpath?)
  end

  def test_enabled_xpointer
    assert(LibXML::XML.enabled_xpointer?)
  end

  def test_enabled_zlib
    assert(LibXML::XML.enabled_zlib?.is_a?(TrueClass) || LibXML::XML.enabled_zlib?.is_a?(FalseClass))
  end

  def test_intent_tree_output
    assert(TrueClass, LibXML::XML.indent_tree_output)

    LibXML::XML.indent_tree_output = false
    assert(FalseClass, LibXML::XML.indent_tree_output)

    LibXML::XML.indent_tree_output = true
    assert(TrueClass, LibXML::XML.indent_tree_output)
  end

  def test_version
    assert_instance_of(String, LibXML::XML::VERSION)
  end

  def test_vernum
    assert_instance_of(Integer, LibXML::XML::VERNUM)
  end

  def test_default_save_no_empty_tags
    original = LibXML::XML.default_save_no_empty_tags

    LibXML::XML.default_save_no_empty_tags = false
    refute(LibXML::XML.default_save_no_empty_tags)

    LibXML::XML.default_save_no_empty_tags = true
    assert(LibXML::XML.default_save_no_empty_tags)

    LibXML::XML.default_save_no_empty_tags = original
  end
end
