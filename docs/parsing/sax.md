# SAX Parser

The SAX parser (`XML::SaxParser`) provides an event-driven, callback-based API for parsing XML. You define a handler class with callback methods that are invoked as the parser encounters elements, text, and other XML constructs.

## Defining a Handler

Include `XML::SaxParser::Callbacks` and define the callbacks you need:

```ruby
class MyHandler
  include XML::SaxParser::Callbacks

  def on_start_element_ns(name, attributes, prefix, uri, namespaces)
    puts "Start: #{name}"
  end

  def on_end_element_ns(name, prefix, uri)
    puts "End: #{name}"
  end

  def on_characters(chars)
    puts "Text: #{chars}" unless chars.strip.empty?
  end
end
```

## Parsing

```ruby
handler = MyHandler.new

parser = XML::SaxParser.file('data.xml')
parser.callbacks = handler
parser.parse
```

## Available Callbacks

| Callback | Called When |
|----------|------------|
| `on_start_document` | Document begins |
| `on_end_document` | Document ends |
| `on_start_element_ns` | Opening tag |
| `on_end_element_ns` | Closing tag |
| `on_characters` | Text content |
| `on_cdata_block` | CDATA section |
| `on_comment` | Comment |
| `on_processing_instruction` | Processing instruction |
| `on_error` | Parse error |

## Example: Extract All Text from a Document

```ruby
class TextExtractor
  include XML::SaxParser::Callbacks

  attr_reader :text

  def initialize
    @text = []
  end

  def on_characters(chars)
    stripped = chars.strip
    @text << stripped unless stripped.empty?
  end
end

handler = TextExtractor.new
parser = XML::SaxParser.file('document.xml')
parser.callbacks = handler
parser.parse

puts handler.text.join(' ')
```

## Example: Build a Hash from XML

```ruby
class XMLToHash
  include XML::SaxParser::Callbacks

  attr_reader :result

  def initialize
    @result = {}
    @stack = []
    @current_text = ''
  end

  def on_start_element_ns(name, attributes, prefix, uri, namespaces)
    @stack.push(name)
    @current_text = ''
  end

  def on_characters(chars)
    @current_text << chars
  end

  def on_end_element_ns(name, prefix, uri)
    @stack.pop
    stripped = @current_text.strip
    @result[name] = stripped unless stripped.empty?
    @current_text = ''
  end
end

handler = XMLToHash.new
parser = XML::SaxParser.string('<user><name>Alice</name><email>alice@example.com</email></user>')
parser.callbacks = handler
parser.parse

puts handler.result  # => {"name"=>"Alice", "email"=>"alice@example.com"}
```

## Example: Count Elements by Name

```ruby
class ElementCounter
  include XML::SaxParser::Callbacks

  attr_reader :counts

  def initialize
    @counts = Hash.new(0)
  end

  def on_start_element_ns(name, attributes, prefix, uri, namespaces)
    @counts[name] += 1
  end
end

handler = ElementCounter.new
parser = XML::SaxParser.file('large_document.xml')
parser.callbacks = handler
parser.parse

handler.counts.sort_by { |_, v| -v }.first(10).each do |name, count|
  puts "#{name}: #{count}"
end
```

## When to Use SAX

SAX parsing is useful when:

- The document is too large for DOM parsing
- You only need to extract specific data
- You want to process the document in a single pass
- Memory usage is a concern

For most streaming use cases, the [Reader](reader.md) provides a simpler API.
