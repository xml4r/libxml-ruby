# libxml-ruby

Ruby language bindings for the [GNOME Libxml2](http://xmlsoft.org/) XML toolkit. It is free software, released under the MIT License.

libxml-ruby stands out because of:

* **Speed** - Much faster than REXML
* **Features** - Full DOM, SAX, Reader, Writer, XPath, validation (DTD, RelaxNG, XML Schema) and more
* **Conformance** - Passes all 1800+ tests from the OASIS XML Tests Suite

## Quick Example

```ruby
require 'libxml-ruby'

# Parse a document
doc = LibXML::XML::Document.file('books.xml')

# Find nodes with XPath
doc.find('//book').each do |node|
  puts node['title']
end

# Validate against a schema
schema = LibXML::XML::Schema.new('books.xsd')
doc.validate_schema(schema)
```

## Requirements

libxml-ruby requires Ruby 3.2 or higher and depends on [libxml2](http://xmlsoft.org/).

## License

libxml-ruby is released under the [MIT License](https://github.com/xml4r/libxml-ruby/blob/master/LICENSE).
