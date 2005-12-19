#!/usr/local/bin/ruby -w

# $Id$

# Chump little program to update the version for libxml.  This program
# must have a PWD of base ruby-libxml directory or be the utils
# directory.


# This program must execute in the base directory of libxml.h
if !FileTest.exists?('libxml.h')
  Dir.chdir('..')
end

content = []
File.open('.rubynet_version') do |f|
  for line in f.readlines
    if line =~ /^\s*#/ or line.strip.empty?
      content.push(line)
      next
    else
      $version = line.chomp.succ
      content.push($version)
      content.push("\n")
    end
  end
end
File.open('.rubynet_version', 'w+') { |f| f.syswrite(content.join('')) }
$vernum = $version.delete('.').to_i

puts $version

# Look for the big wordy multi-line comment and replace the versions
# below it:
tmp_file = ".libxml.h.tmp-#{$$}"
File.open(tmp_file, 'w+') do |t|
  File.open('libxml.h') do |libxml_h|
    for line in libxml_h
      if line == "/* Don't nuke this block!  It is used for automatically updating the\n" and
	  libxml_h.readline == " * versions below. VERSION = string formatting, VERNUM = numbered\n" and
	  libxml_h.readline == " * version for inline testing: increment both or none at all. */\n"

	t.write("/* Don't nuke this block!  It is used for automatically updating the\n")
	t.write(" * versions below. VERSION = string formatting, VERNUM = numbered\n")
	t.write(" * version for inline testing: increment both or none at all. */\n")
	t.write("#define RUBY_LIBXML_VERSION  \"#{$version}\"\n")
	t.write("#define RUBY_LIBXML_VERNUM   #{$vernum}\n")

	throw,away = libxml_h.readline,libxml_h.readline
      else
	t.write(line)
      end # if line ==...
    end # for line in
  end # File.open('libxml.h')
end # File.open()

File.rename(tmp_file, "libxml.h")
