require 'rubygems'
require 'date'
require 'rake/gempackagetask'
require 'rake/rdoctask'
require 'date'

SO_NAME = "libxml.so"

# ------- Default Package ----------
RUBY_PROF_VERSION = "0.6.1"

FILES = FileList[
  'Rakefile',
  'README',
  'LICENSE',
  'CHANGES',
  'bin/*',
  'lib/**/*',
  'ext/*',
  'doc/**/*',
  'test/*'
]

# Default GEM Specification
default_spec = Gem::Specification.new do |spec|
  spec.name = "libxml-ruby"
  
  spec.homepage = "http://libxml.rubyforge.org/"
  spec.summary = "Ruby libxml bindings"
  spec.description = <<-EOF
    The Libxml-Ruby project provides Ruby language bindings for the GNOME
    Libxml2 XML toolkit. It is free software, released under the MIT License.
    Libxml-ruby's primary advantage over REXML is performance - if speed 
    is your need, these are good libraries to consider, as demonstrated
    by the informal benchmark below.
  EOF

  # Determine the current version of the software
  if File.read('ext/libxml/libxml.h') =~ /\s*RUBY_LIBXML_VERSION\s*['"](\d.+)['"]/
    CURRENT_VERSION = $1
  else
    CURRENT_VERSION = "0.0.0"
  end
  
  if ENV['REL']
    PKG_VERSION = ENV['REL']
  else
    PKG_VERSION = CURRENT_VERSION
  end

  spec.version = RUBY_PROF_VERSION

  spec.author = "Charlie Savage"
  spec.email = "cfis@savagexi.com"
  spec.platform = Gem::Platform::RUBY
  spec.require_path = "lib" 
  spec.bindir = "bin"
#  spec.executables = ["ruby-prof"]
 # spec.extensions = ["ext/extconf.rb"]
  spec.files = FILES.to_a
  spec.test_files = Dir["test/test_*.rb"]
  
  spec.required_ruby_version = '>= 1.8.4'
  spec.date = DateTime.now
  spec.rubyforge_project = 'libxml-ruby'
  
  # rdoc
  spec.has_rdoc = true
  spec.rdoc_options << "--title" << "libxml-ruby"
  # Show source inline with line numbers
  spec.rdoc_options << "--inline-source" << "--line-numbers"
  # Make the readme file the start page for the generated html
  spec.rdoc_options << '--main' << 'README'
#  spec.extra_rdoc_files = ['bin/ruby-prof',
 #                          'ext/ruby_prof.c',
  #                         'examples/flat.txt',
   #                        'examples/graph.txt',
    #                       'examples/graph.html',
     #                      'README',
      #                     'LICENSE']

end

# Rake task to build the default package
Rake::GemPackageTask.new(default_spec) do |pkg|
  pkg.need_tar = true
  pkg.need_zip = true
end


# ------- Windows Package ----------
# Windows specification
win_spec = default_spec.clone
win_spec.extensions = []
win_spec.platform = Gem::Platform::CURRENT
win_spec.files += ["lib/#{SO_NAME}"]

desc "Create Windows Gem"
task :create_win32_gem do
  # Copy the win32 extension built by MingW - easier to install
  # since there are no dependencies of msvcr80.dll
  current_dir = File.expand_path(File.dirname(__FILE__))
  #source = File.join(current_dir, "mingw", SO_NAME)
  #target = File.join(current_dir, "lib", SO_NAME)
  source = File.join(current_dir, "vc", "Debug", SO_NAME)
  target = File.join(current_dir, "lib", SO_NAME)
  cp(source, target)

  # Create the gem, then move it to pkg
  Gem::Builder.new(win_spec).build
  gem_file = "#{win_spec.name}-#{win_spec.version}-#{win_spec.platform}.gem"
  mv(gem_file, "pkg/#{gem_file}")

  # Remove win extension from top level directory  
  rm(target)
end


task :package => :create_win32_gem

# ---------  RDoc Documentation ------
desc "Generate rdoc documentation"
Rake::RDocTask.new("rdoc") do |rdoc|
  rdoc.rdoc_dir = 'doc'
  rdoc.title    = "libxml-ruby"
  # Show source inline with line numbers
  rdoc.options << "--inline-source" << "--line-numbers"
  # Make the readme file the start page for the generated html
  rdoc.options << '--main' << 'README'
  rdoc.rdoc_files.include('bin/**/*',
                          'doc/*.rdoc',
                          'lib/**/*.rb',
                          'ext/**/*.c',
                          'README',
                          'LICENSE')
end