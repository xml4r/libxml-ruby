# $Id$
require "libxml_test"
require 'test/unit'

class TC_XML_Parser_Context < Test::Unit::TestCase
  def setup()
    str = '<ruby_array uga="booga" foo="bar"><fixnum>one</fixnum><fixnum>two</fixnum></ruby_array>'
    xp = XML::Parser.string(str)
    assert_equal(str, xp.string = str)
    doc = xp.parse
    assert_instance_of(XML::Document, doc)
    @ctxt = xp.parser_context()
    assert_instance_of(XML::Parser::Context, @ctxt)
  end

  def teardown()
    @ctxt = nil
  end

  def test_libxml_parser_context_well_formed()
    if @ctxt.well_formed?
      assert_instance_of(TrueClass, @ctxt.well_formed?)
    else
      assert_instance_of(FalseClass, @ctxt.well_formed?)
    end
  end

  def test_ruby_xml_parser_context_version_info()
    assert_instance_of(String, @ctxt.version)
  end

  def test_ruby_xml_parser_context_depth()
    assert_instance_of(Fixnum, @ctxt.depth)
  end

  def test_ruby_xml_parser_context_doc()
    assert_instance_of(XML::Document, @ctxt.doc)
  end

  def test_ruby_xml_parser_context_disable_sax()
    assert(!@ctxt.disable_sax?)
  end

  def test_ruby_xml_parser_context_docbook()
    assert(!@ctxt.docbook?)
  end

  def test_ruby_xml_parser_context_encoding()
    assert(!@ctxt.encoding)
  end

  def test_ruby_xml_parser_context_html()
    assert(!@ctxt.html?)
  end

  def test_ruby_xml_parser_context_keep_blanks()
    if @ctxt.keep_blanks?
      assert_instance_of(TrueClass, @ctxt.keep_blanks?)
    else
      assert_instance_of(FalseClass, @ctxt.keep_blanks?)
    end
  end

  if ENV['NOTWORKING']
    def test_ruby_xml_parser_context_num_chars()
      assert_equal(17, @ctxt.num_chars)
    end
  end

  def test_ruby_xml_parser_context_replace_entities()
    if @ctxt.replace_entities?
      assert_instance_of(TrueClass, @ctxt.replace_entities?)
    else
      assert_instance_of(FalseClass, @ctxt.replace_entities?)
    end
  end

  def test_ruby_xml_parser_context_space_depth()
    assert_equal(1, @ctxt.space_depth)
  end

  def test_ruby_xml_parser_context_subset_external()
    assert(!@ctxt.subset_external?)
  end

  def test_ruby_xml_parser_context_data_directory_get()
    # assert_instance_of(String, @ctxt.data_directory)
  end
end # TC_XML_Parser_Context
