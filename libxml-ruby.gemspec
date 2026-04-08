# encoding: utf-8

# Determine the current version of the software
version = File.read('ext/libxml/ruby_xml_version.h').match(/\s*RUBY_LIBXML_VERSION\s*['"](\d.+)['"]/)[1]

Gem::Specification.new do |spec|
  spec.name        = 'libxml-ruby'
  spec.version     = version
  spec.homepage    = 'https://xml4r.github.io/libxml-ruby/'
  spec.summary     = 'Ruby Bindings for LibXML2'
  spec.description = <<-EOS
    libxml-Ruby provides Ruby language bindings for libxml2 It is free software, 
    released under the MIT License. libxml-ruby provides DOM, SAX, Reader, and 
    Writer APIs along with XPath support and validation via DTD, RelaxNG, and XML Schema.
  EOS
  spec.authors = ['Ross Bamform', 'Wai-Sun Chia', 'Sean Chittenden',
                  'Dan Janwoski', 'Anurag Priyam', 'Charlie Savage',
                  'Ryan Johnson']
  spec.platform = Gem::Platform::RUBY
  spec.bindir = 'bin'
  spec.extensions = ['ext/libxml/extconf.rb']
  spec.metadata = {'documentation_uri' => 'https://xml4r.github.io/libxml-ruby/'}
  spec.files = Dir.glob(['CHANGELOG.md',
                         'LICENSE',
                         'libxml-ruby.gemspec',
                         'Rakefile',
                         'README.md',
                         'ext/libxml/*.def',
                         'ext/libxml/*.h',
                         'ext/libxml/*.c',
                         'ext/libxml/*.rb',
                         'ext/vc/*.sln',
                         'ext/vc/*.vcprojx',
                         'ext/xcode/**/*',
                         'lib/**/*.rb',
                         'script/**/*',
                         'test/**/*'])

  spec.test_files = Dir.glob('test/test_*.rb')
  spec.required_ruby_version = '>= 3.2'
  spec.add_development_dependency('logger')
  spec.add_development_dependency('rake-compiler')
  spec.add_development_dependency('minitest')
  spec.add_development_dependency('rdoc')
  spec.license = 'MIT'
end
