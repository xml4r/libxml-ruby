# Attributes

Attributes are name-value pairs on element nodes.

## Reading Attributes

```ruby
doc = XML::Parser.string('<book id="1" lang="en"/>').parse
node = doc.root

# Hash-style access
node['id']                  # => "1"
node['lang']                # => "en"

# Attribute object
attr = node.attributes.get_attribute('id')
attr.name                   # => "id"
attr.value                  # => "1"
```

## Setting Attributes

```ruby
node['category'] = 'fiction'
```

## Removing Attributes

```ruby
attr = node.attributes.get_attribute('lang')
attr.remove!
```

## Iterating

```ruby
node.attributes.each do |attr|
  puts "#{attr.name} = #{attr.value}"
end
```

## Namespaced Attributes

```ruby
attr = node.attributes.get_attribute_ns('http://www.w3.org/1999/xlink', 'href')
attr.value
```
