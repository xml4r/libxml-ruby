# $Id$ 
# Please see the LICENSE file for copyright and distribution information 

require 'libxml_ruby.so'

module XML::XMLParserOptions
  XML_PARSE_RECOVER = 1 # recover on errors
  XML_PARSE_NOENT = 2 # substitute entities
  XML_PARSE_DTDLOAD = 4 # load the external subset
  XML_PARSE_DTDATTR = 8 # default DTD attributes
  XML_PARSE_DTDVALID = 16 # validate with the DTD
  XML_PARSE_NOERROR = 32 # suppress error reports
  XML_PARSE_NOWARNING = 64 # suppress warning reports
  XML_PARSE_PEDANTIC = 128 # pedantic error reporting
  XML_PARSE_NOBLANKS = 256 # remove blank nodes
  XML_PARSE_SAX1 = 512 # use the SAX1 interface internally
  XML_PARSE_XINCLUDE = 1024 # Implement XInclude substitition
  XML_PARSE_NONET = 2048 # Forbid network access
  XML_PARSE_NODICT = 4096 # Do not reuse the context dictionnary
  XML_PARSE_NSCLEAN = 8192 # remove redundant namespaces declarations
  XML_PARSE_NOCDATA = 16384 # merge CDATA as text nodes
  XML_PARSE_NOXINCNODE = 32768 # do not generate XINCLUDE START/END nodes
  XML_PARSE_COMPACT = 65536 # compact small text nodes; no modification of the tree allowed afterwards (will possibly crash if you try to modify the tree)
end

class XML::Document
  include Enumerable

  # maybe, maybe not...
  def each(&blk)
    find('//*').each(&blk)
  end
end

class XML::Node::Set 
  include Enumerable 

  # inefficient, but maybe can find a way to cache the
  # ary and dump on change?
  def [](i, count = nil) 
    if count
      to_a[i,count]
    else
      to_a[i]
    end
  end

  def to_s #:nodoc:
    to_a.to_s
  end
end

module XML::SiblingEnum
  def length
    inject(0) do |result, node| 
      result + 1
    end
  end
  
  private 
  
  # Iterates nodes and attributes
  def siblings(node, &blk)
    if n = node
      loop do
        blk.call(n)
        break unless n = n.next
      end
    end 
  end
end

class XML::Node
  include XML::SiblingEnum
  include Enumerable
  
  # maybe these don't belong on all nodes...
  def each_child(&blk)
    siblings(child, &blk)   
  end

  def each_attr(&blk)
    siblings(properties, &blk)
  end

  # all siblings INCLUDING self
  def each_sibling(&blk)
    siblings(self, &blk)
  end
  
  # I guess this is what you'd expect?
  alias :each :each_child

  def to_a
    inject([]) do |ary,n|
      ary << n
      ary
    end
  end

  def clone
    copy(false)
  end
end

class XML::Attr 
  include XML::SiblingEnum
  include Enumerable

  def each_sibling(&blk)
    siblings(self,&blk)
  end
  
  alias :each_attr :each_sibling
  alias :each :each_sibling
  
  def to_h
    inject({}) do |h,a|
      h[a.name] = a.value
      h
    end
  end

  def to_a
    inject([]) do |ary,a| 
      ary << [a.name, a.value]
      ary
    end
  end
  
  def to_s
    "#{name} = #{value}"
  end
end