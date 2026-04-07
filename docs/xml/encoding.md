# Encoding

libxml-ruby handles character encoding conversion between libxml2 (which works in UTF-8 internally) and Ruby's encoding system.

## Document Encoding

```ruby
# Set encoding when creating a document
doc = XML::Document.new
doc.encoding = XML::Encoding::UTF_8

# Read encoding from a parsed document
doc = XML::Parser.file('data.xml').parse
doc.encoding   # => XML::Encoding::UTF_8
```

## Input Encoding

Specify the encoding when parsing if it's not declared in the document:

```ruby
parser = XML::Parser.file('data.xml', encoding: XML::Encoding::ISO_8859_1)
doc = parser.parse
```

## String Encoding

Strings returned by libxml-ruby carry the appropriate Ruby encoding based on the document's encoding. Internal conversion is handled automatically.

## Available Encodings

Encoding constants are defined on `XML::Encoding`:

```ruby
XML::Encoding::UTF_8
XML::Encoding::UTF_16LE
XML::Encoding::UTF_16BE
XML::Encoding::ISO_8859_1
XML::Encoding::ASCII
```

Use `XML::Encoding.to_s` to convert a constant to its string name, or `XML::Encoding.from_s` to look up a constant by name.
