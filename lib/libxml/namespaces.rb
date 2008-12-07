module LibXML
  module XML
    class Namespaces
      # Register the prefix (i.e., name) of the default namespace to
      # make xpath searches easier.
      def default_prefix=(prefix)
        # Find default prefix
        ns = find_by_prefix(nil)
        raise(ArgumentError, "No default namespace was found") unless ns
        Namespace.new(self.node, prefix, ns.href)
      end
    end
  end
end