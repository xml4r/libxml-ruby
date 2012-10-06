module LibXML
  module XML
    class Schema::Type
      class Container < Array
        attr_reader :container_id

        def self.create(element)
          case element.container_type
          when Schema::Types::XML_SCHEMA_TYPE_SEQUENCE
            Sequence.new element.container_id
          when Schema::Types::XML_SCHEMA_TYPE_CHOICE
            Choice.new element.container_id
          else
            Any.new element.container_id
          end
        end

        def initialize(id)
          @container_id = id
        end

        # @param [LibXML::XML::Schema::Element] element
        def <<(element)
          if element.container_id != @container_id
            find_or_create_container(element) << element
          else
            super(element)
          end
        end

        def find_or_create_container(element)
          find_exist_container(element) || Container.create(element).tap { |c| self.push(c) }
        end

        def find_exist_container(element)
          find { |i| i.is_a?(Container) && element.container_id == i.container_id }
        end

        def choice?
          is_a?(Choice)
        end
      end

      class Any < Container
      end

      class Sequence < Any
      end

      class Choice < Any
        def keys
          map(&:name)
        end

        def choice(data)
          keys.find { |k| data[k] }
        end
      end

      def kind_name
        Schema::Types.constants.find { |k| Schema::Types.const_get(k) == kind }
      end

      def content
        @content ||= begin
          container = Container.create(elements.values.first)

          elements.values.each do |element|
            container << element
          end
          container
        end
      end

      def annotation
        return if node.nil?
        annotations = node.children.select { |n| n.name == 'annotation' }
        annotations.map do |annotation|
          annotation.children.map(&:content).join("\n")
        end.join("\n")
      end

      def annonymus_subtypes
        elements.select { |_, e| e.type.name.nil? }
      end

      def annonymus_subtypes_recursively(parent=nil)
        annonymus_subtypes.map do |element_name, e|
          [{ [parent, element_name].compact.join('::') => e.type },
           e.type.annonymus_subtypes_recursively(element_name)]
        end.flatten
      end

    end
  end
end
