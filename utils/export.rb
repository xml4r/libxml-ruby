#!/usr/local/bin/ruby -w

CVSROOT = ':ext:sean@cvs.rubynet.org:/opt/cvsroot/rubynet'
REPO    = 'modules/xml/libxml'

dir = nil
if ARGV[0].nil?
  puts "Usage: #{$0} [snapshot|release]"
  exit(1)
elsif ARGV[0] == 'snapshot'
  File.open("../.rubynet_version") do |f|
    for line in f.readlines
      next if line =~ /^\s*#/ or line.strip.empty?
      LIBVER = line.chomp.succ
      break
    end
  end
  dir = "libxml-#{LIBVER}-snapshot-" + Time.now.strftime('%Y%m%d')
elsif ARGV[0] == 'release'
  LIBVER = `./update_version.rb`.chomp
  dir = "libxml-#{LIBVER}"
end

TAG = 'libxml_0_3_2'
LIBVER = '0.3.2'
dir = "libxml-#{LIBVER}"

print "Checking out..."
$stdout.flush
`cvs -d #{CVSROOT} export -r #{TAG} -d #{dir} #{REPO}`
puts "done."

print "Tar'ing up..."
$stdout.flush
`tar -cvpf #{dir}.tar #{dir}`
puts "done."

print "gzip'ing..."
$stdout.flush
`gzip -9 -f -c #{dir}.tar > #{dir}.tar.gz`
puts "done."

print "bzip'ing..."
$stdout.flush
`bzip2 -9 -f #{dir}.tar`
puts "done."

print "md5'ing..."
$stdout.flush
`/sbin/md5 #{dir}.tar.bz2 > #{dir}.tar.bz2.md5`
`/sbin/md5 #{dir}.tar.gz > #{dir}.tar.gz.md5`
puts "done."

print "scp'ing..."
$stdout.flush
`scp -q #{dir}.tar.bz2 ssh.rubynet.org:/opt/www/data/rubynet/www.rubynet.org/docs/modules/xml/libxml/`
`scp -q #{dir}.tar.bz2.md5 ssh.rubynet.org:/opt/www/data/rubynet/www.rubynet.org/docs/modules/xml/libxml/`
`scp -q #{dir}.tar.gz ssh.rubynet.org:/opt/www/data/rubynet/www.rubynet.org/docs/modules/xml/libxml/`
`scp -q #{dir}.tar.gz.md5 ssh.rubynet.org:/opt/www/data/rubynet/www.rubynet.org/docs/modules/xml/libxml/`
puts "done."

print "Cleaning up..."
$stdout.flush
`rm -rf #{dir} #{dir}.tar.bz2 #{dir}.tar.bz2.md5 #{dir}.tar.gz #{dir}.tar.gz.md5`
puts "done."


puts ""
puts "bzip2:"
puts "http://www.rubynet.org/modules/xml/libxml/#{dir}.tar.bz2"
puts "http://www.rubynet.org/modules/xml/libxml/#{dir}.tar.bz2.md5"
puts ""
puts "gzip:"
puts "http://www.rubynet.org/modules/xml/libxml/#{dir}.tar.gz"
puts "http://www.rubynet.org/modules/xml/libxml/#{dir}.tar.gz.md5"
