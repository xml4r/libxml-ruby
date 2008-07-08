require "libxml"
require "tempfile"
require "test/unit"

class TextXPath < Test::Unit::TestCase
  def setup()
    xp = XML::Parser.new()
    str = '<ruby_array uga="booga" foo="bar"><fixnum>one</fixnum><fixnum>two</fixnum></ruby_array>'
    assert_equal(str, xp.string = str)
    doc = xp.parse
    assert_instance_of(XML::Document, doc)
    @xpt = doc.find('/ruby_array/fixnum')
    assert_instance_of(XML::XPath::Object, @xpt)
  end

  def teardown()
    @xpt = nil
  end

  def test_libxml_xpath_set()
    set = @xpt.set
    assert_instance_of(XML::Node::Set, set)
  end

  def test_ary()
    assert_equal(2,@xpt.length)
    assert_equal("one",@xpt.first.content)
    assert_equal("one",@xpt[0].content)
    assert_equal("two",@xpt[1].content)
    assert_equal("two",@xpt[-1].content)
    assert_equal(nil,@xpt[-3])
    assert_equal(nil,@xpt[2])

    @xpt.inject(%w(one two).reverse) {|m,v|
      assert_equal(m.pop,v.content)
      m
    }
  end
  
  def test_custom_function
    xml = Tempfile.new("xxx")
    xml.puts("<a/>")
    xml.close

    doc = XML::Document.file(xml.path)
    assert_nil(doc.find("//*[name(.)=normalize_space(' a ')]"))
  end
end
