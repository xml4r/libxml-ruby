# Parsing Overview

libxml-ruby provides four parsers for reading XML and HTML content. Each parser supports files, strings, IO objects and URIs as data sources.

## Parser Comparison

| Parser | API Style | Memory | Use Case |
|--------|-----------|--------|----------|
| [DOM Parser](dom.md) | Tree | Loads entire document | Most common. Navigate and modify documents freely. |
| [Reader](reader.md) | Pull/cursor | Streaming | Large documents. Move forward through nodes one at a time. |
| [SAX Parser](sax.md) | Push/callback | Streaming | Event-driven processing. You define callbacks for each event. |
| [HTML Parser](html.md) | Tree | Loads entire document | Malformed HTML. Tolerates missing tags, bad nesting, etc. |

## Choosing a Parser

For most use cases, start with the **DOM Parser**. It loads the entire document into memory and gives you full access to navigate, query, and modify the tree.

Use the **Reader** when the document is too large for memory, or when you only need to extract specific data in a single pass.

Use the **SAX Parser** only if you need maximum control over the parsing events. The Reader is usually simpler for streaming.

Use the **HTML Parser** when dealing with real-world HTML that may not be well-formed XML.

## Data Sources

All parsers support the same data sources:

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

## Parser Options

Options control parsing behavior. They are constants on `XML::Parser::Options` and can be combined with bitwise OR:

```ruby
parser = XML::Parser.file('data.xml')
parser.options = XML::Parser::Options::NOBLANKS | XML::Parser::Options::NONET
doc = parser.parse
```

Common options:

| Option | Effect |
|--------|--------|
| `NOBLANKS` | Remove blank nodes (whitespace-only text between elements) |
| `NONET` | Disable network access (recommended for untrusted input) |
| `NOERROR` | Suppress error messages |
| `NOWARNING` | Suppress warning messages |
| `NOCDATA` | Merge CDATA as text nodes |
| `DTDLOAD` | Load the external DTD subset |
| `DTDVALID` | Validate with the DTD |
| `HUGE` | Relax hardcoded parser limits |

## Security

When parsing untrusted input, disable network access and entity expansion:

```ruby
parser = XML::Parser.string(untrusted_xml)
parser.options = XML::Parser::Options::NONET | XML::Parser::Options::NOENT
doc = parser.parse
```

## Encoding

Specify the encoding when the document doesn't declare it:

```ruby
parser = XML::Parser.file('data.xml', encoding: XML::Encoding::UTF_8)
doc = parser.parse
```
