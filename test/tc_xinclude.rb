require 'xml'
require 'test/unit'

class TestXInclude < Test::Unit::TestCase
  def setup
    @doc = XML::Document.file(File.join(File.dirname(__FILE__), 'model/xinclude.xml'))
    assert_instance_of(XML::Document, @doc)
  end

  def teardown
    @doc = nil
  end

  def test_ruby_xml_xinclude
    expected = <<-EOS
<?xml version="1.0"?>
<document xmlns:xi="http://www.w3.org/2001/XInclude">
  <p>This libxml2 binding has the following project information:
   <code>This is some text to include in an xml file via XInclude.</code></p>
</document>
EOS

    assert_equal(1, @doc.xinclude)
    assert_equal(expected, @doc.to_s)    
  end
end
