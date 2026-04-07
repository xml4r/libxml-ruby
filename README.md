# LibXML Ruby

## Overview
The libxml gem provides Ruby language bindings for GNOME's Libxml2
XML toolkit. It is free software, released under the MIT License.

We think libxml-ruby is the best XML library for Ruby because:

* Speed - It's much faster than REXML
* Features - It provides an amazing number of features
* Conformance - It passes all 1800+ tests from the OASIS XML Tests Suite

## Requirements
libxml-ruby requires Ruby 3.2 or higher.  It depends on libxml2 to
function properly.  libxml2, in turn, depends on:

* libm      (math routines: very standard)
* libz      (zlib)
* libiconv

If you are running Linux or Unix you'll need a C compiler so the
extension can be compiled when it is installed.  If you are running
Windows, then install the x64-mingw-ucr gem or build it yourself using
[Ruby for Windows](https://rubyinstaller.org/) or directly with
[msys2](https://msys2.github.io/) and ucrt64.

## Installation
The easiest way to install libxml-ruby is via RubyGems.  To install:

```
gem install libxml-ruby
```

If the extension compile process cannot find libxml2, you may need to indicate
the location of the libxml2 configuration utility as it is used to find the
required header and include files.  (If you need to indicate a location for the
libxml2 library or header files different than reported by `xml2-config`,
see the additional configuration options.)

This may be done with RubyGems:

```
gem install libxml-ruby -- --with-xml2-dir=/path/to/xml2-config
```

Or bundler:

```
bundle config build.libxml-ruby --with-xml2-config=/path/to/xml2-config
bundle install libxml-ruby
```

If you are running Windows, then install the libxml-ruby-x64-mingw32 gem.
The gem includes prebuilt extensions for Ruby 3.2 and 3.3.

The gem also includes a Microsoft VC++ solution and XCode project - these
are very useful for debugging.

libxml-ruby's source code lives on [GitHub](https://github.com/xml4r/libxml-ruby).

## Getting Started
Using libxml is easy. First decide what parser you want to use:

* Generally you'll want to use the `LibXML::XML::Parser` which provides a tree based API.
* For larger documents that don't fit into memory, or if you prefer an input based API, use the `LibXML::XML::Reader`.
* To parse HTML files use `LibXML::XML::HTMLParser`.
* If you are masochistic, then use the `LibXML::XML::SaxParser`, which provides a callback API.

Once you have chosen a parser, choose a datasource.  Libxml can parse files, strings, URIs
and IO streams.  For each data source you can specify an `LibXML::XML::Encoding`, a base uri and
various parser options.  For more information, refer the `LibXML::XML::Parser.document`,
`LibXML::XML::Parser.file`, `LibXML::XML::Parser.io` or `LibXML::XML::Parser.string` methods (the
same methods are defined on all four parser classes).

## Advanced Functionality
Beyond the basics of parsing and processing XML and HTML documents,
libxml provides a wealth of additional functionality.

Most commonly, you'll want to use its `LibXML::XML::XPath` support, which makes
it easy to find data inside an XML document.  Although not as popular,
`LibXML::XML::XPointer` provides another API for finding data inside an XML document.

Often times you'll need to validate data before processing it. For example,
if you accept user generated content submitted over the Web, you'll
want to verify that it does not contain malicious code such as embedded scripts.
This can be done using libxml's powerful set of validators:

* DTDs (`LibXML::XML::Dtd`)
* Relax Schemas (`LibXML::XML::RelaxNG`)
* XML Schema (`LibXML::XML::Schema`)

Finally, if you'd like to use XSL Transformations to process data, then install
the [libxslt gem](https://github.com/xml4r/libxslt-ruby).

## Usage
For information about using libxml-ruby please refer to its
[documentation](https://xml4r.github.io/libxml-ruby/).

All libxml classes are in the `LibXML::XML` module. The easiest
way to use libxml is to `require 'xml'`.  This will mixin
the LibXML module into the global namespace, allowing you to
write code like this:

```ruby
require 'xml'
document = XML::Document.new
```

However, when creating an application or library you plan to
redistribute, it is best to not add the LibXML module to the global
namespace, in which case you can either write your code like this:

```ruby
require 'libxml'
document = LibXML::XML::Document.new
```

Or you can utilize a namespace for your own work and include LibXML into it.
For example:

```ruby
require 'libxml'

module MyApplication
  include LibXML

  class MyClass
    def some_method
      document = XML::Document.new
    end
  end
end
```

For simplicity's sake, the documentation uses the xml module in its examples.

## Tests

To run tests you first need to build the shared library:

```
rake compile
```

Once you have built the shared library, you can then run tests using rake:

```
rake test
```

[![Build Status](https://github.com/xml4r/libxml-ruby/actions/workflows/mri.yml/badge.svg)](https://github.com/xml4r/libxml-ruby/actions/workflows/mri.yml)

## Documentation
Documentation is available at [xml4r.github.io/libxml-ruby](https://xml4r.github.io/libxml-ruby/).

API reference documentation is generated via rdoc and is available at
[xml4r.github.io/libxml-ruby/reference](https://xml4r.github.io/libxml-ruby/reference/).

## Support
If you have any questions about using libxml-ruby, please report an issue
on [GitHub](https://github.com/xml4r/libxml-ruby/issues).

## License
See [LICENSE](LICENSE) for license information.
