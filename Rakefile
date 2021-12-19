#!/usr/bin/env ruby

require "rubygems"
require "rake/extensiontask"
require "rake/testtask"
require "rubygems/package_task"
require "rdoc/task"
require "yaml"

GEM_NAME = "libxml-ruby"
SO_NAME  = "libxml_ruby"

# Read the spec file
spec = Gem::Specification.load("#{GEM_NAME}.gemspec")

task :default => [:test]

# Setup compile tasks
Rake::ExtensionTask.new do |ext|
  ext.gem_spec = spec
  ext.name = SO_NAME
  ext.ext_dir = "ext/libxml"
  ext.lib_dir = "lib/#{RUBY_VERSION.sub(/\.\d$/, '')}"
  if RUBY_PLATFORM.match(/mswin32|mswin64|mingw32|ucrt/)
    ext.config_options <<
      if (dir = ENV['WINDOWS_XML2_INCLUDE'])
        "--with-xml2-include=#{dir}"
      else
        case RUBY_PLATFORM
        when 'i386-mingw32'
          '--with-xml2-include=C:/msys64/mingw32/include/libxml2'
        when 'x64-mingw32'
          '--with-xml2-include=C:/msys64/mingw64/include/libxml2'
        when 'x64-mingw-ucrt'
          '--with-xml2-include=C:/msys64/ucrt64/include/libxml2'
        else
          raise "Unknown Windows Ruby, please set ENV['WINDOWS_XML2_INCLUDE']"
        end
      end
  else
    ext.config_options << '--with-xml2-include=/usr/include/libxml2'
  end
end

# Setup generic gem
Gem::PackageTask.new(spec) do |pkg|
  pkg.package_dir = 'pkg'
  pkg.need_tar    = false
end

# Setup Windows Gem
if RUBY_PLATFORM.match(/mswin32|mswin64|mingw32/)
  binaries = (FileList['lib/**/*.so',
                       'lib/**/*dll'])

  # Windows specification
  win_spec = spec.clone
  win_spec.platform = Gem::Platform::CURRENT
  win_spec.files += binaries.to_a
  win_spec.instance_variable_set(:@cache_file, nil)

  # Unset extensions
  win_spec.extensions = nil

  # Rake task to build the windows package
  Gem::PackageTask.new(win_spec) do |pkg|
    pkg.package_dir = 'pkg'
    pkg.need_tar = false
  end
end

# RDoc Task
desc 'Generate rdoc documentation'
RDoc::Task.new("rdoc") do |rdoc|
  rdoc.rdoc_dir = 'rdoc'
  rdoc.title    = 'LibXML'
  rdoc.generator = 'hanna'

  # Show source inline with line numbers
  rdoc.options << '--line-numbers'
  rdoc.options << '--charset=utf-8'
  # Make the readme file the start page for the generated html
  rdoc.main = 'README.rdoc'
  rdoc.rdoc_files.include('doc/*.rdoc',
                          'ext/**/libxml.c',
                          'ext/**/ruby_xml.c',
                          'ext/**/*.c',
                          'lib/**/*.rb',
                          'README.rdoc',
                          'HISTORY',
                          'LICENSE')
end

# Test Task
Rake::TestTask.new do |t|
  t.libs << "test"
  t.test_files = FileList['test/test*.rb'] - ['test/test_suite.rb']
  t.verbose = true
end