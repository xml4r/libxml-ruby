require "xml"
require 'test/unit'


class TC_XML_Document < Test::Unit::TestCase
  def setup
    xp = XML::Parser.new
    assert_instance_of(XML::Parser, xp)
    str = '<ruby_array uga="booga" foo="bar"><fixnum>one</fixnum><fixnum>two</fixnum></ruby_array>'
    assert_equal(str, xp.string = str)
    @doc = xp.parse
    assert_instance_of(XML::Document, @doc)
  end

  def teardown
    @doc = nil
  end

  def test_libxml_document_find
    set = @doc.find('/ruby_array/fixnum')
    assert_instance_of(XML::XPath::Object, set)
    assert_raise(NoMethodError) {
      xpt = set.xpath
    }
  end

  def test_ruby_xml_document_compression
    if XML::Parser::enabled_zlib?
      0.upto(9) do |i|
        assert_equal(i, @doc.compression = i)
        assert_equal(i, @doc.compression)
      end

      9.downto(0) do |i|
        assert_equal(i, @doc.compression = i)
        assert_equal(i, @doc.compression)
      end

      10.upto(20) do |i|
        # assert_equal(9, @doc.compression = i)
        assert_equal(i, @doc.compression = i) # This works around a bug in Ruby 1.8
        assert_equal(9, @doc.compression)
      end

      -1.downto(-10) do |i|
        # assert_equal(0, @doc.compression = i)
        assert_equal(i, @doc.compression = i) # FIXME This bug should get fixed ASAP
        assert_equal(0, @doc.compression)
      end
    end
  end
  
  def test_save
    filename = 'test_write' 
    bytes = @doc.save(filename)
    assert_equal(110, bytes)
    contents = File.read(filename)
    
    expected =<<-EOS
<?xml version="1.0"?>
<ruby_array uga="booga" foo="bar"><fixnum>one</fixnum><fixnum>two</fixnum></ruby_array>
EOS
    assert_equal(expected, contents)
  ensure
    File.delete(filename)    
  end
  
  def test_save_formatted
    filename = 'test_write' 
    bytes = @doc.save(filename, true)
    assert_equal(117, bytes)
    contents = File.read(filename)
    
    expected =<<-EOS
<?xml version="1.0"?>
<ruby_array uga="booga" foo="bar">
  <fixnum>one</fixnum>
  <fixnum>two</fixnum>
</ruby_array>
EOS
    assert_equal(expected, contents)
  ensure
    File.delete(filename)    
  end
  
  def test_save_formatted_invalid
    filename = 'test_write' 
    
    assert_raise(ArgumentError) do
      @doc.save(filename, 1)
    end
  end  
end
