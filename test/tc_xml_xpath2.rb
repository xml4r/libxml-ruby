require "libxml"
require "test/unit"
require "tempfile"

class TC_XML_XPath2 < Test::Unit::TestCase
  def test_custom_xpath_function
    xml = Tempfile.new("xxx")
    xml.puts("<a/>")
    xml.close

    doc = XML::Document.file(xml.path)
    assert_nil(doc.find("//*[name(.)=normalize_space(' a ')]"))
  end
end 