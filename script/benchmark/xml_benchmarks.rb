# From: https://svn.concord.org/svn/projects/trunk/common/ruby/xml_benchmarks/
#
# Each test has an XML Parser open a 98k XML document 
# and count one type of leaf element (466 entries).
# This is repeated a total of 50 time sfor each Parser.
#
# These results were produced on:
#   MacBook Pro
#   2.33 GHz Intel Core 2 Duo
#   4 GB memory
# running MacOS X 10.5.2
#
# Ruby versions tested:
#   MRI:   ruby 1.8.6 (2007-09-24 patchlevel 111) [universal-darwin9.0]
#   JRuby: ruby 1.8.6 (2008-03-20 rev 6255) [i386-jruby1.1RC3] on Java 1.5.0_13
#   JRuby: ruby 1.8.6 (2008-03-20 rev 6255) [i386-jruby1.1RC3] on Java 1.6.0_03 (Soylatte)
#
# Library versions MRI:
#   libxml-ruby 0.5.2.0
#   hpricot 0.6
#
# Library versions JRuby:
#   hpricot 0.6.161
#   jrexml 0.5.3
#
# Summary data: 100 iterations, timings from second set
#
# Platform and method                             total time
# -----------------------------------------------------------
# JRuby (Java 1.6.0) jdom_document_builder          0.363
# MRI: libxml                                       0.389
# JRuby (Java 1.6.0 server) jdom_document_builder   0.412
# JRuby (server) jdom_document_builder              0.617
# JRuby: jdom_document_builder                      1.451
# MRI: hpricot                                      2.056
# JRuby (Java 1.6.0 server) hpricot                 2.272
# JRuby (Java 1.6.0) hpricot                        2.273
# JRuby (server) hpricot                            3.447
# JRuby: hpricot                                    6.198
# JRuby (Java 1.6.0 server) rexml                   6.251
# JRuby (Java 1.6.0) rexml                          6.356
# MRI: rexml                                        7.624
# JRuby (server) rexml                              9.609
# JRuby: rexml                                     12.944
# 
# Detailed data:
#
# $ ruby ./xml_benchmarks.rb
# 
# Ruby version: MRI 1.8.6 (2007-09-24 rev 111), platform: universal-darwin9.0
# 
# 100 times: Open 98k XML document and count one type of leaf element (466 entries)
# 
# Rehearsal -------------------------------------------
# rexml     7.650000   0.040000   7.690000 (  7.692767)
# hpricot   2.100000   0.030000   2.130000 (  2.133564)
# libxml    0.390000   0.030000   0.420000 (  0.419391)
# --------------------------------- total: 10.240000sec
# 
#               user     system      total        real
# rexml     7.600000   0.010000   7.610000 (  7.624436)
# hpricot   2.040000   0.020000   2.060000 (  2.056690)
# libxml    0.380000   0.010000   0.390000 (  0.389298)
# 
# $ jruby ./xml_benchmarks.rb
# 
# Ruby version: JRuby 1.8.6 (2008-03-20 rev 6255) [i386-jruby1.1RC3], platform: Java: version: 1.5.0_13
# 
# 100 times: Open 98k XML document and count one type of leaf element (466 entries)
# 
# Rehearsal ---------------------------------------------------------
# rexml                  14.063000   0.000000  14.063000 ( 14.063000)
# hpricot                 6.579000   0.000000   6.579000 (  6.579000)
# jdom_document_builder   2.211000   0.000000   2.211000 (  2.211000)
# ----------------------------------------------- total: 22.853000sec
# 
#                             user     system      total        real
# rexml                  12.944000   0.000000  12.944000 ( 12.944000)
# hpricot                 6.198000   0.000000   6.198000 (  6.198000)
# jdom_document_builder   1.450000   0.000000   1.450000 (  1.451000)
# 
# $ jruby -J-server ./xml_benchmarks.rb
# 
# Ruby version: JRuby 1.8.6 (2008-03-20 rev 6255) [i386-jruby1.1RC3], platform: Java: version: 1.5.0_13
# 
# 100 times: Open 98k XML document and count one type of leaf element (466 entries)
# 
# Rehearsal ---------------------------------------------------------
# rexml                  13.212000   0.000000  13.212000 ( 13.212000)
# hpricot                 6.992000   0.000000   6.992000 (  6.993000)
# jdom_document_builder   1.873000   0.000000   1.873000 (  1.873000)
# ----------------------------------------------- total: 22.077000sec
# 
#                             user     system      total        real
# rexml                   9.611000   0.000000   9.611000 (  9.609000)
# hpricot                 3.446000   0.000000   3.446000 (  3.447000)
# jdom_document_builder   0.616000   0.000000   0.616000 (  0.617000)
# 
#
#               *** Now using: Java 1.6.0_03 (Soylatte) ***
# 
# $ jruby ./xml_benchmarks.rb
# 
# Ruby version: JRuby 1.8.6 (2008-03-20 rev 6255) [i386-jruby1.1RC3], platform: Java: version: 1.6.0_03-p3
# 
# 100 times: Open 98k XML document and count one type of leaf element (466 entries)
# 
# Rehearsal ---------------------------------------------------------
# rexml                   9.976000   0.000000   9.976000 (  9.976000)
# hpricot                 4.129000   0.000000   4.129000 (  4.129000)
# jdom_document_builder   2.287000   0.000000   2.287000 (  2.287000)
# ----------------------------------------------- total: 16.392000sec
# 
#                             user     system      total        real
# rexml                   6.357000   0.000000   6.357000 (  6.356000)
# hpricot                 2.274000   0.000000   2.274000 (  2.273000)
# jdom_document_builder   0.363000   0.000000   0.363000 (  0.363000)
# 
# $ jruby -J-server ./xml_benchmarks.rb
# 
# Ruby version: JRuby 1.8.6 (2008-03-20 rev 6255) [i386-jruby1.1RC3], platform: Java: version: 1.6.0_03-p3
# 
# 100 times: Open 98k XML document and count one type of leaf element (466 entries)
# 
# Rehearsal ---------------------------------------------------------
# rexml                   9.936000   0.000000   9.936000 (  9.935000)
# hpricot                 4.074000   0.000000   4.074000 (  4.074000)
# jdom_document_builder   1.355000   0.000000   1.355000 (  1.355000)
# ----------------------------------------------- total: 15.365000sec
# 
#                             user     system      total        real
# rexml                   6.251000   0.000000   6.251000 (  6.251000)
# hpricot                 2.273000   0.000000   2.273000 (  2.272000)
# jdom_document_builder   0.412000   0.000000   0.412000 (  0.412000)
#

