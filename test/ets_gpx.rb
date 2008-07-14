require 'libxml'

100.times do
  doc = LibXML::Document.new
  doc.encoding = 'UTF-8'

  root = LibXML::Node.new 'gpx'
  root['version'] = '1.0'
  root['creator'] = 'OpenStreetMap.org'
  root['xmlns'] = "http://www.topografix.com/GPX/1/0/"

  doc.root = root

  track = LibXML::Node.new 'trk'
  doc.root << track

  trkseg = LibXML::Node.new 'trkseg'
  track << trkseg

  1.upto(1000) do |n|
    trkpt = LibXML::Node.new 'trkpt'
    trkpt['lat'] = n.to_s
    trkpt['lon'] = n.to_s
    trkseg << trkpt
  end
end
