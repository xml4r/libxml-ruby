# RelaxNG Validation

RelaxNG is a schema language for XML that is simpler and more expressive than XML Schema for many use cases.

## Loading a Schema

```ruby
# From a file
schema = XML::RelaxNG.new('/path/to/schema.rng')

# From a document
schema_doc = XML::Document.file('schema.rng')
schema = XML::RelaxNG.document(schema_doc)
```

## Validating

```ruby
doc = XML::Parser.file('data.xml').parse

begin
  doc.validate_relaxng(schema)
rescue XML::Error => e
  puts e.message
end
```

The `validate_relaxng` method returns `true` on success or raises `XML::Error` on failure.
