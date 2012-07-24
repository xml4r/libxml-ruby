module LibXML
  module XML
    class Schema::Facet

      def self.create(facets)
        klass = facets.first.kind_name.to_s.gsub(/\AXML_SCHEMA_FACET_/, '').capitalize
        const_get(klass).new(facets)
      end

      class Base
        attr_reader :facets

        def initialize(facets)
          @facets = facets
        end

        def value
        end

        def random
          case value
          when Array
            value.sample
          when Regexp
            value.inspect
          end
        end

        def inspect
          "#<#{self.class.name} #{value}>"
        end
      end

      class Enumeration < Base
        def value
          facets.map(&:value)
        end
      end

      class Pattern < Base
        def value
          Regexp.new(facets.first.value)
        end
      end

      def kind_name
        Schema::Types.constants.find { |k| Schema::Types.const_get(k) == kind }
      end

      def inspect
        r     = super
        r[-1] = " #{kind_name}>"
        r
      end
    end
  end
end