# encoding: UTF-8

require_relative './test_helper'
require 'stringio'

class TestParser < Minitest::Test
  def setup
    LibXML::XML::Error.set_handler(&LibXML::XML::Error::QUIET_HANDLER)
  end

  # -----  Sources  -------
  def test_document
    file = File.expand_path(File.join(File.dirname(__FILE__), 'model/bands.utf-8.xml'))
    parser = LibXML::XML::Parser.file(file)
    doc = parser.parse

    parser = LibXML::XML::Parser.document(doc)

    doc = parser.parse

    assert_instance_of(LibXML::XML::Document, doc)
    assert_instance_of(LibXML::XML::Parser::Context, parser.context)
  end

  def test_nil_document
    error = assert_raises(TypeError) do
      LibXML::XML::Parser.document(nil)
    end

    assert_equal("Must pass an LibXML::XML::Document object", error.message)
  end

  def test_file
    file = File.expand_path(File.join(File.dirname(__FILE__), 'model/rubynet.xml'))

    parser = LibXML::XML::Parser.file(file)
    doc = parser.parse
    assert_instance_of(LibXML::XML::Document, doc)
    assert_instance_of(LibXML::XML::Parser::Context, parser.context)
  end

  def test_noexistent_file
    error = assert_raises(LibXML::XML::Error) do
      LibXML::XML::Parser.file('i_dont_exist.xml')
    end

    assert_equal('Warning: failed to load external entity "i_dont_exist.xml".', error.message)
  end

  def test_nil_file
    error = assert_raises(TypeError) do
      LibXML::XML::Parser.file(nil)
    end

    assert_match(/nil into String/, error.message)
  end

  def test_file_encoding
    file = File.expand_path(File.join(File.dirname(__FILE__), 'model/bands.utf-8.xml'))
    parser = LibXML::XML::Parser.file(file, :encoding => LibXML::XML::Encoding::ISO_8859_1)

    error = assert_raises(LibXML::XML::Error) do
      parser.parse
    end

    if windows?
      assert_match(/Fatal error: Opening and ending tag mismatch/, error.message)
    else
      assert_match(/Fatal error: Extra content at the end of the document/, error.message)
    end

    parser = LibXML::XML::Parser.file(file, :encoding => LibXML::XML::Encoding::UTF_8)
    doc = parser.parse
    refute_nil(doc)
  end

  def test_file_base_uri
    file = File.expand_path(File.join(File.dirname(__FILE__), 'model/bands.utf-8.xml'))

    parser = LibXML::XML::Parser.file(file)
    doc = parser.parse
    assert(doc.child.base_uri.match(/test\/model\/bands.utf-8.xml/))

    parser = LibXML::XML::Parser.file(file, :base_uri => "http://libxml.org")
    doc = parser.parse
    assert(doc.child.base_uri.match(/test\/model\/bands.utf-8.xml/))
  end

  def test_io
    File.open(File.join(File.dirname(__FILE__), 'model/rubynet.xml')) do |io|
      parser = LibXML::XML::Parser.io(io)
      assert_instance_of(LibXML::XML::Parser, parser)

      doc = parser.parse
      assert_instance_of(LibXML::XML::Document, doc)
      assert_instance_of(LibXML::XML::Parser::Context, parser.context)
    end
  end

  def test_io_gc
    # Test that the reader keeps a reference
    # to the io object
    file = File.open(File.join(File.dirname(__FILE__), 'model/rubynet.xml'))
    parser = LibXML::XML::Parser.io(file)
    file = nil
    GC.start
    assert(parser.parse)
  end

  def test_nil_io
    error = assert_raises(TypeError) do
      LibXML::XML::Parser.io(nil)
    end

    assert_equal("Must pass in an IO object", error.message)
  end

  def test_string_io
    data = File.read(File.join(File.dirname(__FILE__), 'model/rubynet.xml'))
    string_io = StringIO.new(data)
    parser = LibXML::XML::Parser.io(string_io)

    doc = parser.parse
    assert_instance_of(LibXML::XML::Document, doc)
    assert_instance_of(LibXML::XML::Parser::Context, parser.context)
  end

  def test_string_io_thread
    thread = Thread.new do
      data = File.read(File.join(File.dirname(__FILE__), 'model/rubynet.xml'))
      string_io = StringIO.new(data)
      parser = LibXML::XML::Parser.io(string_io)

      doc = parser.parse
      assert_instance_of(LibXML::XML::Document, doc)
      assert_instance_of(LibXML::XML::Parser::Context, parser.context)
    end

    thread.join
    assert(true)
    puts 'Thread completed'
  end

  def test_string
    str = '<ruby_array uga="booga" foo="bar"><fixnum>one</fixnum><fixnum>two</fixnum></ruby_array>'

    parser = LibXML::XML::Parser.string(str)
    assert_instance_of(LibXML::XML::Parser, parser)

    doc = parser.parse
    assert_instance_of(LibXML::XML::Document, doc)
    assert_instance_of(LibXML::XML::Parser::Context, parser.context)
  end

  def test_nil_string
    error = assert_raises(TypeError) do
      LibXML::XML::Parser.string(nil)
    end

    assert_equal("wrong argument type nil (expected String)", error.message)
  end

  def test_string_options
    xml = <<-EOS
      <!DOCTYPE foo [<!ENTITY foo 'bar'>]>
      <test>
        <cdata><![CDATA[something]]></cdata>
        <entity>&foo;</entity>
      </test>
    EOS

    # Parse normally - entities won't be substituted
    parser = LibXML::XML::Parser.string(xml)
    doc = parser.parse
    assert_nil(doc.child.base_uri)

    # Cdata section should be cdata nodes
    node = doc.find_first('/test/cdata').child
    assert_equal(LibXML::XML::Node::CDATA_SECTION_NODE, node.node_type)

    # Entities should not be substituted
    node = doc.find_first('/test/entity')
    assert_equal('&foo;', node.child.to_s)

    # Parse with options
    parser = LibXML::XML::Parser.string(xml, base_uri: 'http://libxml.rubyforge.org',
                                             options: LibXML::XML::Parser::Options::NOCDATA | LibXML::XML::Parser::Options::NOENT)
    doc = parser.parse
    assert_equal(doc.child.base_uri, 'http://libxml.rubyforge.org')

    # Cdata section should be text nodes
    node = doc.find_first('/test/cdata').child
    assert_equal(LibXML::XML::Node::TEXT_NODE, node.node_type)

    # Entities should be subtituted
    node = doc.find_first('/test/entity')
    assert_equal('bar', node.child.to_s)
  end

  def test_string_encoding
    # ISO_8859_1:
    # ö - f6 in hex, \366 in octal
    # ü - fc in hex, \374 in octal

    xml = <<-EOS
      <bands>
        <metal>m\366tley_cr\374e</metal>
      </bands>
    EOS

    # Parse as UTF_8
    parser = LibXML::XML::Parser.string(xml, :encoding => LibXML::XML::Encoding::UTF_8)

    error = assert_raises(LibXML::XML::Error) do
      parser.parse
    end

    assert_equal("Fatal error: Input is not proper UTF-8, indicate encoding !\nBytes: 0xF6 0x74 0x6C 0x65 at :2.",
                 error.message)

    # Parse as ISO_8859_1:
    parser = LibXML::XML::Parser.string(xml, :encoding => LibXML::XML::Encoding::ISO_8859_1)
    doc = parser.parse
    node = doc.find_first('//metal')
    assert_equal(Encoding::UTF_8, node.content.encoding)
    assert_equal("m\303\266tley_cr\303\274e", node.content)
  end

  def test_fd_gc
    # Test opening # of documents up to the file limit for the OS.
    # Ideally it should run until libxml emits a warning,
    # thereby knowing we've done a GC sweep. For the time being,
    # re-open the same doc `limit descriptors` times.
    # If we make it to the end, then we've succeeded,
    # otherwise an exception will be thrown.
    LibXML::XML::Error.set_handler {|error|}

    max_fd = if RUBY_PLATFORM.match(/mswin32|mswin64|mingw/i)
      500
    else
      Process.getrlimit(Process::RLIMIT_NOFILE)[0] + 1
    end

    file = File.join(File.dirname(__FILE__), 'model/rubynet.xml')
    max_fd.times do
       LibXML::XML::Parser.file(file).parse
    end
    LibXML::XML::Error.reset_handler {|error|}
  end

  def test_open_many_files
    file = File.expand_path(File.join(File.dirname(__FILE__), 'model/atom.xml'))
    1000.times do
      LibXML::XML::Parser.file(file).parse
    end
  end

  # -----  Errors  ------
  def test_error
    error = assert_raises(LibXML::XML::Error) do
      LibXML::XML::Parser.string('<foo><bar/></foz>').parse
    end

    refute_nil(error)
    assert_kind_of(LibXML::XML::Error, error)
    assert_equal("Fatal error: Opening and ending tag mismatch: foo line 1 and foz at :1.", error.message)
    assert_equal(LibXML::XML::Error::PARSER, error.domain)
    assert_equal(LibXML::XML::Error::TAG_NAME_MISMATCH, error.code)
    assert_equal(LibXML::XML::Error::FATAL, error.level)
    assert_nil(error.file)
    assert_equal(1, error.line)
    assert_equal('foo', error.str1)
    assert_equal('foz', error.str2)
    assert_nil(error.str3)
    assert_equal(1, error.int1)
    assert([18, 20].include?(error.int2))
    assert_nil(error.node)
  end

  def test_bad_xml
    parser = LibXML::XML::Parser.string('<ruby_array uga="booga" foo="bar"<fixnum>one</fixnum><fixnum>two</fixnum></ruby_array>')
    error = assert_raises(LibXML::XML::Error) do
      refute_nil(parser.parse)
    end

    refute_nil(error)
    assert_kind_of(LibXML::XML::Error, error)
    if windows?
      assert_equal("Fatal error: Couldn't find end of Start Tag ruby_array line 1 at :1.", error.message)
      assert_equal(LibXML::XML::Error::GT_REQUIRED, error.code)
      assert_equal("ruby_array", error.str1)
      assert_equal(1, error.int1)
    else
      assert_equal("Fatal error: Extra content at the end of the document at :1.", error.message)
      assert_equal(LibXML::XML::Error::DOCUMENT_END, error.code)
      assert_nil(error.str1)
      assert_equal(0, error.int1)
    end
    assert_equal(LibXML::XML::Error::PARSER, error.domain)
    assert_equal(LibXML::XML::Error::FATAL, error.level)
    assert_nil(error.file)
    assert_equal(1, error.line)
    assert_nil(error.str2)
    assert_nil(error.str3)
    assert_equal(34, error.int2)
    assert_nil(error.node)
  end

  def test_errors_from_background_thread
    errors = []
    background_errors = []

    begin
      LibXML::XML::Error.set_handler do |error|
        errors << error
      end
      parser = LibXML::XML::Parser.string("<moo>")

      thread = Thread.new do
        LibXML::XML::Error.set_handler do |error|
          background_errors << error
        end
        parser.parse rescue nil
      end
      thread.join
    ensure
      LibXML::XML::Error.set_handler(&LibXML::XML::Error::QUIET_HANDLER)
    end

    assert_equal(errors.size, 0)
    assert_equal(background_errors.size, 1)
  end
end
