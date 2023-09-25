# encoding: UTF-8

require_relative './test_helper'

class TestSchema < Minitest::Test
  Import_NS = 'http://xml4r.org/libxml-ruby/test/shiporder'.freeze

  def setup
    file = File.join(File.dirname(__FILE__), 'model/shiporder.xml')
    @doc = LibXML::XML::Document.file(file)
    schema_file = File.join(File.dirname(__FILE__), 'model/shiporder.xsd')
    schema_doc = LibXML::XML::Document.file(schema_file)
    @schema = LibXML::XML::Schema.document(schema_doc)
  end

  def teardown
    @doc = nil
    @schema = nil
  end

  def check_error(error)
    refute_nil(error)
    assert(error.message.match(/Error: Element 'invalid': This element is not expected. Expected is \( item \)/))
    assert_kind_of(LibXML::XML::Error, error)
    assert_equal(LibXML::XML::Error::SCHEMASV, error.domain)
    assert_equal(LibXML::XML::Error::SCHEMAV_ELEMENT_CONTENT, error.code)
    assert_equal(LibXML::XML::Error::ERROR, error.level)
    assert(error.file.match(/shiporder.xml/)) if error.file
    assert_nil(error.str1)
    assert_nil(error.str2)
    assert_nil(error.str3)
    assert_equal(0, error.int1)
    assert_equal(0, error.int2)
  end

  def test_load_from_doc
    assert_instance_of(LibXML::XML::Schema, @schema)
  end

  def test_schema_load_from_uri
    xlink_schema = LibXML::XML::Schema.new('http://www.w3.org/1999/xlink.xsd')
    assert_instance_of(LibXML::XML::Schema, xlink_schema)
    assert_instance_of(LibXML::XML::Schema::Element, xlink_schema.elements['title'])
    assert_instance_of(LibXML::XML::Schema::Type, xlink_schema.types['titleEltType'])
    assert_equal('http://www.w3.org/1999/xlink', xlink_schema.target_namespace)
  end

  def test_schema_from_string
    schema_file = File.join(File.dirname(__FILE__), 'model/shiporder.xsd')
    schema_from_str = LibXML::XML::Schema.from_string(File.read(schema_file))
    assert_instance_of(LibXML::XML::Schema, schema_from_str)
  end

  def test_invalid_schema_doc
    bad_schema_file = File.join(File.dirname(__FILE__), 'model/shiporder_bad.xsd')
    bad_schema_doc = LibXML::XML::Document.file(bad_schema_file)
    bad_schema = nil
    ## Note: this type of error throws
    begin
      bad_schema = LibXML::XML::Schema.document(bad_schema_doc)
    rescue LibXML::XML::Error => error
      bad_schema = error
      assert(error.message.match(/Error: Element '.*': The content is not valid. Expected is \(/))
      assert_kind_of(LibXML::XML::Error, error)
      assert_equal(LibXML::XML::Error::SCHEMASP, error.domain)
      assert_equal(LibXML::XML::Error::SCHEMAP_S4S_ELEM_NOT_ALLOWED, error.code)
      assert_equal(LibXML::XML::Error::ERROR, error.level)
      assert(error.file.match(/shiporder_bad.xsd/))
      assert_equal("Element '{http://www.w3.org/2001/XMLSchema}complexType'", error.str1)
      refute_nil(error.str2)
      assert_nil(error.str3)
      assert_equal(0, error.int1)
      assert_equal(0, error.int2)
    end
    ## Last but not least, make sure we threw an error
    assert_instance_of(LibXML::XML::Error, bad_schema)
  end

  def test_doc_valid
    assert(@doc.validate_schema(@schema))
  end

  def test_doc_invalid
    new_node = LibXML::XML::Node.new('invalid', 'this will mess up validation')
    @doc.root << new_node

    error = assert_raises(LibXML::XML::Error) do
      @doc.validate_schema(@schema)
    end

    check_error(error)
    assert_nil(error.line)
    refute_nil(error.node)
    assert_equal('invalid', error.node.name)
  end

  def test_reader_valid
    reader = LibXML::XML::Reader.string(@doc.to_s)
    assert(reader.schema_validate(@schema))

    while reader.read
    end
  end

  def test_reader_invalid
    # Set error handler
    errors = Array.new
    LibXML::XML::Error.set_handler do |error|
      errors << error
    end

    new_node = LibXML::XML::Node.new('invalid', 'this will mess up validation')
    @doc.root << new_node
    reader = LibXML::XML::Reader.string(@doc.to_s)

    # Set a schema
    assert(reader.schema_validate(@schema))

    while reader.read
    end

    assert_equal(1, errors.length)

    error = errors.first
    check_error(error)
    assert_equal(21, error.line)
  ensure
    LibXML::XML::Error.set_handler(&LibXML::XML::Error::VERBOSE_HANDLER)
  end


  # Schema meta-data tests
  def test_elements
    assert_instance_of(Hash, @schema.elements)
    assert_equal(1, @schema.elements.length)
    assert_instance_of(LibXML::XML::Schema::Element, @schema.elements['shiporder'])
  end

  def test_types
    assert_instance_of(Hash, @schema.types)
    assert_equal(1, @schema.types.length)
    assert_instance_of(LibXML::XML::Schema::Type, @schema.types['shiporderType'])
  end

  def test_imported_types
    assert_instance_of(Hash, @schema.imported_types)
    assert_equal(2, @schema.imported_types.length)
    assert_instance_of(LibXML::XML::Schema::Type, @schema.imported_types['shiporderType'])
    assert_instance_of(LibXML::XML::Schema::Type, @schema.imported_types['shiporderFooType'])
  end

  def test_imported_ns_types
    assert_instance_of(Hash, @schema.imported_ns_types)
    assert_equal(2, @schema.imported_ns_types.length)
    assert_equal(1, @schema.imported_ns_types[nil].length)
    assert_equal(1, @schema.imported_ns_types[Import_NS].length)
    assert_instance_of(LibXML::XML::Schema::Type, @schema.imported_ns_types[nil]['shiporderType'])
    assert_instance_of(LibXML::XML::Schema::Type, @schema.imported_ns_types[Import_NS]['shiporderFooType'])
  end

  def test_imported_ns_elements
    assert_instance_of(Hash, @schema.imported_ns_elements)
    assert_equal(2, @schema.imported_ns_elements.length)
    assert_equal(1, @schema.imported_ns_elements[nil].length)
    assert_equal(1, @schema.imported_ns_elements[Import_NS].length)
    assert_instance_of(LibXML::XML::Schema::Element, @schema.imported_ns_elements[nil]['shiporder'])
    assert_instance_of(LibXML::XML::Schema::Element, @schema.imported_ns_elements[Import_NS]['shiporder'])
    assert_equal('shiporderType', @schema.imported_ns_elements[nil]['shiporder'].type.name)
    assert_equal('shiporderFooType', @schema.imported_ns_elements[Import_NS]['shiporder'].type.name)
  end

  def test_namespaces
    assert_instance_of(Array, @schema.namespaces)
    ## For some reason, when importing another schema we end up with two xs->http://www.w3.org/2001/XMLSchema namespaces.
    ## So, we expect 3 here (one for Import_NS and then two for the schema namespace.
    assert_equal(3, @schema.namespaces.length)
  end

  def test_schema_type
    type = @schema.types['shiporderType']

    assert_equal('shiporderType', type.name)
    assert_nil(type.namespace)
    assert_equal("Shiporder type documentation", type.annotation)
    assert_instance_of(LibXML::XML::Node, type.node)
    assert_equal(LibXML::XML::Schema::Types::XML_SCHEMA_TYPE_COMPLEX, type.kind)
    assert_instance_of(LibXML::XML::Schema::Type, type.base)
    assert_equal("anyType", type.base.name)
    assert_equal(LibXML::XML::Schema::Types::XML_SCHEMA_TYPE_BASIC, type.base.kind)

    assert_instance_of(Hash, type.elements)
    assert_equal(3, type.elements.length)
  end

  def test_schema_element
    element = @schema.types['shiporderType'].elements['orderperson']

    assert_equal('orderperson', element.name)
    assert_nil(element.namespace)
    assert_equal("orderperson element documentation", element.annotation)
    assert_equal(false, element.array?)
    assert_equal(true, element.required?)

    element = @schema.types['shiporderType'].elements['item']
    assert_equal('item', element.name)
    assert_equal(true, element.array?)
    assert_equal(true, element.required?)

    element = @schema.types['shiporderType'].elements['item'].type.elements['note']
    assert_equal('note', element.name)
    assert_equal('string', element.type.name)
    assert_equal(false, element.array?)
    assert_equal(false, element.required?)
  end

  def test_schema_attributes
    type = @schema.types['shiporderType']

    assert_instance_of(Array, type.attributes)
    assert_equal(2, type.attributes.length)
    assert_instance_of(LibXML::XML::Schema::Attribute, type.attributes.first)
  end

  def test_schema_attribute
    attribute = @schema.types['shiporderType'].attributes.first

    assert_equal("orderid", attribute.name)
    assert_nil(attribute.namespace)
    assert_equal(1, attribute.occurs)
    assert_equal('string', attribute.type.name)

    attribute = @schema.types['shiporderType'].attributes[1]
    assert_equal(2, attribute.occurs)
    assert_equal('1', attribute.default)
    assert_equal('integer', attribute.type.name)
  end
end
