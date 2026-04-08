# LibXML Ruby

## Overview
The libxml gem provides Ruby language bindings for GNOME's Libxml2
XML toolkit. It is free software, released under the MIT License.

libxml-ruby let's you:

* Read xml files
* Write xml files
* Search xml files using XPath
* Validate XML files

Full documentation is available at [xml4r.github.io/libxml-ruby](https://xml4r.github.io/libxml-ruby/) including a getting started guide.

## Requirements

libxml-ruby requires Ruby 3.2 or higher.

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
gem install libxml-ruby -- --with-xml2-config=/path/to/xml2-config
```

Or bundler:

```
bundle config build.libxml-ruby --with-xml2-config=/path/to/xml2-config
bundle install libxml-ruby
```

Full documentation is available at [xml4r.github.io/libxml-ruby](https://xml4r.github.io/libxml-ruby/) including a [getting started](https://xml4r.github.io/libxml-ruby/getting_started/) guide.

## Tests

To run tests you first need to build the shared library:

```
rake compile
```

Once you have built the shared library, you can then run tests using rake:

```
rake test
```

## Support
If you have any questions about using libxml-ruby, please report an issue
on [GitHub](https://github.com/xml4r/libxml-ruby/issues).

## License
See [LICENSE](LICENSE) for license information.
