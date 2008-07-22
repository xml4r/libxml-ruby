require 'xml'

def test( doc2 )
  doc = XML::Document.new('1.0')
  doc.root = XML::Node.new("ccc")
  doc.root['aaa'] = 'aaa'
  doc.root.child_add(doc2.root.copy(true)) # BUG!
  doc.root << doc2.root.copy(true)
  return doc
end

def test2
  doc2 = XML::Document.new('1.0')
  doc2.root = XML::Node.new("aaa")
  test( doc2 )
end

1000.times { |i|
  print "\r#{i}"; $stdout.flush
  test2
}
