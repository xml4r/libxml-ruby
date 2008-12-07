module LibXML
  module XML
    class NS < Namespace # :nodoc: 
      def initialize(node, prefix, href)
        warn('The XML::NS class is deprecated.  Use XML::Namespace instead.')
        super(node, href, prefix)
      end
    end
  end
end