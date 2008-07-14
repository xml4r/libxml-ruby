require 'libxml'

# test of bug 13310, clears MEM2

include GC

inner = LibXML::Node.new('inner')
save = nil
1.times do
  outer = LibXML::Node.new('outer')
  outer.child = inner
  1.times do
    doc = LibXML::Document.new
    doc.root = outer
    # Uncomment the following line and it won't crash
    save = doc
  end
  garbage_collect
end
garbage_collect
puts inner
