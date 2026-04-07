# DTD Validation

A Document Type Definition (DTD) defines the structure and allowed elements of an XML document.

## Creating a DTD

```ruby
# From a string
dtd = XML::Dtd.new(<<~DTD)
  <!ELEMENT root (item*) >
  <!ELEMENT item (#PCDATA) >
DTD

# From public and system identifiers
dtd = XML::Dtd.new(
  '-//W3C//DTD XHTML 1.0 Transitional//EN',
  'http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd'
)
```

## Validating

```ruby
doc = XML::Parser.file('data.xml').parse

begin
  doc.validate(dtd)
rescue XML::Error => e
  puts e.message
end
```

The `validate` method returns `true` on success or raises `XML::Error` with details about the validation failure.
