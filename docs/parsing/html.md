# HTML Parser

The `XML::HTMLParser` parses HTML documents, including malformed HTML that would fail strict XML parsing. It produces a DOM tree just like `XML::Parser`.

## Parsing HTML

```ruby
# From a file
doc = XML::HTMLParser.file('page.html').parse

# From a string
doc = XML::HTMLParser.string('<html><body><p>Hello</p></body></html>').parse

# From an IO
File.open('page.html') do |io|
  doc = XML::HTMLParser.io(io).parse
end
```

## Example: Extract Links from HTML

```ruby
html = <<~HTML
  <html>
  <body>
    <a href="https://example.com">Example</a>
    <a href="https://ruby-lang.org">Ruby</a>
    <a href="/about">About</a>
  </body>
  </html>
HTML

doc = XML::HTMLParser.string(html).parse

doc.find('//a[@href]').each do |link|
  puts "#{link.content} -> #{link['href']}"
end
```

## Example: Extract Text Content

```ruby
doc = XML::HTMLParser.file('article.html').parse

# Get all paragraph text
doc.find('//p').each do |p|
  puts p.content
end

# Get the page title
title = doc.find_first('//title')
puts title.content if title
```

## Example: Parse a Table

```ruby
doc = XML::HTMLParser.string(html).parse

doc.find('//table//tr').each do |row|
  cells = row.find('td').map(&:content)
  puts cells.join(' | ')
end
```

## Handling Malformed HTML

The HTML parser is lenient — it handles missing close tags, incorrect nesting, and other common HTML issues:

```ruby
# This would fail as XML but parses fine as HTML
html = '<p>First<p>Second<br><b>Bold<i>BoldItalic</b></i>'

doc = XML::HTMLParser.string(html).parse
doc.find('//p').each { |p| puts p.content }
```

## Options

HTML parser options are on `XML::HTMLParser::Options`:

```ruby
parser = XML::HTMLParser.string(html)
parser.options = XML::HTMLParser::Options::NOERROR |
                 XML::HTMLParser::Options::NOWARNING
doc = parser.parse
```

Suppressing errors and warnings is common with real-world HTML, which often triggers parser warnings.

## Encoding

Specify encoding when the HTML doesn't declare it:

```ruby
doc = XML::HTMLParser.string(html, encoding: XML::Encoding::UTF_8).parse
```
