require 'xml/libxml'

count = 0
1000.times do
  
  xml_doc = XML::Document.new()
  xml_doc.encoding = "UTF-8"
  xml_doc.root = XML::Node.new("Request")
  
  1000.times do |index|
    
    xml_doc.root << node = XML::Node.new("row")
    node["user_id"] = index.to_s
    node << "600445"
    
  end
  
  xml_str = xml_doc.to_s
  count += 1
  puts count
end
