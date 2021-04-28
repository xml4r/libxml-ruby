# Determine the current version of the software
version = File.read('ext/libxml/ruby_xml_version.h').match(/\s*RUBY_LIBXML_VERSION\s*['"](\d.+)['"]/)[1]

Gem::Specification.new do |spec|
  spec.name        = 'salsify_libxml_ruby'
  spec.version     = version
  spec.homepage    = 'http://xml4r.github.com/libxml-ruby'
  spec.summary     = 'Salsify modified Ruby Bindings for LibXML2'
  spec.authors     = ['Salsify, Inc']
  spec.email       = ['engineering@salsify.com']
  spec.description = <<-EOS
    Forked from the Libxml-Ruby project to do static linking against a local
    copy of the libxml library and use the bindings to undocumented Schema
    Object Model accessors.
    The Libxml-Ruby project provides Ruby language bindings for the GNOME
    Libxml2 XML toolkit. It is free software, released under the MIT License.
  EOS
  spec.platform = Gem::Platform::RUBY
  spec.bindir = 'bin'
  spec.extensions = ['ext/libxml/extconf.rb']
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
  spec.add_runtime_dependency("mini_portile2", ">= 2.4.0") # keep version in sync with the one in extconf.rb
  spec.add_development_dependency 'salsify_gem'
  spec.add_development_dependency 'rake-compiler'
  spec.add_development_dependency 'minitest'
  spec.license = 'MIT'
  spec.metadata['allowed_push_host'] = 'https://gems.salsify.com'
end
