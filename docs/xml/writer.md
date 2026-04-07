# Writing XML

The `XML::Writer` class provides a streaming API for generating XML. It writes XML content sequentially without building a DOM tree, making it memory efficient for large output.

## Output Targets

```ruby
# Write to a file
writer = XML::Writer.file('output.xml')

# Write to a string
writer = XML::Writer.string

# Write to an IO object
writer = XML::Writer.io(io_object)

# Write to an in-memory document
writer = XML::Writer.document
```

## Generating XML

```ruby
writer = XML::Writer.string

writer.start_document
writer.start_element('catalog')

writer.start_element('book')
writer.write_attribute('id', '1')
writer.write_element('title', 'Ruby Programming')
writer.write_element('author', 'Dave Thomas')
writer.end_element  # </book>

writer.end_element  # </catalog>
writer.end_document

puts writer.result
```

Output:

```xml
<?xml version="1.0"?>
<catalog>
  <book id="1">
    <title>Ruby Programming</title>
    <author>Dave Thomas</author>
  </book>
</catalog>
```

## Indentation

Enable indentation for readable output:

```ruby
writer.set_indent(true)
writer.set_indent_string('  ')
```

## CDATA and Comments

```ruby
writer.write_cdata('raw <content>')
writer.write_comment('a comment')
```

## Document Writer

`XML::Writer.document` returns a writer that builds an `XML::Document`:

```ruby
writer = XML::Writer.document
writer.start_document
writer.start_element('root')
writer.end_element
writer.end_document

doc = writer.result   # => XML::Document
```
