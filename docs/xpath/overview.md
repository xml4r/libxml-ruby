# XPath

XPath is the primary way to find and extract data from XML documents in libxml-ruby. Unlike some other Ruby XML libraries, libxml-ruby does not support CSS selectors — XPath is the query language for all search operations.

## Quick Reference

```ruby
doc = XML::Parser.file('books.xml').parse

# Find all matching nodes — returns XML::XPath::Object
nodes = doc.find('//book')

# Find from a specific node
titles = doc.root.find('book/title')

# Find the first match
node = doc.find_first('//book[@id="1"]')

# XPath can return different types
doc.find('count(//book)')      # => Float
doc.find('string(//title)')    # => String
doc.find('1 = 1')              # => true
```

## XPath Crash Course

If you're new to XPath, here are the essentials.

### Selecting Nodes

| Expression | Selects |
|-----------|---------|
| `/root` | Root element named "root" |
| `/root/child` | Direct children named "child" |
| `//book` | All "book" elements anywhere in the document |
| `.` | Current node |
| `..` | Parent node |
| `@id` | Attribute named "id" |

### Predicates (Filters)

| Expression | Selects |
|-----------|---------|
| `//book[1]` | First book element |
| `//book[last()]` | Last book element |
| `//book[@id]` | Books with an "id" attribute |
| `//book[@id="42"]` | Books where id is "42" |
| `//book[price>10]` | Books where price child > 10 |

### Axes

| Expression | Selects |
|-----------|---------|
| `child::book` | Child elements named "book" (same as `book`) |
| `ancestor::catalog` | Ancestor elements named "catalog" |
| `following-sibling::*` | All following siblings |
| `preceding-sibling::*` | All preceding siblings |
| `descendant::*` | All descendants |
| `self::book` | Current node if it's named "book" |

### Functions

| Function | Returns |
|----------|---------|
| `count(//book)` | Number of matching nodes |
| `string(//title)` | Text content of first match |
| `contains(@class, 'active')` | True if attribute contains substring |
| `starts-with(name, 'J')` | True if string starts with prefix |
| `not(@disabled)` | Boolean negation |
| `position()` | Position of current node in set |
| `normalize-space(text())` | Trimmed, collapsed whitespace |

### Combining Expressions

```ruby
# Union — combine multiple paths
doc.find('//title | //author')

# Boolean operators in predicates
doc.find('//book[@year > 2000 and @lang = "en"]')
```

For the full XPath 1.0 specification, see the [W3C XPath Reference](https://www.w3.org/TR/xpath-10/).

## Practical Examples

### Extract Data from an RSS Feed

```ruby
doc = XML::Parser.file('feed.xml').parse

doc.find('//item').each do |item|
  title = item.find_first('title').content
  link  = item.find_first('link').content
  puts "#{title}: #{link}"
end
```

### Find Elements by Attribute Value

```ruby
# All books published after 2020
doc.find('//book[@year > 2020]').each do |book|
  puts book.find_first('title').content
end

# Elements with a specific class
doc.find('//*[@class="highlight"]')
```

### Conditional Extraction

```ruby
# Books with a price, sorted extraction
doc.find('//book[price]').each do |book|
  title = book.find_first('title').content
  price = book.find_first('price').content.to_f
  puts "#{title}: $#{'%.2f' % price}" if price > 20
end
```

### Count and Aggregate

```ruby
# Count elements
total = doc.find('count(//book)')  # => Float

# Get text content directly
first_title = doc.find('string(//book[1]/title)')  # => String
```

### Find First Match

```ruby
# find_first is a convenience for find(...).first
node = doc.find_first('//book[@id="42"]')
if node
  puts node.find_first('title').content
end
```

### Navigate Relative to a Node

```ruby
chapter = doc.find_first('//chapter[@id="3"]')

# All sections within this chapter
chapter.find('section').each do |section|
  puts section['title']
end

# Paragraphs anywhere under this chapter
chapter.find('.//p').each do |p|
  puts p.content
end
```
