require 'net/ftp'
require 'rake/clean'
require 'rake/testtask'
require 'rake/rdoctask'

CLEAN.include '*.o'
CLEAN.include '*.so'
CLOBBER.include '*.log'
CLOBBER.include 'Makefile'
CLOBBER.include 'extconf.h'

desc "Default Task (All tests)"
task :default => :alltests

# Compile tasks -----------------------------------------------------
MAKECMD = ENV['MAKE_CMD'] || 'make'
MAKEOPTS = ENV['MAKE_OPTS'] || ''

file 'Makefile' => 'extconf.rb' do
  ruby 'extconf.rb'
end

# Let make handle dependencies between c/o/so - we'll just run it. 
file 'libxml.so' => 'Makefile' do
  result = IO.popen("#{MAKECMD} #{MAKEOPTS}","r+") do |fp|
    puts fp.read
  end    

  fail "Make failed (status #{$?.exitstatus})" unless $?.exitstatus == 0
end

# Test Tasks ---------------------------------------------------------
task :ta => :alltests
task :tu => :unittests
task :test => :unittests

Rake::TestTask.new(:alltests) do |t|
  t.test_files = FileList[
    'tests/*.rb',
    'tests/contrib/*.rb',
  ]
  t.verbose = true
end
                    
Rake::TestTask.new(:unittests) do |t|
  t.test_files = FileList['tests/tc_*.rb']
  t.verbose = false
end
                          
#Rake::TestTask.new(:funtests) do |t|
  #  t.test_files = FileList['test/fun*.rb']
  #t.warning = true
  #t.warning = true
#end

task :unittests => 'libxml.so'
task :alltests => 'libxml.so'

# RDoc Tasks ---------------------------------------------------------
# Create a task to build the RDOC documentation tree.
Rake::RDocTask.new(:doc) do |rdoc|
  rdoc.rdoc_dir = 'doc'
  rdoc.title    = "Libxml-Ruby API"
  rdoc.options << '--main' << 'README'
  rdoc.rdoc_files.include('README', 'LICENSE', 'TODO')
  rdoc.rdoc_files.include('ruby_xml*.c', '*.rdoc')
end
                
