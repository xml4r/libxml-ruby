# Reader

The `XML::Reader` provides a pull-based streaming API for reading XML. It acts as a cursor moving forward through the document, stopping at each node. This is more memory efficient than DOM parsing for large documents.

## Basic Usage

```ruby
reader = XML::Reader.file('large.xml')

while reader.read
  if reader.node_type == XML::Reader::TYPE_ELEMENT
    puts reader.name
  end
end
```

## Node Properties

At each position, the reader exposes the current node's properties:

```ruby
reader.name              # node name
reader.value             # node value (for text, attributes)
reader.node_type         # node type constant
reader.depth             # nesting depth
reader.empty_element?    # self-closing element?
reader.has_attributes?   # has attributes?
reader.local_name        # local name (without prefix)
reader.namespace_uri     # namespace URI
reader.prefix            # namespace prefix
```

## Node Type Constants

```ruby
XML::Reader::TYPE_ELEMENT         # opening tag
XML::Reader::TYPE_END_ELEMENT     # closing tag
XML::Reader::TYPE_TEXT            # text content
XML::Reader::TYPE_CDATA           # CDATA section
XML::Reader::TYPE_COMMENT         # comment
XML::Reader::TYPE_SIGNIFICANT_WHITESPACE
```

## Reading Attributes

```ruby
reader = XML::Reader.string('<book id="1" title="Ruby"/>')
reader.read

reader['id']                    # => "1"
reader.get_attribute('title')   # => "Ruby"
reader.attribute_count          # => 2

# Walk attributes
reader.move_to_first_attribute
puts "#{reader.name}=#{reader.value}"
while reader.move_to_next_attribute
  puts "#{reader.name}=#{reader.value}"
end
reader.move_to_element  # move back to the element
```

## Example: Extract Data from a Large File

```ruby
reader = XML::Reader.file('products.xml')

products = []
while reader.read
  if reader.node_type == XML::Reader::TYPE_ELEMENT && reader.name == 'product'
    product = {}
    product['id'] = reader['id']

    # Read child elements
    while reader.read
      break if reader.node_type == XML::Reader::TYPE_END_ELEMENT && reader.name == 'product'

      if reader.node_type == XML::Reader::TYPE_ELEMENT
        name = reader.name
        reader.read  # move to text content
        product[name] = reader.value if reader.has_value?
      end
    end

    products << product
  end
end

products.each { |p| puts "#{p['name']}: $#{p['price']}" }
```

## Example: Count Elements

```ruby
reader = XML::Reader.file('data.xml')
counts = Hash.new(0)

while reader.read
  if reader.node_type == XML::Reader::TYPE_ELEMENT
    counts[reader.name] += 1
  end
end

counts.sort_by { |_, v| -v }.each do |name, count|
  puts "#{name}: #{count}"
end
```

## Navigating with next

`reader.read` descends into child nodes. Use `reader.next` to skip to the next sibling, skipping the current node's subtree:

```ruby
reader = XML::Reader.file('data.xml')

while reader.read
  if reader.node_type == XML::Reader::TYPE_ELEMENT && reader.name == 'skip_me'
    reader.next  # skip this element and its children
  end
end
```

## Expanding Nodes

You can expand the current node into a full DOM subtree for detailed inspection:

```ruby
reader = XML::Reader.file('books.xml')
while reader.read
  if reader.name == 'book' && reader.node_type == XML::Reader::TYPE_ELEMENT
    node = reader.expand

    # Use XPath on the expanded node (requires reader.doc first)
    reader.doc
    title = node.find_first('title').content
    puts title
  end
end
```

!!! warning
    Expanded nodes are only valid until the next `reader.read` call. Do not store references to them.

## Validation While Reading

The reader can validate against a schema as it reads:

```ruby
reader = XML::Reader.file('data.xml')
reader.schema_validate('schema.xsd')

while reader.read
  # reader.valid? returns the validation state
end
```

Or with RelaxNG:

```ruby
reader = XML::Reader.file('data.xml')
reader.relax_ng_validate('schema.rng')

while reader.read
  # process nodes
end
```
