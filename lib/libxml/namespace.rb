module LibXML
  module XML
    class Namespace
      include Comparable
      include Enumerable

      def <=>(other)
        if self.prefix.nil? and other.prefix.nil?
          self.href <=> other.href
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

      def to_s
        if self.prefix
          "#{self.prefix}:#{self.href}"
        else
          self.href
        end
      end
    end
  end
end