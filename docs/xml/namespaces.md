# Namespaces

XML namespaces prevent naming conflicts by qualifying element and attribute names with a URI.

## Reading Namespaces

```ruby
doc = XML::Parser.string('<root xmlns:dc="http://purl.org/dc/elements/1.1/"/>').parse
node = doc.root

node.namespaces.each do |ns|
  puts "#{ns.prefix}: #{ns.href}"
end
```

## Namespace Properties

```ruby
ns = node.namespaces.first
ns.prefix     # => "dc" (nil for default namespace)
ns.href       # => "http://purl.org/dc/elements/1.1/"
```

## Default Namespace

```ruby
doc = XML::Parser.string('<root xmlns="http://example.com"/>').parse
ns = doc.root.namespaces.default
ns.href       # => "http://example.com"
```

## XPath with Namespaces

Default namespaces require a prefix when using XPath. See [XPath](xpath.md) for details.

```ruby
doc.find('//ns:element', 'ns:http://example.com')
```

## Setting a Default Prefix

To simplify XPath queries on documents with a default namespace:

```ruby
doc.root.namespaces.default_prefix = 'ns'
doc.find('//ns:element')
```
