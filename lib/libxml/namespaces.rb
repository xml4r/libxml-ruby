module LibXML
  module XML
    class Namespaces
      # call-seq:
      #   namespace.default_prefix = "string"
      #
      # Assigns a name (prefix) to the default namespace.
      # This makes it much easier to perform XML::XPath
      # searches.
      #
      # Usage:
      #   doc = XML::Document.string('<Envelope xmlns="http://schemas.xmlsoap.org/soap/envelope/"/>')
      #   doc.root.namespaces.default_prefix = 'soap'
      #   node = doc.root.find_first('soap:Envelope')
      def default_prefix=(prefix)
        # Find default prefix
        ns = find_by_prefix(nil)
        raise(ArgumentError, "No default namespace was found") unless ns
        Namespace.new(self.node, prefix, ns.href)
      end
    end
  end
end