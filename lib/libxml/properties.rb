# $Id: libxml.rb 374 2008-07-11 04:51:41Z cfis $ 
# Please see the LICENSE file for copyright and distribution information 


# This is for backwards compatibility and is DEPRECATED
module LibXML
  module XML
    class Node
      alias :properties :attributes
      alias :properties? :attributes?
    end
  end
end