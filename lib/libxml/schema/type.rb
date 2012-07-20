module LibXML
  module XML
    class Schema::Type
      def kind_name
        Schema::Types.constants.find { |k| Schema::Types.const_get(k) == kind }
      end

      def facet
        @facet ||= Schema::Facet.create(facets)
      end

      def annotation
        return if node.nil?
        annotations = node.children.select { |n| n.name == 'annotation' }
        annotations.map do |annotation|
          annotation.children.map(&:content).join("\n")
        end.join("\n")
      end
    end
  end
end