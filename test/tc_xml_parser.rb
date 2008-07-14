require "libxml"
require 'test/unit'

class TestParser < Test::Unit::TestCase
  def setup
    LibXML::Parser.register_error_handler(nil)
    @xp = LibXML::Parser.new
  end

  def teardown
    @xp = nil
    LibXML::Parser.register_error_handler(nil)
    # Clear out all the files we opened up in
    # the test_fd_gc test
    GC.start
  end
      
  # -----  Constants  ------
  def test_lib_versions
    assert(LibXML::Parser::check_lib_versions)
  end

  def test_debug_entities
    LibXML::Parser::debug_entities = false
    assert(!LibXML::Parser::debug_entities)

    LibXML::Parser::debug_entities = true
    assert(LibXML::Parser::debug_entities)

    LibXML::Parser::debug_entities = false
    assert(!LibXML::Parser::debug_entities)
  end

  def test_default_compression
    return unless LibXML::Parser::default_compression
    
    0.upto(9) do |i|
      LibXML::Parser::default_compression = i
      assert_equal(i, LibXML::Parser::default_compression)
    end

    9.downto(0) do |i|
      assert_equal(i, LibXML::Parser::default_compression = i)
      assert_equal(i, LibXML::Parser::default_compression)
    end

    0.downto(-10) do |i|
      assert_equal(i, LibXML::Parser::default_compression = i)
      assert_equal(0, LibXML::Parser::default_compression)
    end

    10.upto(20) do |i|
      assert_equal(i, LibXML::Parser::default_compression = i)
      assert_equal(9, LibXML::Parser::default_compression)
    end
  end

  def test_default_keep_blanks
    LibXML::Parser::default_keep_blanks = false
    assert(!LibXML::Parser::default_keep_blanks)

    LibXML::Parser::default_keep_blanks = true
    assert(LibXML::Parser::default_keep_blanks)

    LibXML::Parser::default_keep_blanks = false
    assert(!LibXML::Parser::default_keep_blanks)

    # other tests depend on keeping blanks by default,
    # which is the default default behaviour anyway.
    LibXML::Parser::default_keep_blanks = true
  end

  def test_default_line_numbers
    LibXML::Parser::default_line_numbers = false
    assert(!LibXML::Parser::default_line_numbers)

    LibXML::Parser::default_line_numbers = true
    assert(LibXML::Parser::default_line_numbers)

    LibXML::Parser::default_line_numbers = false
    assert(!LibXML::Parser::default_line_numbers)
  end

  def test_default_substitute_entities
    LibXML::Parser::default_substitute_entities = false
    assert(!LibXML::Parser::default_substitute_entities)

    LibXML::Parser::default_substitute_entities = true
    assert(LibXML::Parser::default_substitute_entities)

    LibXML::Parser::default_substitute_entities = false
    assert(!LibXML::Parser::default_substitute_entities)
  end

  def test_default_tree_indent_string
    s = LibXML::Parser::default_tree_indent_string
    assert_instance_of(String, s)
    assert_equal('  ', s)
    LibXML::Parser::default_tree_indent_string = 'uga'
    s = LibXML::Parser::default_tree_indent_string
    assert_instance_of(String, s)
    assert_equal('uga', s)
    LibXML::Parser::default_tree_indent_string = '  '
    s = LibXML::Parser::default_tree_indent_string
    assert_instance_of(String, s)
    assert_equal('  ', s)
  end

  def test_default_validity_checking
    LibXML::Parser::default_validity_checking = false
    assert(!LibXML::Parser::default_validity_checking)

    LibXML::Parser::default_validity_checking = true
    assert(LibXML::Parser::default_validity_checking)

    LibXML::Parser::default_validity_checking = false
    assert(!LibXML::Parser::default_validity_checking)
  end

  def test_default_warnings
    LibXML::Parser::default_warnings = false
    assert(!LibXML::Parser::default_warnings)

    LibXML::Parser::default_warnings = true
    assert(LibXML::Parser::default_warnings)

    LibXML::Parser::default_warnings = false
    assert(!LibXML::Parser::default_warnings)
  end

  def test_intent_tree_output
    assert(TrueClass, LibXML::Parser::indent_tree_output)

    LibXML::Parser::indent_tree_output = false
    assert(FalseClass, LibXML::Parser::indent_tree_output)

    LibXML::Parser::indent_tree_output = true
    assert(TrueClass, LibXML::Parser::indent_tree_output)
  end

  def test_version
    assert_instance_of(String, LibXML::Parser::VERSION)
  end

  def test_vernum
    assert_instance_of(Fixnum, LibXML::Parser::VERNUM)
  end

  # -----  IO  ------
  def test_string
    str = '<ruby_array uga="booga" foo="bar"><fixnum>one</fixnum><fixnum>two</fixnum></ruby_array>'
    assert_equal(str, @xp.string = str)
    assert_instance_of(LibXML::Document, @xp.parse)
  end
  
  def test_context
    @xp = LibXML::Parser.string('<rubynet><testing>uga</testing><uga>foo</uga></rubynet>')
    doc = @xp.parse
    assert_instance_of(LibXML::Document, doc)
    assert_instance_of(LibXML::Parser::Context, @xp.context)
  end

  def test_file
    file = File.expand_path(File.join(File.dirname(__FILE__), 'model/rubynet.xml'))
    
    @xp.filename = file
    assert_equal(file, @xp.filename)
    
    doc = @xp.parse
    assert_instance_of(LibXML::Document, doc)
  end

  def test_io
    File.open(File.join(File.dirname(__FILE__), 'model/rubynet.xml')) do |f|
      assert_kind_of(IO, f)
      assert_kind_of(IO, @xp.io = f)
      assert_instance_of(LibXML::Document, @xp.parse)
    end # File.open
  end # def test_libxml_parser_io

  def test_fd_gc
    # Test opening # of documents up to the file limit for the OS.
    # Ideally it should run until libxml emits a warning, 
    # thereby knowing we've done a GC sweep. For the time being,
    # re-open the same doc `limit descriptors` times. 
    # If we make it to the end, then we've succeeded, 
    # otherwise an exception will be thrown.
    LibXML::Parser.register_error_handler(lambda {|msg| nil })
    
    max_fd = if RUBY_PLATFORM.match(/mswin32/i)
      500
    else
      (`ulimit -n`.chomp.to_i) + 1
    end
    
    filename = File.join(File.dirname(__FILE__), 'model/rubynet.xml')
    max_fd.times do
       LibXML::Document.file(filename)
    end
  end

  def test_libxml_parser_features
    assert_instance_of(Array, LibXML::Parser::features)
  end

  # -----  Errors  ------
  def test_error_handler
    assert_raise(LibXML::Parser::ParseError) do
      LibXML::Parser.string('<foo><bar/></foz>').parse
    end

    ary = []
    LibXML::Parser.register_error_handler do |msg|
      ary << msg
    end

    # this will use our error handler
    assert_raise(LibXML::Parser::ParseError) do
      LibXML::Parser.string('<foo><bar/></foz>').parse
    end
    
    assert_equal(["Entity: line 1: ",
                  "parser ",
                  "error : ",
                  "Opening and ending tag mismatch: foo line 1 and foz\n",
                  "<foo><bar/></foz>\n",
                  "                 ^\n"], ary)

    assert_instance_of(Proc, LibXML::Parser.register_error_handler(nil))

    # this will go to stderr again
    assert_raise(LibXML::Parser::ParseError) do
      d = LibXML::Parser.string('<foo><bar/></foz>').parse
    end
  end
  
  def test_error_handler_lambda
    LibXML::Parser.register_error_handler(nil)
    assert_raise(LibXML::Parser::ParseError) do
      LibXML::Parser.string('<foo><bar/></foz>').parse
    end

    ary = []
    assert_nil LibXML::Parser.register_error_handler(lambda { |msg| ary << msg })

    # this will use our error handler
    assert_raise(LibXML::Parser::ParseError) do
      LibXML::Parser.string('<foo><bar/></foz>').parse
    end
    
    assert_equal(["Entity: line 1: ",
                  "parser ",
                  "error : ",
                  "Opening and ending tag mismatch: foo line 1 and foz\n",
                  "<foo><bar/></foz>\n",
                  "                 ^\n"], ary)

    assert_instance_of(Proc, LibXML::Parser.register_error_handler(nil))

    # this will go to stderr again
    assert_raise(LibXML::Parser::ParseError) do
      d = LibXML::Parser.string('<foo><bar/></foz>').parse
    end
  end
  
  def test_bad_xml
    @xp.string = '<ruby_array uga="booga" foo="bar"<fixnum>one</fixnum><fixnum>two</fixnum></ruby_array>'
    LibXML::Parser.register_error_handler(lambda {|msg| nil })
    assert_raise(LibXML::Parser::ParseError) do
      assert_not_nil(@xp.parse)
    end
  end
  
  def test_double_parse
    LibXML::Parser.register_error_handler(lambda {|msg| nil })
    parser = LibXML::Parser.string("<test>something</test>")
    doc = parser.parse
    
    assert_raise(LibXML::Parser::ParseError) do
      parser.parse
    end
  end
  
  def test_libxml_parser_empty_string
    assert_raise(LibXML::Parser::ParseError) do
      @xp.string = ''
      @xp.parse
    end
    
    assert_raise(TypeError) do
      @xp.string = nil
    end
  end
end
