#!/usr/bin/ruby -w -I.
require "libxml_test"

$FAILS = []

def test( doc, doc2, iter )
  doc.root = XML::Node.new("ccc")
  iter.times { |i|
    doc.root << doc2.root
    doc.root << doc2.root.copy(true)
  }
  return doc
end

def test2(iter)
  doc = XML::Document.new('1.0')
  doc2 = XML::Document.new('1.0')
  doc2.root = XML::Node.new("aaa")
  ret = test( doc, doc2, iter )

  $FAILS << iter unless ret.find('*/aaa').length == iter
end

1000.times do |i| 
  puts i.to_s
  test2(i)
end

puts "#{$FAILS.length} failures"
p $FAILS


