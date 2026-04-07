# Installation

## RubyGems

```
gem install libxml-ruby
```

## Bundler

Add to your Gemfile:

```ruby
gem 'libxml-ruby'
```

## Specifying libxml2 Location

If the build process cannot find libxml2, specify the location of `xml2-config`:

```
gem install libxml-ruby -- --with-xml2-config=/path/to/xml2-config
```

Or with Bundler:

```
bundle config build.libxml-ruby --with-xml2-config=/path/to/xml2-config
bundle install
```

You can also specify directories directly:

```
--with-xml2-dir=/path/to/libxml2
--with-xml2-lib=/path/to/libxml2/lib
--with-xml2-include=/path/to/libxml2/include
```

## Platform Notes

### Linux / macOS

A C compiler is required to build the native extension. libxml2 development headers must be installed:

```
# Debian/Ubuntu
sudo apt-get install libxml2-dev

# macOS (Homebrew)
brew install libxml2
```

### Windows

Install the `libxml-ruby` gem with a MinGW toolchain via [RubyInstaller](https://rubyinstaller.org/) or [MSYS2](https://msys2.github.io/).

## Source Code

libxml-ruby's source code is on [GitHub](https://github.com/xml4r/libxml-ruby).
