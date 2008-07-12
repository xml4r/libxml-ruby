# $Id: libxml.rb 374 2008-07-11 04:51:41Z cfis $ 
# Please see the LICENSE file for copyright and distribution information 

module XML
  class Node
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
end
