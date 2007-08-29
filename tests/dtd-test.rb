require "libxml_test"

xp = XML::Parser.new
xp.string = '<?xml version="1.0" encoding="utf-8"?>
<root><head a="ee" id="1">ass<buzz/></head><descr>really big ass</descr></root>'

doc = xp.parse

dtd = XML::Dtd.new('
<!ELEMENT root (head, descr)>
<!ELEMENT head (#PCDATA)>
<!ATTLIST head
  id NMTOKEN #REQUIRED
  a CDATA #IMPLIED
>
<!ELEMENT descr (#PCDATA)>
')

if doc.validate(dtd) { |message, error| puts "#{error ? 'error' : 'warning'} : #{message}" } 
  puts "validation passed"
else
  puts "validation failed"
end

