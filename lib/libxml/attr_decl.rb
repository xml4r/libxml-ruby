# $Id: libxml.rb 374 2008-07-11 04:51:41Z cfis $ 
# Please see the LICENSE file for copyright and distribution information 

module LibXML
  module XML
    class AttrDecl
      include Enumerable

      # Returns this node's type name
      def node_type_name
        if node_type == Node::ATTRIBUTE_DECL
          'attribute declaration'
        else
          raise(UnknownType, "Unknown node type: %n", node.node_type);
        end
      end

      def to_s
        "#{name} = #{value}"
      end
    end
  end
end
