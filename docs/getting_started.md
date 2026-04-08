# Getting Started

## Requiring the Library

```ruby
require 'libxml-ruby'
document = LibXML::XML::Document.new
```

If you prefer shorter names, include the LibXML module in your own namespace:

```ruby
require 'libxml-ruby'

module MyApp
  include LibXML

  class Processor
    def parse(file)
      XML::Document.file(file)
    end
  end
end
```

## Choosing a Parser

libxml-ruby provides four parsers, each suited to different use cases:

| Parser | Best For |
|--------|----------|
| `XML::Parser` | General-purpose DOM parsing. Loads the entire document into a tree. |
| `XML::Reader` | Large documents that don't fit in memory. Pull-based streaming API. |
| `XML::HTMLParser` | Parsing HTML documents (tolerates malformed markup). |
| `XML::SaxParser` | Event-driven parsing with callbacks. |

## Data Sources

All parsers support multiple data sources:

```ruby
# From a file
doc = XML::Parser.file('data.xml').parse

# From a string
doc = XML::Parser.string('<root/>').parse

# From an IO object
File.open('data.xml') do |io|
  doc = XML::Parser.io(io).parse
end
```

## A Complete Example

```ruby
require 'libxml-ruby'

# Parse
doc = LibXML::XML::Document.file('books.xml')

# Navigate
root = doc.root
puts root.name

# Find nodes with XPath
doc.find('//book[@year > 2000]').each do |book|
  title = book.find_first('title').content
  puts title
end

# Create new content
new_book = LibXML::XML::Node.new('book')
new_book['year'] = '2024'
new_book << LibXML::XML::Node.new('title', 'New Book')
root << new_book

# Save
doc.save('books_updated.xml', indent: true)
```
