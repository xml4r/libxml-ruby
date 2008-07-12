# $Id: libxml.rb 374 2008-07-11 04:51:41Z cfis $ 
# Please see the LICENSE file for copyright and distribution information 

module XML
  class Document
    include Enumerable

    # maybe, maybe not...
    def each(&blk)
      find('//*').each(&blk)
    end
  end
end