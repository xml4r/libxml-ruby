# $Id: libxml.rb 374 2008-07-11 04:51:41Z cfis $ 
# Please see the LICENSE file for copyright and distribution information 

module XML
  module SiblingEnum
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
end