#!/usr/bin/env ruby

# Be sure to set ENV['RUBYFORGE_USERNAME'] to use publish.

require 'rubygems'
require 'rake/gempackagetask'
require 'hanna/rdoctask'
require 'rake/testtask'
require 'grancher/task'
require 'date'
require 'yaml'

UNIXNAME = File.read('.root/unixname').strip
LOADPATH = File.read('.root/loadpath').strip
VERSION  = File.read('VERSION').strip
PROFILE  = YAML.load(File.new('PROFILE'))

# ------- Default Package ----------
FILES = FileList[
  'Rakefile',
  'CHANGES',
  'LICENSE',
  'README',
  'setup.rb',
  'doc/**/*',
  'ext/libxml/*',
  'ext/mingw/Rakefile',
  'ext/mingw/build.rake',
  'ext/vc/*.sln',
  'ext/vc/*.vcproj',
  'lib/**/*',
  'script/**/*',
  'test/**/*'
]

# Default GEM Specification
default_spec = Gem::Specification.new do |spec|
  spec.name = UNIXNAME
  
  spec.homepage    = PROFILE['resources']['home']
  spec.summary     = PROFILE['summary']
  spec.description = PROFILE['description']

  # Determine the current version of the software
  spec.version = 
    if File.read('ext/libxml/ruby_xml_version.h') =~ /\s*RUBY_LIBXML_VERSION\s*['"](\d.+)['"]/
      CURRENT_VERSION = $1
    else
      CURRENT_VERSION = "0.0.0"
    end
  
  spec.author = PROFILE['authors'].first
  spec.email  = PROFILE['resources']['mail'] # ?
  spec.platform = Gem::Platform::RUBY
  spec.require_paths = LOADPATH
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
  pkg.package_dir = 'pkg'
  pkg.need_tar    = false
end


# ------- Windows GEM ----------
if RUBY_PLATFORM.match(/win32/)
  binaries = (FileList['ext/mingw/*.so',
                       'ext/mingw/*.dll*'])

  # Windows specification
  win_spec = default_spec.clone
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

# ---------  Publish Website to Rubyforge ---------
#desc "publish website (uses rsync)"
#task :publish => [:publish_website, :publish_rdoc]

#task :publish_website do
  #unixname = 'libxml'
  #username = ENV['RUBYFORGE_USERNAME']

  #dir = 'admin/web'
  #url = "#{username}@rubyforge.org:/var/www/gforge-projects/#{unixname}"

  #dir = dir.chomp('/') + '/'

  ## Using commandline filter options didn't seem to work, so
  ## I opted for creating an .rsync_filter file for all cases.

  #protect = %w{usage statcvs statsvn robot.txt wiki}
  #exclude = %w{.svn}

  #rsync_file = File.join(dir,'.rsync-filter')
  #unless File.file?(rsync_file)
    #File.open(rsync_file, 'w') do |f|
      #exclude.each{|e| f << "- #{e}\n"}
      #protect.each{|e| f << "P #{e}\n"}
    #end
  #end

  ## maybe -p ?
  #cmd = "rsync -rLvz --delete-after --filter='dir-merge #{rsync_file}' #{dir} #{url}"
  #sh cmd
#end

# Instead of this, we simply ln -s web/rdoc ../doc/libxml-ruby/rdoc, and publish website.
#task :publish_rdoc do
#  unixname = 'libxml'
#  username = ENV['RUBYFORGE_USERNAME']
#
#  dir = 'doc/rdoc'
#  url = "#{username}@rubyforge.org:/var/www/gforge-projects/#{unixname}/rdoc"
#
#  dir = dir.chomp('/') + '/'
#
#  # maybe -p ?
#  cmd = "rsync -rLvz --delete-after #{dir} #{url}"
#  sh cmd
#end

