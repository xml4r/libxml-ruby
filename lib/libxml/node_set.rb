# $Id: libxml.rb 374 2008-07-11 04:51:41Z cfis $ 
# Please see the LICENSE file for copyright and distribution information 


module LibXML
  module XML
    class Node
      class Set 
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
    end
  end
end  