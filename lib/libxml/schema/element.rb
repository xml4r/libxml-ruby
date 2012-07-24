# encoding: UTF-8

module LibXML
  module XML
    class Schema::Element

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
