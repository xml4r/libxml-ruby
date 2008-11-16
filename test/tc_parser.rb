require 'xml'
require 'test/unit'
require 'stringio'

class TestParser < Test::Unit::TestCase
  def setup
    XML::Error.set_handler(&XML::Error::QUIET_HANDLER)
    @xp = XML::Parser.new
  end

  def teardown
    @xp = nil
    # Clear out all the files we opened up in
    # the test_fd_gc test
    GC.start
  end
      
  # -----  Constants  ------
  def test_lib_versions
    assert(XML::Parser::check_lib_versions)
  end

  def test_debug_entities
    XML::Parser::debug_entities = false
    assert(!XML::Parser::debug_entities)

    XML::Parser::debug_entities = true
    assert(XML::Parser::debug_entities)

    XML::Parser::debug_entities = false
    assert(!XML::Parser::debug_entities)
  end

  def test_default_compression
    return unless XML::Parser::default_compression
    
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

  def test_default_keep_blanks
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

  def test_default_line_numbers
    XML::Parser::default_line_numbers = false
    assert(!XML::Parser::default_line_numbers)

    XML::Parser::default_line_numbers = true
    assert(XML::Parser::default_line_numbers)

    XML::Parser::default_line_numbers = false
    assert(!XML::Parser::default_line_numbers)
  end

  def test_default_substitute_entities
    XML::Parser::default_substitute_entities = false
    assert(!XML::Parser::default_substitute_entities)

    XML::Parser::default_substitute_entities = true
    assert(XML::Parser::default_substitute_entities)

    XML::Parser::default_substitute_entities = false
    assert(!XML::Parser::default_substitute_entities)
  end

  def test_default_tree_indent_string
    s = XML::Parser::default_tree_indent_string
    assert_instance_of(String, s)
    assert_equal('  ', s)
    XML::Parser::default_tree_indent_string = 'uga'
    s = XML::Parser::default_tree_indent_string
    assert_instance_of(String, s)
    assert_equal('uga', s)
    XML::Parser::default_tree_indent_string = '  '
    s = XML::Parser::default_tree_indent_string
    assert_instance_of(String, s)
    assert_equal('  ', s)
  end

  def test_default_validity_checking
    XML::Parser::default_validity_checking = false
    assert(!XML::Parser::default_validity_checking)

    XML::Parser::default_validity_checking = true
    assert(XML::Parser::default_validity_checking)

    XML::Parser::default_validity_checking = false
    assert(!XML::Parser::default_validity_checking)
  end

  def test_default_warnings
    XML::Parser::default_warnings = false
    assert(!XML::Parser::default_warnings)

    XML::Parser::default_warnings = true
    assert(XML::Parser::default_warnings)

    XML::Parser::default_warnings = false
    assert(!XML::Parser::default_warnings)
  end

  def test_intent_tree_output
    assert(TrueClass, XML::Parser::indent_tree_output)

    XML::Parser::indent_tree_output = false
    assert(FalseClass, XML::Parser::indent_tree_output)

    XML::Parser::indent_tree_output = true
    assert(TrueClass, XML::Parser::indent_tree_output)
  end

  def test_version
    assert_instance_of(String, XML::Parser::VERSION)
  end

  def test_vernum
    assert_instance_of(Fixnum, XML::Parser::VERNUM)
  end

  # -----  IO  ------
  def test_string
    str = '<ruby_array uga="booga" foo="bar"><fixnum>one</fixnum><fixnum>two</fixnum></ruby_array>'
    assert_equal(str, @xp.string = str)
    assert_instance_of(XML::Document, @xp.parse)
  end
  
  def test_context
    @xp = XML::Parser.string('<rubynet><testing>uga</testing><uga>foo</uga></rubynet>')
    doc = @xp.parse
    assert_instance_of(XML::Document, doc)
    assert_instance_of(XML::Parser::Context, @xp.context)
  end

  def test_file
    file = File.expand_path(File.join(File.dirname(__FILE__), 'model/rubynet.xml'))
    
    @xp.filename = file
    assert_equal(file, @xp.filename)
    
    doc = @xp.parse
    assert_instance_of(XML::Document, doc)
  end

  def test_io
    File.open(File.join(File.dirname(__FILE__), 'model/rubynet.xml')) do |f|
      assert_kind_of(IO, f)
      assert_kind_of(IO, @xp.io = f)
      assert_instance_of(XML::Document, @xp.parse)
    end
  end

  def test_string_io
    data = File.read(File.join(File.dirname(__FILE__), 'model/rubynet.xml'))
    string_io = StringIO.new(data)
    assert_raises(TypeError) do
      assert_kind_of(StringIO, @xp.io = string_io)
    end
  end

  def test_fd_gc
    # Test opening # of documents up to the file limit for the OS.
    # Ideally it should run until libxml emits a warning, 
    # thereby knowing we've done a GC sweep. For the time being,
    # re-open the same doc `limit descriptors` times. 
    # If we make it to the end, then we've succeeded, 
    # otherwise an exception will be thrown.
    XML::Error.set_handler {|error|}
    
    max_fd = if RUBY_PLATFORM.match(/mswin32/i)
      500
    else
      (`ulimit -n`.chomp.to_i) + 1
    end
    
    filename = File.join(File.dirname(__FILE__), 'model/rubynet.xml')
    max_fd.times do
       XML::Document.file(filename)
    end
    XML::Error.reset_handler {|error|}
  end

  def test_libxml_parser_features
    assert_instance_of(Array, XML::Parser::features)
  end

  # -----  Errors  ------
  def test_error
    error = assert_raise(XML::Error) do
      XML::Parser.string('<foo><bar/></foz>').parse
    end

    assert_not_nil(error)
    assert_kind_of(XML::Error, error)
    assert_equal("Fatal error: Opening and ending tag mismatch: foo line 1 and foz at :1.", error.message)
    assert_equal(XML::Error::XML_FROM_PARSER, error.domain)
    assert_equal(XML::Error::XML_ERR_TAG_NAME_MISMATCH, error.code)
    assert_equal(XML::Error::XML_ERR_FATAL, error.level)
    assert_nil(error.file)
    assert_equal(1, error.line)
    assert_equal('foo', error.str1)
    assert_equal('foz', error.str2)
    assert_nil(error.str3)
    assert_equal(1, error.int1)
    assert_equal(20, error.int2)
    assert_nil(error.node)
  end
  
  def test_bad_xml
    @xp.string = '<ruby_array uga="booga" foo="bar"<fixnum>one</fixnum><fixnum>two</fixnum></ruby_array>'
    error = assert_raise(XML::Error) do
      assert_not_nil(@xp.parse)
    end

    assert_not_nil(error)
    assert_kind_of(XML::Error, error)
    assert_equal("Fatal error: Extra content at the end of the document at :1.", error.message)
    assert_equal(XML::Error::XML_FROM_PARSER, error.domain)
    assert_equal(XML::Error::XML_ERR_DOCUMENT_END, error.code)
    assert_equal(XML::Error::XML_ERR_FATAL, error.level)
    assert_nil(error.file)
    assert_equal(1, error.line)
    assert_nil(error.str1)
    assert_nil(error.str2)
    assert_nil(error.str3)
    assert_equal(0, error.int1)
    assert_equal(20, error.int2)
    assert_nil(error.node)
  end
  
  def test_double_parse
    parser = XML::Parser.string("<test>something</test>")
    doc = parser.parse

    assert_raise(XML::Error) do
      parser.parse
    end
  end

  def test_libxml_parser_empty_string
    assert_raise(XML::Error) do
      @xp.string = ''
      @xp.parse
    end

    assert_raise(TypeError) do
      @xp.string = nil
    end
  end
end
