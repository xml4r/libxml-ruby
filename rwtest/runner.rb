#!/usr/bin/env ruby
#
# Allsuite for Eclipse and GEM.
$LOAD_PATH.unshift File.dirname(__FILE__)
require "tests/libxml_test"

ALL_TESTS = true
TESTS = File.expand_path(File.dirname(__FILE__))

glob = File.join(TESTS, ENV['TESTS'] || '*.rb')
Dir[glob].each { |fn|
  next if fn =~ %r{runner.rb$}
  puts "test #{fn}"
  f=Process.fork
  if f.nil?
    require fn
    exit
  end
  rss_k=0
  while (px=Process.wait2(f,Process::WNOHANG)).nil?
    sleep 2
    rss_k2=`ps -o rss= -p #{f}`.to_i
    rss_k=(rss_k2>rss_k) ? rss_k2 : rss_k
  end
  pid,status=px
  puts "Process #{pid} exited status #{status.exitstatus}, max rss(k) #{rss_k}"
}
