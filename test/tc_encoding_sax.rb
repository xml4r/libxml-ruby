# encoding: UTF-8
require './test_helper'

class SaxEncodingCallbacks
  attr_reader :encoding
  attr_accessor :assertions
  include Minitest::Assertions

  def initialize
    @assertions = 0
    @encoding = Encoding::UTF_8
  end

  # Called for a CDATA block event.
  def on_cdata_block(cdata)
    assert_equal(self.encoding, cdata.encoding)
  end

  # Called for a characters event.
  def on_characters(chars)
    assert_equal(self.encoding, chars.encoding)
  end

  # Called for a comment event.
  def on_comment(msg)
    assert_equal(self.encoding, msg.encoding)
  end

  # Called for a end document event.
  def on_end_document
  end

  # Called for a end element event.
  def on_end_element_ns(name, prefix, uri)
    assert_equal(self.encoding, name.encoding)
    assert_equal(self.encoding, prefix.encoding) if prefix
    assert_equal(self.encoding, uri.encoding) if uri
  end

  # Called for parser errors.
  def on_error(msg)
    assert_equal(self.encoding, msg.encoding)
  end

  # Called for an external subset event.
  def on_external_subset(name, external_id, system_id)
    assert_equal(self.encoding, name.encoding)
    assert_equal(self.encoding, external_id.encoding)
    assert_equal(self.encoding, system_id.encoding)
  end

  # Called for an external subset notification event.
  def on_has_external_subset
  end

  # Called for an internal subset notification event.
  def on_has_internal_subset
  end

  # Called for an internal subset event.
  def on_internal_subset(name, external_id, system_id)
    assert_equal(self.encoding, name.encoding)
    assert_equal(self.encoding, external_id.encoding)
    assert_equal(self.encoding, system_id.encoding)
  end

  # Called for 'is standalone' event.
  def on_is_standalone
  end

  # Called for an processing instruction event.
  def on_processing_instruction(target, data)
    assert_equal(self.encoding, target.encoding)
    assert_equal(self.encoding, data.encoding)
  end

  # Called for a reference event.
  def on_reference(name)
    assert_equal(self.encoding, name.encoding)
  end

  # Called for a start document event.
  def on_start_document
  end

  # Called for a start element event.
  def on_start_element_ns(name, attributes, prefix, uri, namespaces)
    assert_equal(self.encoding, name.encoding) if name
    assert_equal(self.encoding, prefix.encoding) if prefix
    assert_equal(self.encoding, uri.encoding) if uri
  end
end

class TestEncodingSax < Minitest::Test
  def setup
    Encoding.default_internal = nil
  end

  def file_for_encoding(encoding)
    file_name = "model/bands.#{encoding.name.downcase}.xml"
    File.join(File.dirname(__FILE__), file_name)
  end

  def test_encoding_iso_8859_1
    parser = XML::SaxParser.file(file_for_encoding(Encoding::ISO_8859_1))
    parser.callbacks = SaxEncodingCallbacks.new
    parser.parse
  end

  def test_encoding_utf8
    parser = XML::SaxParser.file(file_for_encoding(Encoding::UTF_8))
    parser.callbacks = SaxEncodingCallbacks.new
    parser.parse
  end
end