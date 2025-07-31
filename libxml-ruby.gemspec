# encoding: utf-8
require 'date'

# Determine the current version of the software
version = File.read('ext/libxml/ruby_xml_version.h').match(/\s*RUBY_LIBXML_VERSION\s*['"](\d.+)['"]/)[1]

Gem::Specification.new do |spec|
  spec.name        = 'libxml-ruby'
  spec.version     = version
  spec.homepage    = 'https://xml4r.github.io/libxml-ruby/'
  spec.summary     = 'Ruby Bindings for LibXML2'
  spec.description = <<-EOS
    The Libxml-Ruby project provides Ruby language bindings for the GNOME
    Libxml2 XML toolkit. It is free software, released under the MIT License.
    Libxml-ruby's primary advantage over REXML is performance - if speed
    is your need, these are good libraries to consider, as demonstrated
    by the informal benchmark below.
  EOS
  spec.authors = ['Ross Bamform', 'Wai-Sun Chia', 'Sean Chittenden',
                  'Dan Janwoski', 'Anurag Priyam', 'Charlie Savage',
                  'Ryan Johnson']
  spec.platform = Gem::Platform::RUBY
  spec.bindir = 'bin'
  spec.extensions = ['ext/libxml/extconf.rb']
  spec.metadata = {'documentation_uri' => 'https://xml4r.github.io/libxml-ruby/'}
  spec.files = Dir.glob(['HISTORY',
                         'LICENSE',
                         'libxml-ruby.gemspec',
                         'MANIFEST',
                         'Rakefile',
                         'README.rdoc',
                         'setup.rb',
                         'ext/libxml/*.def',
                         'ext/libxml/*.h',
                         'ext/libxml/*.c',
                         'ext/libxml/*.rb',
                         'ext/vc/*.sln',
                         'ext/vc/*.vcprojx',
                         'lib/**/*.rb',
                         'script/**/*',
                         'test/**/*'])

  spec.test_files = Dir.glob('test/test_*.rb')
  spec.required_ruby_version = '>= 2.5'
  spec.date = DateTime.now
  spec.add_development_dependency('logger')
  spec.add_development_dependency('rake-compiler')
  spec.add_development_dependency('minitest')
  spec.license = 'MIT'
end
