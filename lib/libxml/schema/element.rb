# encoding: UTF-8

module LibXML
  module XML
    class Schema::Element
      def required?
        !min.zero?
      end

      def array?
        max > 1
      end

      def elements
        type.elements
      end
    end
  end
end
