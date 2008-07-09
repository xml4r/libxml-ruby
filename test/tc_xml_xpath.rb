require "libxml"
require "tempfile"
require "test/unit"

class TextXPath < Test::Unit::TestCase
  def setup()
    @doc = XML::Parser.string(<<-EOS).parse
      <ruby_array uga="booga" foo="bar">
        <fixnum>one</fixnum>
        <fixnum>two</fixnum>
      </ruby_array>
    EOS
  end

  def teardown()
    @doc = nil
  end

  def test_find_class
    nodes = @doc.find('/ruby_array/fixnum')
    assert_instance_of(XML::XPath::Object, nodes)
  end

  def test_xpath_set
    nodes = @doc.find('/ruby_array/fixnum')
    assert_instance_of(XML::Node::Set, nodes.set)
  end
  
  def test_xpath_empty
    nodes = @doc.find('/ruby_array/i_dont_exist')
    assert_equal(0, nodes.length)
    
    node = nodes.first
    assert_nil(node)
  end    

  def test_ary
    nodes = @doc.find('/ruby_array/fixnum')
    assert_equal(2, nodes.length)
    assert_equal("one", nodes.first.content)
    assert_equal("one", nodes[0].content)
    assert_equal("two", nodes[1].content)
    assert_equal("two", nodes[-1].content)
    assert_equal(nil, nodes[-3])
    assert_equal(nil, nodes[2])

    nodes.inject(%w(one two).reverse) do |m,v|
      assert_equal(m.pop,v.content)
      m
    end
  end
  
  def test_custom_function
    xml = Tempfile.new("xxx")
    xml.puts("<a/>")
    xml.close

    doc = XML::Document.file(xml.path)
    assert_nil(doc.find("//*[name(.)=normalize_space(' a ')]"))
  end
end
