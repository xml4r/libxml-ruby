module LibXML
  module XML
    class Namespace
      include Comparable
      include Enumerable

      def <=>(other)
        if self.prefix.nil? and other.prefix.nil?
          0
        elsif self.prefix.nil?
          -1
        elsif other.prefix.nil?
          1
        else
          self.prefix <=> other.prefix
        end
      end

      def each
        ns = self

        while ns
          yield ns
          ns = ns.next
        end
      end
    end
  end
end