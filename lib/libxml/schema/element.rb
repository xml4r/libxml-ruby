# encoding: UTF-8

module LibXML
  module XML
    class Schema::Element
      # call-seq:
      #   element.min_occurs -> Integer
      #
      # Returns the minimum number of times this element must appear.
      def min_occurs
        @min
      end

      # call-seq:
      #   element.max_occurs -> Integer
      #
      # Returns the maximum number of times this element may appear.
      def max_occurs
        @max
      end

      # call-seq:
      #   element.required? -> true or false
      #
      # Returns whether this element is required (min_occurs > 0).
      def required?
        !min_occurs.zero?
      end

      # call-seq:
      #   element.array? -> true or false
      #
      # Returns whether this element can appear more than once (max_occurs > 1).
      def array?
        max_occurs > 1
      end

      # call-seq:
      #   element.elements -> Hash
      #
      # Returns the child elements of this element's type.
      def elements
        type.elements
      end
    end
  end
end
