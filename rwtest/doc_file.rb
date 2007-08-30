require 'xml/libxml'

# This is related to bug 8337, complaint is on amd64/fbsd
# unknown if it happens on other amd64/os combos

(1..10000).each{|time|
  XML::Document.file(File.join(File.dirname(__FILE__),'test.xml'))
  p time if time % 100 == 0
}
