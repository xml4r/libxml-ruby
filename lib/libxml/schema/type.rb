module LibXML
  module XML
    class Schema::Type
      # call-seq:
      #   type.kind_name -> String
      #
      # Returns the name of the constant that matches this type's +kind+ value.
      #
      #   schema.types['shiporderType'].kind_name
      #   # => :XML_SCHEMA_TYPE_COMPLEX
      def kind_name
        Schema::Types.constants.find { |k| Schema::Types.const_get(k) == kind }
      end

      # call-seq:
      #   type.anonymous_subtypes -> Hash
      #
      # Returns a Hash of child elements whose types are anonymous
      # (inline complex types with no global name).
      #
      #   type.anonymous_subtypes
      #   # => {"shipto" => #<Schema::Element>, "item" => #<Schema::Element>}
      def anonymous_subtypes
        elements.select { |_, e| e.type.name.nil? }
      end

      # call-seq:
      #   type.anonymous_subtypes_recursively -> Array
      #
      # Returns a flattened Array of Hashes mapping qualified names to
      # anonymous Schema::Type instances, descending into nested types.
      #
      #   type.anonymous_subtypes_recursively
      #   # => [{"shipto" => #<Schema::Type>}, {"item" => #<Schema::Type>}]
      def anonymous_subtypes_recursively(parent=nil)
        anonymous_subtypes.map do |element_name, e|
          [{[parent, element_name].compact.join('::') => e.type},
           e.type.anonymous_subtypes_recursively(element_name)]
        end.flatten
      end

    end
  end
end
