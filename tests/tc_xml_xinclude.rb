# $Id$
require "libxml_test"
require 'test/unit'

class TC_XML_XInclude < Test::Unit::TestCase
  def setup()
    @doc = XML::Document.file('tests/model/xinclude.xml')
    assert_instance_of(XML::Document, @doc)
  end

  def teardown()
    @doc = nil
  end

  def test_ruby_xml_xinclude()
    xinclude_doc =  "<?xml version=\"1.0\"?>\n<document xmlns:xi=\"http://www.w3.org/2001/XInclude\">\n"
    xinclude_doc << "  <p>This libxml2 binding has the following project information:\n   <code>"
    msg = ''
    File.open('tests/model/rubynet_project') do |f|
      for line in f
        msg << line
      end
    end
    xinclude_doc << msg << "</code></p>\n</document>\n"

    ret = @doc.xinclude
    assert_equal(1, ret)
    assert_equal(xinclude_doc, @doc.to_s)
  end
end
