# encoding: UTF-8

module LibXML
  module XML
    class Schema::Element
      def required?
        !@min || !@min.zero?
      end

      def array?
        @max && @max > 1
      end

      def elements
        type.elements
      end
    end
  end
end
