module LibXML
  module XML
    module XPath
      class Object
        alias :size :length

        def first
          self[0]
        end

        def empty?
          self.length == 0
        end

        def debug
          warn("XPath::Object#debug is no longer available.")
          self
        end

        def set
          warn("XPath::Object#set is deprecated.  Simply use the XPath::Object API instead")
          self
        end
      end
    end
  end
end