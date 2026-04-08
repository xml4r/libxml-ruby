# encoding: UTF-8

module LibXML
  module XML
    class Schema::Attribute
      REQUIRED = 1
      OPTIONAL = 2

      # call-seq:
      #   attribute.default -> String or nil
      #
      # Returns the default value for this attribute, or nil if none.
      def default
        node['default']
      end

      # call-seq:
      #   attribute.required? -> true or false
      #
      # Returns whether this attribute is required.
      def required?
        occurs == REQUIRED
      end

    end
  end
end
