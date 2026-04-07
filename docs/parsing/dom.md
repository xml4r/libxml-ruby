# DOM Parser

The DOM parser (`XML::Parser`) loads an entire XML document into memory as a tree of nodes. This is the most common way to work with XML in libxml-ruby.

## Parsing

```ruby
# From a file
doc = XML::Parser.file('books.xml').parse

# From a string
doc = XML::Parser.string('<root><item/></root>').parse

# From an IO
File.open('books.xml') do |io|
  doc = XML::Parser.io(io).parse
end
```

## Example: Parse and Extract Data

```ruby
xml = <<~XML
  <library>
    <book id="1" available="true">
      <title>The Pragmatic Programmer</title>
      <author>Dave Thomas</author>
      <year>1999</year>
    </book>
    <book id="2" available="false">
      <title>Design Patterns</title>
      <author>Gang of Four</author>
      <year>1994</year>
    </book>
  </library>
XML

doc = XML::Parser.string(xml).parse

# Access the root
root = doc.root
puts root.name  # => "library"

# Iterate over children
root.each do |book|
  next unless book.element?
  puts book.find_first('title').content
end

# Use XPath
available = doc.find('//book[@available="true"]')
available.each do |book|
  title = book.find_first('title').content
  year = book.find_first('year').content
  puts "#{title} (#{year})"
end
```

## Example: Parse a Configuration File

```ruby
doc = XML::Parser.file('config.xml').parse

db_host = doc.find_first('//database/host').content
db_port = doc.find_first('//database/port').content.to_i
db_name = doc.find_first('//database/name').content

puts "Connecting to #{db_name} at #{db_host}:#{db_port}"
```

## Example: Parse with Options

```ruby
# Strip whitespace-only text nodes and disable network access
parser = XML::Parser.file('data.xml')
parser.options = XML::Parser::Options::NOBLANKS | XML::Parser::Options::NONET
doc = parser.parse
```

## Example: Parse from a Web Response

```ruby
require 'net/http'

uri = URI('https://example.com/api/data.xml')
xml = Net::HTTP.get(uri)

doc = XML::Parser.string(xml).parse
doc.find('//item').each do |item|
  puts item.find_first('name').content
end
```

## Error Handling

```ruby
begin
  doc = XML::Parser.string('<broken').parse
rescue XML::Error => e
  puts "Parse failed: #{e.message}"
end
```

See [Error Handling](../xml/errors.md) for details on error properties and custom handlers.
