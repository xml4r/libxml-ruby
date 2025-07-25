# encoding: UTF-8

require_relative './test_helper'

# Code  UTF8        Latin1      Hex
# m      109          109        6D
# ö      195 182      246        C3 B6 / F6
# t      116          116        74
# l      108          108        6C
# e      101          101        65
# y      121          121        79
# _       95           95        5F
# c       99           99        63
# r      114          114        72
# ü      195 188      252        C3 BC / FC
# e      101          101        65

# See:
#  http://en.wikipedia.org/wiki/ISO/IEC_8859-1
#  http://en.wikipedia.org/wiki/List_of_Unicode_characters

class TestEncoding < Minitest::Test
  def setup
    @original_encoding = Encoding.default_internal
    Encoding.default_internal = nil
  end

  def teardown
    Encoding.default_internal = @original_encoding
  end

  def file_for_encoding(encoding)
    file_name = "model/bands.#{encoding.name.downcase}.xml"
    File.join(File.dirname(__FILE__), file_name)
  end

  def load_encoding(encoding)
    @encoding = encoding
    file = file_for_encoding(encoding)

    @doc = LibXML::XML::Document.file(file, options: LibXML::XML::Parser::Options::NOBLANKS)
  end

  def test_encoding
    doc = LibXML::XML::Document.new
    assert_equal(LibXML::XML::Encoding::NONE, doc.encoding)
    assert_equal(Encoding::ASCII_8BIT, doc.rb_encoding)

    file = File.expand_path(File.join(File.dirname(__FILE__), 'model/bands.xml'))
    doc = LibXML::XML::Document.file(file)
    assert_equal(LibXML::XML::Encoding::UTF_8, doc.encoding)
    assert_equal(Encoding::UTF_8, doc.rb_encoding)

    doc.encoding = LibXML::XML::Encoding::ISO_8859_1
    assert_equal(LibXML::XML::Encoding::ISO_8859_1, doc.encoding)
    assert_equal(Encoding::ISO8859_1, doc.rb_encoding)
  end

  def test_no_internal_encoding_iso_8859_1
    load_encoding(Encoding::ISO_8859_1)
    node = @doc.root.children.first

    name = node.name
    assert_equal(Encoding::UTF_8, name.encoding)
    assert_equal("m\u00F6tley_cr\u00FCe", name)
    assert_equal("109 195 182 116 108 101 121 95 99 114 195 188 101",
                 name.bytes.to_a.join(" "))
    assert_equal("M\u00F6tley Cr\u00FCe is an American heavy metal band formed in Los Angeles, California in 1981.",
                 node.content)

    name = name.encode(Encoding::ISO_8859_1)
    assert_equal(Encoding::ISO_8859_1, name.encoding)
    assert_equal("m\xF6tley_cr\xFCe".force_encoding(Encoding::ISO_8859_1), name)
    assert_equal("109 246 116 108 101 121 95 99 114 252 101",
                 name.bytes.to_a.join(" "))
    assert_equal("M\xF6tley Cr\xFCe is an American heavy metal band formed in Los Angeles, California in 1981.".force_encoding(Encoding::ISO_8859_1),
                node.content.encode(Encoding::ISO_8859_1))
  end

  def test_internal_encoding_iso_8859_1
    Encoding.default_internal = Encoding::ISO_8859_1
    load_encoding(Encoding::ISO_8859_1)
    node = @doc.root.children.first

    name = node.name
    assert_equal(Encoding::ISO_8859_1, name.encoding)
    assert_equal("109 246 116 108 101 121 95 99 114 252 101",
                 name.bytes.to_a.join(" "))
    assert_equal("m\xF6tley_cr\xFCe".force_encoding(Encoding::ISO_8859_1), name)
    assert_equal("109 246 116 108 101 121 95 99 114 252 101",
                 name.bytes.to_a.join(" "))
    assert_equal("M\xF6tley Cr\xFCe is an American heavy metal band formed in Los Angeles, California in 1981.".force_encoding(Encoding::ISO_8859_1),
                node.content.encode(Encoding::ISO_8859_1))
  end

  def test_no_internal_encoding_utf_8
    load_encoding(Encoding::UTF_8)
    node = @doc.root.children.first

    name = node.name
    assert_equal(@encoding, name.encoding)
    assert_equal("109 195 182 116 108 101 121 95 99 114 195 188 101",
                 name.bytes.to_a.join(" "))

    name = name.encode(Encoding::ISO_8859_1)
    assert_equal(Encoding::ISO_8859_1, name.encoding)
    assert_equal("109 246 116 108 101 121 95 99 114 252 101",
                 name.bytes.to_a.join(" "))
  end

  def test_internal_encoding_utf_8
    Encoding.default_internal = Encoding::ISO_8859_1
    load_encoding(Encoding::UTF_8)
    node = @doc.root.children.first

    name = node.name
    assert_equal(Encoding::ISO_8859_1, name.encoding)
    assert_equal("109 246 116 108 101 121 95 99 114 252 101",
                 name.bytes.to_a.join(" "))
  end

  def test_encoding_conversions
    assert_equal("UTF-8", LibXML::XML::Encoding.to_s(LibXML::XML::Encoding::UTF_8))
    assert_equal(LibXML::XML::Encoding::UTF_8, LibXML::XML::Encoding.from_s("UTF-8"))
  end
end
