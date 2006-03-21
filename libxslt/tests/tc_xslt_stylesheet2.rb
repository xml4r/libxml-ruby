# $Id$
begin
  require 'xml/libxml'
rescue LoadError
  require 'rubygems' and retry    
end
$TDIR = File.dirname(__FILE__)
require "#$TDIR/../ext/xml/libxslt"
require 'test/unit'

class TC_XSLT_Stylesheet2 < Test::Unit::TestCase
  def setup()
    @xslt = XML::XSLT.file("#$TDIR/fuzface.xsl")
    @xslt.doc = XML::Document.file("#$TDIR/fuzface.xml")
    assert_instance_of(XML::XSLT, @xslt)
    assert_instance_of(XML::Document, @xslt.doc)
    @stylesheet = @xslt.parse
    assert_instance_of(XML::XSLT::Stylesheet, @stylesheet)
  end
  
  def tear_down()
    @xslt = nil
    @stylesheet = nil
  end
  
  def test_ruby_xslt_parse()
    assert_instance_of(XML::XSLT::Stylesheet, @stylesheet)
  end

  def test_ruby_xslt_stylesheet_to_s()
    assert_raises(XML::XSLT::Stylesheet::RequireParsedDoc) do
      str = @stylesheet.to_s
    end
  end

  def test_ruby_xslt_stylesheet_print()
    assert_raises(XML::XSLT::Stylesheet::RequireParsedDoc) do
      @stylesheet.print
    end
  end
end
