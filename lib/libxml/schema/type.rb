module LibXML
  module XML
    class Schema::Type
      def kind_name
        Schema::Types.constants.find { |k| Schema::Types.const_get(k) == kind }
      end

      def facet
        @facet ||= Schema::Facet.create(facets)
      end
    end
  end
end