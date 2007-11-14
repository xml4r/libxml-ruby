#!/usr/bin/ruby -w -I.

$FAILS = []

def test( doc, doc2, iter )
  doc.root = XML::Node.new("ccc")
  iter.times { |i|
    doc.root.child_add(doc2.root)
#    doc.root << doc2.root.copy(true)
  }
  return doc
end

def test2(iter)
  doc = XML::Document.new('1.0')
  doc2 = XML::Document.new('1.0')
  doc2.root = XML::Node.new("aaa")
  ret = test( doc, doc2, iter )
  li=ret.find('aaa').length
  $FAILS << [li,iter] unless li == iter
end

test2(5)
1000.times do |i| 
  test2(i)
  print "\r#{i}"; $stdout.flush
end

puts "\n#{$FAILS.length} failures"
p $FAILS


