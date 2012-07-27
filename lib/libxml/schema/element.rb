# encoding: UTF-8

module LibXML
  module XML
    class Schema::Element

      def max_occurs
        case node['maxOccurs']
        when 'unbounded'
          Float::INFINITY
        when /\d+/
          node['maxOccurs'].to_i
        else
          1
        end
      end

      def min_occurs
        case node['minOccurs']
        when /\d+/
          node['minOccurs'].to_i
        else
          1
        end
      end

      def required?
        min_occurs.zero?
      end

      def array?
        max_occurs > 1
      end

      def elements
        type.elements
      end

      def find_child(name)
        elements[name] || elements.values.find{ |e| e.find_child(name) }
      end

      def annotation
        return if node.nil?
        annotations = node.children.select { |n| n.name == 'annotation' }
        annotations.map do |annotation|
          annotation.children.map(&:content).join("\n")
        end.join("\n").split("\n").delete_if(&:blank?).join("\n")
      end
    end
  end
end
