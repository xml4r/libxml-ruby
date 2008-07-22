#!/usr/bin/env ruby

# Be sure to set ENV['RUBYFORGE_USERNAME'] to use publish.

require 'rubygems'
require 'date'
require 'rake/gempackagetask'
require 'rake/rdoctask'
require 'rake/testtask'
require 'date'


# ------- Default Package ----------
FILES = FileList[
  'RAKEFILE',
  'README',
  'LICENSE',
  'CHANGES',
  'setup.rb',
  'doc/**/*',
  'ext/libxml/*',
  'ext/mingw/Rakefile',
  'ext/vc/*.sln',
  'ext/vc/*.vcproj',
  'lib/**/*',
  'benchmark/**/*',
  'test/**/*'
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
  spec.version = 
    if File.read('ext/libxml/version.h') =~ /\s*RUBY_LIBXML_VERSION\s*['"](\d.+)['"]/
      CURRENT_VERSION = $1
    else
      CURRENT_VERSION = "0.0.0"
    end
  
  spec.author = "Charlie Savage"
  spec.email = "libxml-devel@rubyforge.org"
  spec.platform = Gem::Platform::RUBY
  spec.require_paths = ["lib", "ext/libxml"]
  spec.bindir = "bin"
  spec.extensions = ["ext/libxml/extconf.rb"]
  spec.files = FILES.to_a
  spec.test_files = Dir.glob("test/tc_*.rb")
  
  spec.required_ruby_version = '>= 1.8.4'
  spec.date = DateTime.now
  spec.rubyforge_project = 'libxml'
  
  spec.has_rdoc = true
end

# Rake task to build the default package
Rake::GemPackageTask.new(default_spec) do |pkg|
  pkg.package_dir = 'admin/pkg'
  pkg.need_tar = true
  pkg.need_zip = true
end


# ------- Windows Package ----------

# Use *.dll* to get import libraries
binaries = (FileList['ext/mingw/*.so',
                     'ext/mingw/*.dll*'])

# Windows specification
win_spec = default_spec.clone
win_spec.extensions = []
win_spec.platform = Gem::Platform::CURRENT
win_spec.files += binaries.map {|binaryname| "lib/#{File.basename(binaryname)}"}


desc "Create Windows Gem"
task :create_win32_gem do
  # Copy the win32 extension built by MingW - easier to install
  # since there are no dependencies of msvcr80.dll
  current_dir = File.expand_path(File.dirname(__FILE__))

  binaries.each do |binaryname|
    target = File.join(current_dir, 'lib', File.basename(binaryname))
    cp(binaryname, target)
  end
  
  # Create the gem, then move it to admin/pkg
  Gem::Builder.new(win_spec).build
  gem_file = "#{win_spec.name}-#{win_spec.version}-#{win_spec.platform}.gem"
  mv(gem_file, "admin/pkg/#{gem_file}")

  # Remove win extension from top level directory  
  binaries.each do |binaryname|
    target = File.join(current_dir, 'lib', File.basename(binaryname))
    rm(target)
  end
end


# ---------  RDoc Documentation ---------
desc "Generate rdoc documentation"
Rake::RDocTask.new("rdoc") do |rdoc|
  rdoc.rdoc_dir = 'doc/rdoc'
  rdoc.title    = "LibXML"
  # Show source inline with line numbers
  rdoc.options << "--inline-source" << "--line-numbers"
  # Make the readme file the start page for the generated html
  rdoc.options << '--main' << 'README'
  rdoc.rdoc_files.include('doc/*.rdoc',
                          'ext/**/*.c',
                          'lib/**/*.rb',
                          'CHANGES',
                          'README',
                          'LICENSE',
                          'VERSION')
end

task :default => :package
task :package => :create_win32_gem

Rake::TestTask.new do |t|
  t.libs << "test"
  t.libs << "lib"
  t.libs << "ext/libxml"
end

if not RUBY_PLATFORM.match(/mswin32/i)
  Rake::Task[:test].prerequisites << :extensions
end

task :build => :extensions
task :extension => :build

ext = Config::CONFIG["DLEXT"]
task :extensions => ["ext/libxml/libxml_ruby.#{ext}"]
file "ext/libxml/libxml_ruby.#{ext}" =>
  ["ext/libxml/Makefile"] + FileList["ext/libxml/*.{c,h}"].to_a do |t|
  Dir.chdir("ext/libxml") { sh "make" }
end

namespace :extensions do
  task :clean do
    Dir.chdir("ext/libxml") do
      sh "rm -f Makefile"
      sh "rm -f *.{o,so,bundle,log}"
    end
  end
end

file "ext/libxml/Makefile" => ["ext/libxml/extconf.rb"] do

  command = ["ruby"] + $:.map{|dir| "-I#{File.expand_path dir}"} + ["extconf.rb"]
  Dir.chdir("ext/libxml") { sh *command }
end

# ---------  Publish Website to Rubyforge ---------
desc "publish website (uses rsync)"
task :publish => [:publish_website, :publish_rdoc]

task :publish_website do
  unixname = 'libxml'
  username = ENV['RUBYFORGE_USERNAME']

  dir = 'admin/web'
  url = "#{username}@rubyforge.org:/var/www/gforge-projects/#{unixname}"

  dir = dir.chomp('/') + '/'

  # Using commandline filter options didn't seem to work, so
  # I opted for creating an .rsync_filter file for all cases.

  protect = %w{usage statcvs statsvn robot.txt wiki}
  exclude = %w{.svn}

  rsync_file = File.join(dir,'.rsync-filter')
  unless File.file?(rsync_file)
    File.open(rsync_file, 'w') do |f|
      exclude.each{|e| f << "- #{e}\n"}
      protect.each{|e| f << "P #{e}\n"}
    end
  end

  # maybe -p ?
  cmd = "rsync -rLvz --delete-after --filter='dir-merge #{rsync_file}' #{dir} #{url}"
  sh cmd
end

task :publish_rdoc do
  unixname = 'libxml'
  username = ENV['RUBYFORGE_USERNAME']

  dir = 'doc/rdoc'
  url = "#{username}@rubyforge.org:/var/www/gforge-projects/#{unixname}/rdoc"

  dir = dir.chomp('/') + '/'

  # maybe -p ?
  cmd = "rsync -rLvz --delete-after #{dir} #{url}"
  sh cmd
end