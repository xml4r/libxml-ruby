# Nodes

Nodes are the primary objects in an XML document tree. The `XML::Node` class represents elements, text, CDATA, comments, and other node types.

## Node Types

```ruby
XML::Node::ELEMENT_NODE       # <element>
XML::Node::ATTRIBUTE_NODE     # attribute="value"
XML::Node::TEXT_NODE           # text content
XML::Node::CDATA_SECTION_NODE # <![CDATA[...]]>
XML::Node::COMMENT_NODE       # <!-- comment -->
XML::Node::DOCUMENT_NODE      # the document itself
XML::Node::PI_NODE            # <?target data?>
```

## Creating Nodes

```ruby
# Element
node = XML::Node.new('book')

# Element with text content
node = XML::Node.new('title', 'Ruby Programming')

# Special node types
cdata = XML::Node.new_cdata('raw content')
comment = XML::Node.new_comment('a comment')
pi = XML::Node.new_pi('xml-stylesheet', 'type="text/xsl" href="style.xsl"')
```

## Reading Node Properties

```ruby
node.name        # element name
node.content     # text content (recursively)
node.node_type   # node type constant
node.empty?      # no children?
node.doc         # owning document
node.line_num    # line number in source
node.path        # XPath path to this node
node.lang        # xml:lang value
node.base_uri    # base URI
```

## Traversal

```ruby
node.parent      # parent node
node.child       # first child (alias: first)
node.last        # last child
node.next        # next sibling
node.prev        # previous sibling

# Iterate over direct children
node.each { |child| puts child.name }
```

### Example: Walk a Document

```ruby
doc = XML::Parser.string(<<~XML).parse
  <catalog>
    <book id="1"><title>Ruby</title><price>29.99</price></book>
    <book id="2"><title>Rails</title><price>39.99</price></book>
  </catalog>
XML

doc.root.each do |book|
  next unless book.element?
  title = book.find_first('title').content
  price = book.find_first('price').content
  puts "#{title}: $#{price}"
end
```

## Modifying Content

### Changing Text

```ruby
node = doc.find_first('//title')
node.content = 'New Title'
```

!!! note
    Setting `content=` replaces all child nodes with a single text node. The value is automatically XML-escaped.

### Renaming Elements

```ruby
node.name = 'new_name'
```

### Setting Attributes

```ruby
node['category'] = 'fiction'
node['id'] = '42'
```

## Adding Nodes

### Append a Child

```ruby
parent << XML::Node.new('child')
parent << XML::Node.new('item', 'text content')
parent << 'plain text'  # adds a text node
```

### Insert Before/After

```ruby
# Insert after a node
node.sibling = XML::Node.new('after_me')

# Insert using next=/prev=
reference_node.next = XML::Node.new('after_reference')
reference_node.prev = XML::Node.new('before_reference')
```

### Example: Build a Document from Scratch

```ruby
doc = XML::Document.new
doc.root = XML::Node.new('catalog')

['Ruby', 'Rails', 'Sinatra'].each_with_index do |title, i|
  book = XML::Node.new('book')
  book['id'] = (i + 1).to_s
  book << XML::Node.new('title', title)
  book << XML::Node.new('in_stock', 'true')
  doc.root << book
end

puts doc.to_s(indent: true)
```

### Example: Add Elements from Data

```ruby
data = [
  { name: 'Alice', role: 'admin' },
  { name: 'Bob', role: 'user' },
]

doc = XML::Document.new
doc.root = XML::Node.new('users')

data.each do |user|
  node = XML::Node.new('user')
  node['role'] = user[:role]
  node << XML::Node.new('name', user[:name])
  doc.root << node
end
```

## Removing Nodes

```ruby
node.remove!
```

`remove!` detaches the node from its parent and document. The node becomes a detached root — you can reattach it elsewhere or let it be garbage collected.

### Example: Remove Matching Nodes

```ruby
doc.find('//item[@deprecated="true"]').each do |node|
  node.remove!
end
```

## Moving Nodes

To move a node, remove it and reattach it:

```ruby
node = doc.find_first('//footnote')
node.remove!
doc.find_first('//appendix') << node
```

## Copying Nodes

```ruby
shallow = node.copy(false)  # element only, no children
deep = node.copy(true)      # element and all descendants
```

### Example: Duplicate a Subtree

```ruby
template = doc.find_first('//item[@id="template"]')
copy = template.copy(true)
copy['id'] = 'new_item'
template.parent << copy
```

## Detached Nodes

A newly created node is not attached to any document — it is a *detached root node* owned by Ruby. When you insert it into a document tree, ownership transfers to the document. Removing it transfers ownership back to Ruby.

```ruby
node = XML::Node.new('item')   # detached, Ruby owns it
doc.root << node                # attached, document owns it
node.remove!                    # detached again, Ruby owns it
```

If a detached node goes out of scope and is garbage collected, Ruby frees it (and all its children). Once attached, the document handles cleanup.

## Comparing Nodes

```ruby
# Same underlying libxml node?
node1.eql?(node2)
node1 == node2
```

!!! note
    Multiple Ruby objects can wrap the same underlying XML node. Use `eql?` or `==` to compare nodes, not `equal?`.

## Serialization

```ruby
node.to_s                    # XML string
node.to_s(indent: true)     # indented XML string
```