$:.unshift 'lib'
$:.unshift 'ext/libxml'

__DIR__ = File.dirname(__FILE__)

require 'benchmark'
require 'rexml/document'
require 'hpricot'

if RUBY_PLATFORM =~ /java/ 
    include Java 
    import javax.xml.parsers.DocumentBuilder
    import javax.xml.parsers.DocumentBuilderFactory
    @dbf = DocumentBuilderFactory.new_instance
    @ruby_info = "Ruby version: JRuby #{RUBY_VERSION} (#{RUBY_RELEASE_DATE} rev #{RUBY_PATCHLEVEL}) [i386-jruby#{JRUBY_VERSION}]"
    @ruby_info << ", platform: Java, version #{java.lang.System.getProperty('java.version')}"
  else
    require 'xml'
    @xml_parser = XML::Parser.new
    @ruby_info = "Ruby version: MRI #{RUBY_VERSION} (#{RUBY_RELEASE_DATE}"# rev #{RUBY_PATCHLEVEL})"
    @ruby_info << ", platform: #{RUBY_PLATFORM}"
end

@bundle_with_466_sock_entries = File.read(File.join(__DIR__,"sock_entries.xml"))

def rexml_count_socks
  doc = REXML::Document.new(@bundle_with_466_sock_entries).root
  socks = doc.elements.to_a('./sockEntries').length
end

unless RUBY_PLATFORM =~ /java/ 
  def libxml_count_socks
    @xml_parser.string = @bundle_with_466_sock_entries
    doc = @xml_parser.parse.root
    socks = doc.find('//sockEntries').length
  end
end

def hpricot_count_socks
  doc = Hpricot.XML(@bundle_with_466_sock_entries)
  socks = doc.search("//sockEntries").length
end

if RUBY_PLATFORM =~ /java/ 
  def jdom_document_builder_count_socks
    doc = @dbf.new_document_builder.parse("bundle_with_466_sock_entries.xml").get_document_element   
    socks = doc.getElementsByTagName("sockEntries")
    socks.get_length
  end
end

n = 100
test_iterations = ARGV.first.to_i 
test_iterations = 1 unless test_iterations > 1
puts 
puts @ruby_info
puts
puts "#{n} times: Open 98k XML document and count one type of leaf element (466 entries)"
puts
print "running benchmark "
if test_iterations == 1 
  puts "once.\n\n"
else
  puts "#{test_iterations} times.\n\n"
end
test_iterations.times do
  Benchmark.bmbm do |x|
    x.report("libxml") { n.times {libxml_count_socks} }  unless RUBY_PLATFORM =~ /java/ 
    x.report("hpricot")  { n.times {hpricot_count_socks} }
    x.report("rexml") { n.times {rexml_count_socks} }
    x.report("jdom_document_builder")  { n.times {jdom_document_builder_count_socks} } if RUBY_PLATFORM =~ /java/ 
  end
  puts
end

#
# jrexml doesn't appear to have any speedup in this test
#
# if RUBY_PLATFORM =~ /java/ 
#   require 'jrexml'
#   def rexml_with_jrexml_count_socks
#     doc = REXML::Document.new(@bundle_with_466_sock_entries).root
#     socks = doc.elements.to_a('./sockEntries').length
#   end
# 
#   puts "\nNow add in JREXML to see if it speeds up rexml\n"
#   Benchmark.bmbm do |x|
#     x.report("rexml+jrexml") { n.times {rexml_with_jrexml_count_socks} }
#   end
#   puts
# end

