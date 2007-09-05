# $Id$
require "libxml_test"
require 'test/unit'

class TC_XML_Parser < Test::Unit::TestCase
  def setup()
    @xp = XML::Parser.new()
    assert_not_nil(@xp)
  end

  def teardown()
    @xp = nil
  end

  def test_libxml_parser_string()
    str = '<ruby_array uga="booga" foo="bar"><fixnum>one</fixnum><fixnum>two</fixnum></ruby_array>'
    assert_equal(str, @xp.string = str)
    assert_instance_of(XML::Document, @xp.parse)
  end

  def test_libxml_parser_bad_xml()
    # @xp.string = '<ruby_array uga="booga" foo="bar"<fixnum>one</fixnum><fixnum>two</fixnum></ruby_array>'
    # assert_not_nil(@xp.parse)
  end

  def test_ruby_xml_parser_check_lib_versions()
    assert(XML::Parser::check_lib_versions)
  end

  def test_ruby_xml_parser_debug_entities()
    XML::Parser::debug_entities = false
    assert(!XML::Parser::debug_entities)

    XML::Parser::debug_entities = true
    assert(XML::Parser::debug_entities)

    XML::Parser::debug_entities = false
    assert(!XML::Parser::debug_entities)
  end


  def test_ruby_xml_parser_default_compression()
    0.upto(9) do |i|
      XML::Parser::default_compression = i
      assert_equal(i, XML::Parser::default_compression)
    end

    9.downto(0) do |i|
      assert_equal(i, XML::Parser::default_compression = i)
      assert_equal(i, XML::Parser::default_compression)
    end

    0.downto(-10) do |i|
      assert_equal(i, XML::Parser::default_compression = i)
      assert_equal(0, XML::Parser::default_compression)
    end

    10.upto(20) do |i|
      assert_equal(i, XML::Parser::default_compression = i)
      assert_equal(9, XML::Parser::default_compression)
    end
  end

  def test_ruby_xml_parser_default_keep_blanks()
    XML::Parser::default_keep_blanks = false
    assert(!XML::Parser::default_keep_blanks)

    XML::Parser::default_keep_blanks = true
    assert(XML::Parser::default_keep_blanks)

    XML::Parser::default_keep_blanks = false
    assert(!XML::Parser::default_keep_blanks)
    
    # other tests depend on keeping blanks by default,
    # which is the default default behaviour anyway.
    XML::Parser::default_keep_blanks = true    
  end

  def test_ruby_xml_parser_default_line_numbers()
    XML::Parser::default_line_numbers = false
    assert(!XML::Parser::default_line_numbers)

    XML::Parser::default_line_numbers = true
    assert(XML::Parser::default_line_numbers)

    XML::Parser::default_line_numbers = false
    assert(!XML::Parser::default_line_numbers)
  end

  def test_ruby_xml_parser_default_substitute_entities()
    XML::Parser::default_substitute_entities = false
    assert(!XML::Parser::default_substitute_entities)

    XML::Parser::default_substitute_entities = true
    assert(XML::Parser::default_substitute_entities)

    XML::Parser::default_substitute_entities = false
    assert(!XML::Parser::default_substitute_entities)
  end

  def test_ruby_xml_parser_default_tree_indent_string()
    s = XML::Parser::default_tree_indent_string()
    assert_instance_of(String, s)
    assert_equal('  ', s)
    XML::Parser::default_tree_indent_string = 'uga'
    s = XML::Parser::default_tree_indent_string()
    assert_instance_of(String, s)
    assert_equal('uga', s)
    XML::Parser::default_tree_indent_string = '  '
    s = XML::Parser::default_tree_indent_string()
    assert_instance_of(String, s)
    assert_equal('  ', s)
  end

  def test_ruby_xml_parser_default_validity_checking()
    XML::Parser::default_validity_checking = false
    assert(!XML::Parser::default_validity_checking)

    XML::Parser::default_validity_checking = true
    assert(XML::Parser::default_validity_checking)

    XML::Parser::default_validity_checking = false
    assert(!XML::Parser::default_validity_checking)
  end

  def test_ruby_xml_parser_default_warnings()
    XML::Parser::default_warnings = false
    assert(!XML::Parser::default_warnings)

    XML::Parser::default_warnings = true
    assert(XML::Parser::default_warnings)

    XML::Parser::default_warnings = false
    assert(!XML::Parser::default_warnings)
  end

  def test_ruby_xml_parser_intent_tree_output()
    assert(TrueClass, XML::Parser::indent_tree_output)

    XML::Parser::indent_tree_output = false
    assert(FalseClass, XML::Parser::indent_tree_output)

    XML::Parser::indent_tree_output = true
    assert(TrueClass, XML::Parser::indent_tree_output)
  end

  def test_ruby_xml_parser_version()
    assert_instance_of(String, XML::Parser::VERSION)
  end

  def test_ruby_xml_parser_vernum()
    assert_instance_of(Fixnum, XML::Parser::VERNUM)
  end

  def test_ruby_xml_parser_parser_context()
    @xp = XML::Parser.string('<rubynet><testing>uga</testing><uga>foo</uga></rubynet>')
    doc = @xp.parse
    assert_instance_of(XML::Document, doc)
    assert_instance_of(XML::Parser::Context, @xp.parser_context)
  end

  def test_libxml_parser_file()
    file = 'tests/model/rubynet.xml'
    assert_equal(file, @xp.filename = file)
    assert_instance_of(XML::Document, @xp.parse)
  end

  def test_libxml_parser_io()
    File.open('tests/model/rubynet.xml') do |f|
      assert_kind_of(IO, f)
      assert_kind_of(IO, @xp.io = f)
      assert_instance_of(XML::Document, @xp.parse)
    end # File.open
  end # def test_libxml_parser_io()

  def test_libxml_parser_features()
    assert_instance_of(Array, XML::Parser::features)
  end
end # TC_XML_Parser
