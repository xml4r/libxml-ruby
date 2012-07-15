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

    end
  end
end