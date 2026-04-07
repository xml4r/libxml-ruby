# XML Schema Validation

XML Schema (XSD) is the W3C standard for defining the structure, content, and data types of XML documents.

## Loading a Schema

```ruby
# From a file
schema = XML::Schema.new('/path/to/schema.xsd')

# From a document
schema_doc = XML::Document.file('schema.xsd')
schema = XML::Schema.document(schema_doc)
```

## Validating

```ruby
doc = XML::Parser.file('data.xml').parse

begin
  doc.validate_schema(schema)
rescue XML::Error => e
  puts e.message
end
```

## Inspecting the Schema

```ruby
schema.target_namespace

schema.types.each do |name, type|
  puts "Type: #{name}"
end

schema.elements.each do |name, element|
  puts "Element: #{name}"
end
```
