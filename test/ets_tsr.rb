require 'libxml'

10_000.times {|n|
j=LibXML::Node.new2(nil,"happy#{n}")
  10.times {|r|
    j1=LibXML::Node.new("happy#{r}","farts")
    j.child=j1
  }
}
