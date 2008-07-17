require "xml"
require 'test/unit'

class TC_XML_Document_Write < Test::Unit::TestCase
  def setup
    @doc = XML::Document.new('1.0')
  end

  def teardown
    @doc = nil
  end

  def test_klass
    assert_instance_of(XML::Document, @doc)
  end

  def test_version
    assert_equal('1.0', @doc.version)

    doc = XML::Document.new('6.9')
    assert_equal('6.9', doc.version)
  end
  
  def test_write_root
    @doc.root = XML::Node.new('rubynet')
    assert_instance_of(XML::Node, @doc.root)
    assert_instance_of(XML::Document, @doc.root.doc)
    assert_equal("<?xml version=\"1.0\"?>\n<rubynet/>\n", @doc.to_s)
  end

  def test_write_root
    @doc.root = XML::Node.new('rubynet')
    assert_instance_of(XML::Node, @doc.root)
    assert_instance_of(XML::Document, @doc.root.doc)
    assert_equal("<?xml version=\"1.0\"?>\n<rubynet/>\n", @doc.to_s(false))
  end

  def test_encoding
    @doc.root = XML::Node.new('rubynet')
    @doc.encoding = 'UTF-8'
    assert_instance_of(XML::Node, @doc.root)
    assert_equal("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<rubynet/>\n", @doc.to_s)
  end

  def test_child()
    xml = "<?xml version=\"1.0\"?>\n<rubynet>\n  <pkg/>\n</rubynet>\n"
    @doc.root = XML::Node.new('rubynet')
    @doc.root.child = XML::Node.new('pkg')
    assert_equal(xml, @doc.to_s)
  end

  def test_append
    xml = "<?xml version=\"1.0\"?>\n"
    xml << "<rubynet>\n"
    xml << "  <pkg>\n"
    xml << "    <meta>\n"
    xml << "      <pkgname>libxml</pkgname>\n"
    xml << "    </meta>\n"
    xml << "  </pkg>\n"
    xml << "</rubynet>\n"
    
    @doc.root = XML::Node.new('rubynet')
    pkg = @doc.root.child = XML::Node.new('pkg')
    meta = pkg.child = XML::Node.new('meta')
    assert_equal(meta, meta << (pkgname = XML::Node.new('pkgname')))
    pkgname << 'libxml'
    assert_equal(xml, @doc.to_s)
  end

  def test_node_search_href
    prefix = 'xlink'
    uri = 'http://www.rubynet.org'
    xml = "<?xml version=\"1.0\"?>\n"
    xml << "<rubynet xmlns:#{prefix}=\"#{uri}\">\n"
    xml << "  <pkg/>\n"
    xml << "</rubynet>\n"
    
    @doc.root = XML::Node.new('rubynet')
    @doc = XML::Parser.string(xml).parse
    assert_equal(xml, @doc.to_s)
    ns = @doc.root.search_href(uri)
    assert_instance_of(XML::NS, ns)
    assert_equal(uri, ns.href)
    assert_equal(prefix, ns.prefix)
  end

  def test_sibling
    xml = "<?xml version=\"1.0\"?>\n<rubynet>\n  <pkg/>\n  <pkg2/>\n</rubynet>\n"

    @doc.root = XML::Node.new('rubynet')
    child = @doc.root.child = XML::Node.new('pkg')
    assert_instance_of(XML::Node, child)
    child.sibling = XML::Node.new('pkg2')
    assert_equal(xml, @doc.to_s)
  end

  def test_set_doc_property
    xml = "<?xml version=\"1.0\"?>\n"
    xml << "<rubynet xmlns:xlink=\"http://www.w3.org/1999/xlink\">\n"
    xml << "  <pkg/>\n"
    xml << "</rubynet>\n"
    
    @doc.root = XML::Node.new('rubynet')
    @doc.root['xmlns:xlink'] = "http://www.w3.org/1999/xlink"
    pkg = @doc.root.child = XML::Node.new('pkg')
    assert_equal(xml, @doc.to_s)
  end

  def test_set_node_base
    xml = "<?xml version=\"1.0\"?>\n"
    xml << "<rubynet>\n"
    xml << "  <pkg xml:base=\"http://www.rubynet.org/\"/>\n"
    xml << "</rubynet>\n"
    
    @doc.root = XML::Node.new('rubynet')
    pkg = @doc.root.child = XML::Node.new('pkg')
    pkg.base = 'http://www.rubynet.org/'
    assert_equal(xml, @doc.to_s)
  end

  def test_set_node_property
    xml = "<?xml version=\"1.0\"?>\n"
    xml << "<rubynet xmlns:xlink=\"http://www.w3.org/1999/xlink\">\n"
    xml << "  <pkg version=\"1.0\">\n"
    xml << "    <meta>\n"
    xml << "      <pkgname>libxml</pkgname>\n"
    xml << "    </meta>\n"
    xml << "  </pkg>\n"
    xml << "</rubynet>\n"
    
    @doc.root = XML::Node.new('rubynet')
    @doc.root['xmlns:xlink'] = "http://www.w3.org/1999/xlink"
    pkg = @doc.root.child = XML::Node.new('pkg')
    pkg['version'] = '1.0'
    meta = pkg.child = XML::Node.new('meta')
    pkgname = meta.child = XML::Node.new('pkgname', 'libxml')
    assert_equal(xml, @doc.to_s)
  end
end
