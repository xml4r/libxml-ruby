# $Id$
begin
  require 'xml/libxml'
rescue LoadError => orig
  require 'rubygems' and retry    
  raise orig
end

$TDIR = File.dirname(__FILE__)
require "#$TDIR/../ext/xml/libxslt"
require 'test/unit'

class TC_XSLT < Test::Unit::TestCase
  def setup()
    @xslt = XML::XSLT.file("#$TDIR/fuzface.xsl")
    @xslt.doc = XML::Document.file("#$TDIR/fuzface.xml")
    assert_instance_of(XML::XSLT, @xslt)
    assert_instance_of(XML::Document, @xslt.doc)
    f = File.open("#$TDIR/fuzface.xsl")
    assert_instance_of(File, f)

    @xslt2 = XML::XSLT.new()
    assert_instance_of(XML::XSLT, @xslt2)
    @xslt2.filename = "#$TDIR/fuzface.xsl"
    assert_instance_of(String, @xslt.filename)

    @stylesheet = @xslt.parse
    assert_instance_of(XML::XSLT::Stylesheet, @stylesheet)
  end
  
  def tear_down()
    @xslt = nil
    @xslt2 = nil
    @stylesheet = nil
  end

  def test_ruby_xslt_constants()
    assert_instance_of(Fixnum, XML::XSLT::MAX_DEPTH)
    assert_instance_of(Fixnum, XML::XSLT::MAX_SORT)
    assert_instance_of(String, XML::XSLT::ENGINE_VERSION)
    assert_instance_of(Fixnum, XML::XSLT::LIBXSLT_VERSION)
    assert_instance_of(Fixnum, XML::XSLT::LIBXML_VERSION)
    assert_instance_of(String, XML::XSLT::XSLT_NAMESPACE)
    assert_instance_of(String, XML::XSLT::DEFAULT_URL)
    assert_instance_of(String, XML::XSLT::DEFAULT_VENDOR)
    assert_instance_of(String, XML::XSLT::DEFAULT_VERSION)
    assert_instance_of(String, XML::XSLT::NAMESPACE_LIBXSLT)
    assert_instance_of(String, XML::XSLT::NAMESPACE_SAXON)
    assert_instance_of(String, XML::XSLT::NAMESPACE_XT)
    assert_instance_of(String, XML::XSLT::NAMESPACE_XALAN)
    assert_instance_of(String, XML::XSLT::NAMESPACE_NORM_SAXON)
  end

  def test_ruby_xslt_file()
    assert_instance_of(XML::XSLT, @xslt)
  end

  def test_ruby_xslt_new()
    assert_instance_of(XML::XSLT, @xslt2)
  end

  def test_ruby_xslt_parse()
    assert_instance_of(XML::XSLT::Stylesheet, @stylesheet)
  end
end
