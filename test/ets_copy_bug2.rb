require 'libxml'

# This will cause a segmentation fault by 
# freeing a xml_node_object twice.  Haven't
# figured out why this is happening yet.
2.times do 
  doc = XML::Document.new('1.0')
  doc.root = XML::Node.new("header")

  doc2 = XML::Document.new('1.0')
  doc2.root = XML::Node.new("footer")
  
  4000.times do 
    doc.root << doc2.root.copy(false)
  end
  
  nodes = doc.find('/header/footer')
  nodes = nil
  
end
puts 'hi'