#!/usr/bin/ruby -w -I.
require "libxml_test"

def test( doc2 )
  doc = XML::Document.new('1.0')
  doc.root = XML::Node.new("ccc")
  doc.root['aaa'] = 'aaa'
  doc.root << doc2.root # BUG!
  doc.root << doc2.root.copy(true)
  return doc
end

def test2
  doc2 = XML::Document.new('1.0')
  doc2.root = XML::Node.new("aaa")
  test( doc2 )
end

1000.times { |i| puts i.to_s
                test2 }
