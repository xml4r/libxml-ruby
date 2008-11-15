require 'xml'
require 'test/unit'
require 'stringio'

class TestError < Test::Unit::TestCase
  def test_invalid_handler
    assert_raises(RuntimeError) do
      XML::Error.set_handler
    end
  end

#  def test_handler
#    exception = nil
#    XML::Error.set_handler do |error|
#      exception = error
#    end
#
#    # Raise the error
#    XML::Reader.new('<foo').read
#
#    # Check the handler worked
#    assert_not_nil(exception)
#    assert_kind_of(XML::Error, exception)
#    assert_equal("Couldn't find end of Start Tag foo at :1", exception.message)
#    assert_equal(XML::Error::XML_FROM_PARSER, exception.domain)
#    assert_equal(XML::Error::XML_ERR_GT_REQUIRED, exception.code)
#    assert_equal(XML::Error::XML_ERR_FATAL, exception.level)
#    assert_nil(exception.file)
#    assert_equal(1, exception.line)
#    assert_equal('foo', exception.str1)
#    assert_nil(exception.str2)
#    assert_nil(exception.str3)
#    assert_equal(0, exception.int1)
#    assert_equal(5, exception.int2)
#    assert_nil(exception.node)
#  end
#
#  def test_reset_handler
#    exception = nil
#    XML::Error.set_handler do |error|
#      exception = error
#    end
#
#    XML::Error.reset_handler
#    reader = XML::Reader.new('<foo')
#    assert_nil(exception)
#  end
#
#  def test_stderr
#    output = StringIO.new
#    original_stderr = Object::STDERR
#
#    Object.const_set(:STDERR, output)
#    begin
#      assert_raise(XML::Error) do
#        XML::Parser.string('<foo><bar/></foz>').parse
#      end
#    ensure
#      Object.const_set(:STDERR, original_stderr)
#    end
#    assert_equal('Opening and ending tag mismatch: foo line 1 and foz at :1', output.string)
#  end
#
#  def test_no_stderr
#    output = StringIO.new
#    original_stderr = Object::STDERR
#
#    XML::Error.observe(self) do |error|
#      # Eat error message
#    end
#
#    Object.const_set(:STDERR, output)
#    begin
#      assert_raise(XML::Error) do
#        XML::Parser.string('<foo><bar/></foz>').parse
#      end
#    ensure
#      Object.const_set(:STDERR, original_stderr)
#      XML::Error.unobserve(self)
#    end
#    assert_equal('', output.string)
#  end
#
#
#  def test_parse_error
#    exception = assert_raise(XML::Error) do
#      XML::Parser.string('<foo><bar/></foz>').parse
#    end
#
#    assert_instance_of(XML::Error, exception)
#    assert_equal("Opening and ending tag mismatch: foo line 1 and foz\n", exception.message)
#    assert_equal(XML::Error::XML_FROM_PARSER, exception.domain)
#    assert_equal(XML::Error::XML_ERR_TAG_NAME_MISMATCH, exception.code)
#    assert_equal(XML::Error::XML_ERR_FATAL, exception.level)
#    assert_nil(exception.file)
#    assert_equal(1, exception.line)
#  end
#
#  def test_xpath_error
#    doc = XML::Document.file(File.join(File.dirname(__FILE__), 'model/soap.xml'))
#
#    exception = assert_raise(XML::Error) do
#      elements = doc.find('/foo[bar=test')
#    end
#
#    assert_instance_of(XML::Error, exception)
#    assert_equal("Invalid predicate at :0", exception.message)
#    assert_equal(XML::Error::XML_FROM_XPATH, exception.domain)
#    assert_equal(XML::Error::XML_XPATH_INVALID_PREDICATE_ERROR, exception.code)
#    assert_equal(XML::Error::XML_ERR_ERROR, exception.level)
#    assert_nil(exception.file)
#    assert_nil(nil)
#  end
#
#  def test_parser_error
#    assert_raise(XML::Parser::ParseError) do
#      XML::Parser.string('<foo><bar/></foz>').parse
#    end
#
#    assert_equal('', output.read)
#
#    assert_equal(["Entity: line 1: ",
#                  "parser ",
#                  "error : ",
#                  "Opening and ending tag mismatch: foo line 1 and foz\n",
#                  "<foo><bar/></foz>\n",
#                  "                 ^\n"], ary)
#
#    assert_instance_of(Proc, XML::Parser.register_error_handler(nil))
#
#    # this will go to stderr again
#    assert_raise(XML::Parser::ParseError) do
#      d = XML::Parser.string('<foo><bar/></foz>').parse
#    end
#  end
#
#  def test_error_handler_lambda
#    XML::Parser.register_error_handler(nil)
#    assert_raise(XML::Parser::ParseError) do
#      XML::Parser.string('<foo><bar/></foz>').parse
#    end
#
#    ary = []
#    assert_nil XML::Parser.register_error_handler(lambda { |msg| ary << msg })
#
#    # this will use our error handler
#    assert_raise(XML::Parser::ParseError) do
#      XML::Parser.string('<foo><bar/></foz>').parse
#    end
#
#    assert_equal(["Entity: line 1: ",
#                  "parser ",
#                  "error : ",
#                  "Opening and ending tag mismatch: foo line 1 and foz\n",
#                  "<foo><bar/></foz>\n",
#                  "                 ^\n"], ary)
#
#    assert_instance_of(Proc, XML::Parser.register_error_handler(nil))
#
#    # this will go to stderr again
#    assert_raise(XML::Parser::ParseError) do
#      d = XML::Parser.string('<foo><bar/></foz>').parse
#    end
#  end

 

#  def test_double_parse
#    XML::Parser.register_error_handler(lambda {|msg| nil })
#    parser = XML::Parser.string("<test>something</test>")
#    doc = parser.parse
#
#    assert_raise(XML::Parser::ParseError) do
#      parser.parse
#    end
#  end
#
#  def test_libxml_parser_empty_string
#    assert_raise(XML::Parser::ParseError) do
#      @xp.string = ''
#      @xp.parse
#    end
#
#    assert_raise(TypeError) do
#      @xp.string = nil
#    end
#  end
end
