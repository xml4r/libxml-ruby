# encoding: UTF-8

module LibXML
  module XML
    class Schema::Element
      def min_occurs
        @min
      end

      def max_occurs
        @max
      end

      def required?
        !min_occurs.zero?
      end

      def array?
        max_occurs > 1
      end

      def elements
        type.elements
      end
    end
  end
end
