# $Id: libxml.rb 374 2008-07-11 04:51:41Z cfis $ 
# Please see the LICENSE file for copyright and distribution information 

module LibXML
  module XML
    class Node
      # Return nodes matching the specified xpath expression.
      # For more information, please refer to the documentation
      # for XML::Document#find.
      def find(xpath, nslist = nil)
        if not self.doc
          raise(TypeError, "A node must belong to a document before " +
                           "it can be searched with XPath.")
        end
      
        context = XPath::Context.new(self)
        context.node = self  
        context.register_namespaces_from_node(self)
        context.register_namespaces_from_node(self.doc.root)
        context.register_namespaces(nslist) if nslist

        context.find(xpath)
      end
    
      # Return the first node matching the specified xpath expression.
      # For more information, please refer to the documentation
      # for XML::Node#find.
      def find_first(xpath, nslist = nil)
        find(xpath, nslist).first
      end
    
      def each_element
        each do |node|
          yield(node) if node.node_type == ELEMENT_NODE
        end
      end

      # Determines whether this node has a parent node
      def parent?
        not parent.nil?
      end
    
      # Determines whether this node has a first node
      def first?
        not first.nil?
      end
    
      # Returns this node's children as an array.
      def children
        entries
      end
    
      # Determines whether this node has a next node
      def next?
        not self.next.nil?
      end
    
      # Determines whether this node has a previous node
      def prev?
        not prev.nil?
      end
    
      # Determines whether this node has a last node
      def last?
        not last.nil?
      end

      # Returns this node's type name    
      def node_type_name
        case node_type
          when ELEMENT_NODE:
            'element'
          when ATTRIBUTE_NODE:
            'attribute'
          when TEXT_NODE:
            'text'
          when CDATA_SECTION_NODE:
            'cdata'
          when ENTITY_REF_NODE:
            'entity_ref'
          when ENTITY_NODE:
            'entity'
          when PI_NODE:
            'pi'
          when COMMENT_NODE:
            'comment'
          when DOCUMENT_NODE:
            'document_xml'
          when DOCUMENT_TYPE_NODE:
            'doctype'
          when DOCUMENT_FRAG_NODE:
            'fragment'
          when NOTATION_NODE:
            'notation'
          when HTML_DOCUMENT_NODE:
            'document_html'
          when DTD_NODE:
            'dtd'
          when ELEMENT_DECL:
            'elem_decl'
          when ATTRIBUTE_DECL:
            'attribute_decl'
          when ENTITY_DECL:
            'entity_decl'
          when NAMESPACE_DECL:
            'namespace'
          when XINCLUDE_START:
            'xinclude_start'
          when XINCLUDE_END:
            'xinclude_end'
          when DOCB_DOCUMENT_NODE:
            'document_docbook'
          else
            raise(UnknownType, "Unknown node type: %n", node.node_type);
        end
      end
    
      def dup
        copy(false)
      end
    
      def clone
        copy(false)
      end

      alias :child? :first?  
      alias :children? :first?  
      alias :child :first
      alias :each_child :each
    end
  end
end