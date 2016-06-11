# encoding: UTF-8

require './test_helper'

class TestSchema < Minitest::Test
  def setup
    file = File.join(File.dirname(__FILE__), 'model/shiporder.xml')
    @doc = XML::Document.file(file)
  end

  def teardown
    @doc = nil
  end

  def schema
    document = XML::Document.file(File.join(File.dirname(__FILE__), 'model/shiporder.xsd'))
    XML::Schema.document(document)
  end

  def check_error(error)
    refute_nil(error)
    assert(error.message.match(/Error: Element 'invalid': This element is not expected. Expected is \( item \)/))
    assert_kind_of(XML::Error, error)
    assert_equal(XML::Error::SCHEMASV, error.domain)
    assert_equal(XML::Error::SCHEMAV_ELEMENT_CONTENT, error.code)
    assert_equal(XML::Error::ERROR, error.level)
    assert(error.file.match(/shiporder.xml/)) if error.file
    assert_nil(error.str1)
    assert_nil(error.str2)
    assert_nil(error.str3)
    assert_equal(0, error.int1)
    assert_equal(0, error.int2)
  end

  def test_load_from_doc
    assert_instance_of(XML::Schema, schema)
  end

  def test_doc_valid
    assert(@doc.validate_schema(schema))
  end

  def test_doc_invalid
    new_node = XML::Node.new('invalid', 'this will mess up validation')
    @doc.root << new_node

    error = assert_raises(XML::Error) do
      @doc.validate_schema(schema)
    end

    check_error(error)
    assert_nil(error.line)
    refute_nil(error.node)
    assert_equal('invalid', error.node.name)
  end

  def test_reader_valid
    reader = XML::Reader.string(@doc.to_s)
    assert(reader.schema_validate(schema))

    while reader.read
    end
  end

  def test_reader_invalid
    # Set error handler
    errors = Array.new
    XML::Error.set_handler do |error|
      errors << error
    end

    new_node = XML::Node.new('invalid', 'this will mess up validation')
    @doc.root << new_node
    reader = XML::Reader.string(@doc.to_s)

    # Set a schema
    assert(reader.schema_validate(schema))

    while reader.read
    end

    assert_equal(1, errors.length)

    error = errors.first
    check_error(error)
    assert_equal(21, error.line)
  ensure
    XML::Error.set_handler(&LibXML::XML::Error::VERBOSE_HANDLER)
  end


  # Schema meta-data tests

  def test_elements
    assert_instance_of(Hash, schema.elements)
    assert_equal(1, schema.elements.length)
    assert_instance_of(XML::Schema::Element, schema.elements['shiporder'])
  end

  def test_types
    assert_instance_of(Hash, schema.types)
    assert_equal(1, schema.types.length)
    assert_instance_of(XML::Schema::Type, schema.types['shiporder'])
  end

  def test_schema_type
    type = schema.types['shiporder']

    assert_equal('shiporder', type.name)
    assert_equal(nil, type.namespace)
    assert_equal("Shiporder type documentation", type.annotation)
    assert_instance_of(XML::Node, type.node)
    assert_equal(XML::Schema::Types::XML_SCHEMA_TYPE_COMPLEX, type.kind)
    assert_instance_of(XML::Schema::Type, type.base)
    assert_equal("anyType", type.base.name)
    assert_equal(XML::Schema::Types::XML_SCHEMA_TYPE_BASIC, type.base.kind)

    assert_instance_of(Hash, type.elements)
    assert_equal(3, type.elements.length)
  end

  def test_schema_element
    element = schema.types['shiporder'].elements['orderperson']

    assert_equal('orderperson', element.name)
    assert_equal(nil, element.namespace)
    assert_equal("orderperson element documentation", element.annotation)
    assert_equal(1, element.min_occurs)
    assert_equal(1, element.max_occurs)


    element = schema.types['shiporder'].elements['item']
    assert_equal(Float::INFINITY, element.max_occurs)

    element = schema.types['shiporder'].elements['item'].type.elements['note']
    assert_equal(0, element.min_occurs)
    assert_equal('string', element.type.name)
  end

  def test_schema_attributes
    type = schema.types['shiporder']

    assert_instance_of(Array, type.attributes)
    assert_equal(2, type.attributes.length)
    assert_instance_of(XML::Schema::Attribute, type.attributes.first)
  end

  def test_schema_attribute
    attribute = schema.types['shiporder'].attributes.first

    assert_equal("orderid", attribute.name)
    assert_equal(nil, attribute.namespace)
    assert_equal(1, attribute.occurs)
    assert_equal('string', attribute.type.name)

    attribute = schema.types['shiporder'].attributes[1]
    assert_equal(2, attribute.occurs)
    assert_equal('1', attribute.default)
    assert_equal('integer', attribute.type.name)
  end
end