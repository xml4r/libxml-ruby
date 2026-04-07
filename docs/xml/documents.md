# Documents

An `XML::Document` represents an entire XML document. It holds the root node, encoding, version, and other document-level properties.

## Creating Documents

```ruby
doc = XML::Document.new
doc.root = XML::Node.new('catalog')
doc.encoding = XML::Encoding::UTF_8
```

## Document Properties

```ruby
doc = XML::Parser.file('data.xml').parse

doc.root              # root element node
doc.encoding          # document encoding
doc.version           # XML version ("1.0")
doc.url               # source URL/filename
doc.node_type         # XML::Node::DOCUMENT_NODE
```

## Saving

```ruby
# To a file
doc.save('output.xml')
doc.save('output.xml', indent: true, encoding: XML::Encoding::UTF_8)

# To a string
xml_string = doc.to_s
xml_string = doc.to_s(indent: true)
```

## Example: Parse, Modify, Save

```ruby
doc = XML::Parser.file('config.xml').parse

# Update a value
node = doc.find_first('//setting[@name="timeout"]')
node.content = '30'

# Add a new setting
new_setting = XML::Node.new('setting', '100')
new_setting['name'] = 'max_retries'
doc.root << new_setting

# Save
doc.save('config.xml', indent: true, encoding: XML::Encoding::UTF_8)
```

## Example: Create an XML Document

```ruby
doc = XML::Document.new
doc.root = XML::Node.new('bookstore')

book = XML::Node.new('book')
book['category'] = 'fiction'

title = XML::Node.new('title', 'The Great Gatsby')
title['lang'] = 'en'
book << title

book << XML::Node.new('author', 'F. Scott Fitzgerald')
book << XML::Node.new('year', '1925')
book << XML::Node.new('price', '10.99')

doc.root << book

puts doc.to_s(indent: true)
```

Output:

```xml
<?xml version="1.0" encoding="UTF-8"?>
<bookstore>
  <book category="fiction">
    <title lang="en">The Great Gatsby</title>
    <author>F. Scott Fitzgerald</author>
    <year>1925</year>
    <price>10.99</price>
  </book>
</bookstore>
```

## Example: Import a Node from Another Document

```ruby
source = XML::Parser.file('source.xml').parse
target = XML::Parser.file('target.xml').parse

# Copy a node from source into target
node = source.find_first('//item[@id="special"]')
imported = target.import(node)
target.root << imported
```

## Canonicalization

```ruby
# C14N canonical form
canonical = doc.canonicalize
```

## XInclude

Process XInclude directives in a document:

```ruby
doc.xinclude
```

## Validation

Documents can be validated against DTDs, RelaxNG schemas, and XML Schemas. See the [Validation](../validation/dtd.md) section.
