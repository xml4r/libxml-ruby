#!/usr/bin/env ruby

require 'rubygems'
require 'rake/gempackagetask'
require 'rake/testtask'
require 'rake/rdoctask'
require 'grancher/task'
require 'yaml'

# Read the spec file
spec = Gem::Specification.load("libxml-ruby.gemspec")

# Rake task to build the default package
Rake::GemPackageTask.new(spec) do |pkg|
  pkg.package_dir = 'pkg'
  pkg.need_tar    = false
end

# ------- Windows GEM ----------
if RUBY_PLATFORM.match(/win32|mingw32/)
  binaries = (FileList['ext/mingw/*.so',
                       'ext/mingw/*.dll*'])

  # Windows specification
  win_spec =spec.clone
  win_spec.extensions = ['ext/mingw/Rakefile']
  win_spec.platform = Gem::Platform::CURRENT
  win_spec.files += binaries.to_a

  # Rake task to build the windows package
  Rake::GemPackageTask.new(win_spec) do |pkg|
    pkg.package_dir = 'pkg'
    pkg.need_tar    = false
  end
end

# ---------  RDoc Documentation ---------
desc "Generate rdoc documentation"
Rake::RDocTask.new("rdoc") do |rdoc|
  rdoc.rdoc_dir = 'doc/libxml-ruby/rdoc'
  rdoc.title    = "LibXML"
  # Show source inline with line numbers
  rdoc.options << "--line-numbers"
  # Make the readme file the start page for the generated html
  rdoc.main = 'README.rdoc'
  rdoc.rdoc_files.include('doc/*.rdoc',
                          'ext/**/libxml.c',
                          'ext/**/ruby_xml.c',
                          'ext/**/*.c',
                          'lib/**/*.rb',
                          'README.rdoc',
                          'TODO',
                          'HISTORY',
                          'LICENSE')
end

Rake::TestTask.new do |t|
  t.libs << "test"
  t.libs << "lib"
  t.libs << "ext/libxml"
end

if not RUBY_PLATFORM.match(/mswin32/i)
  Rake::Task[:test].prerequisites << :extensions
end

task :default => :package
task :build => :extensions
task :extension => :build

ext = Config::CONFIG["DLEXT"]
task :extensions => ["ext/libxml/libxml_ruby.#{ext}"]

namespace :extensions do
  task :clean do
    Dir.chdir("ext/libxml") do
      sh "rm -f Makefile"
      sh "rm -f *.{o,so,bundle,log}"
    end
  end
end

# ---------  Publish Website to Github ---------
Grancher::Task.new do |g|
  # push gh-pages
  g.branch  = 'gh-pages'
  # to origin
  g.push_to = 'origin'
  # copy the website directory
  g.directory 'web'
  # and the rdoc directory
  g.directory 'doc/libxml-ruby/rdoc' 'rdoc'
end
