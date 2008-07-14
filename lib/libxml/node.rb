# $Id: libxml.rb 374 2008-07-11 04:51:41Z cfis $ 
# Please see the LICENSE file for copyright and distribution information 

module LibXML
  class Node
    include LibXML::SiblingEnum
    include Enumerable
    
    # Return nodes matching the specified xpath expression.
    # For more information, please refer to the documentation
    # for LibXML::Document#find.
    def find(xpath, nslist = nil)
      if not self.doc
        raise(TypeError, "A node must belong to a document before " +
                         "it can be searched with XPath.")
      end
      
      context = XPath::Context.new(self)
      context.node = self  
      context.register_namespaces_from_node(self)
      context.register_namespaces_from_node(self.doc.root)
      context.register_namespaces(nslist) if nslist

      context.find(xpath)
    end
    
    # Return the first node matching the specified xpath expression.
    # For more information, please refer to the documentation
    # for LibXML::Node#find.
    def find_first(xpath, nslist = nil)
      find(xpath, nslist).first
    end
    
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
