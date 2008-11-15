module LibXML
  module XML
    class Parser
      def self.register_error_handler(proc)
        warn('Parser.register_error_handler is deprecated.  Use Error.set_handler instead')
        if proc.nil?
          Error.reset_handler
        else
          Error.set_handler(&proc)
        end
      end
    end
  end
end