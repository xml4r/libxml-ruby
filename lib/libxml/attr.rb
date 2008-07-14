# $Id: libxml.rb 374 2008-07-11 04:51:41Z cfis $ 
# Please see the LICENSE file for copyright and distribution information 

module LibXML
  class Attr 
    include LibXML::SiblingEnum
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
end
