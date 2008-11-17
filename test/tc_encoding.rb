require 'xml'
require 'test/unit'

class TestEncoding < Test::Unit::TestCase
  def test_latin1
    assert_equal(XML::Encoding::ISO_8859_1, 10)
  end

  def test_latin1_to_s
    encoding_str = XML::Encoding.encoding_to_s(XML::Encoding::ISO_8859_1)
    assert_equal('ISO-8859-1', encoding_str)
  end
end
