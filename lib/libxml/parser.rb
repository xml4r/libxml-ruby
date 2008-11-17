module LibXML
  module XML
    class Parser
      def self.filename(value)
        warn("Parser.filename is deprecated.  Use Parser.file instead")
        self.file(value)
      end

      def self.file(value)
        parser = Parser.new
        parser.input.file = value
        parser
      end

      def self.string(value)
        parser = Parser.new
        parser.input.string = value
        parser
      end

      def self.document(value)
        parser = Parser.new
        parser.input.document = value
        parser
      end

      def self.io(value)
        parser = Parser.new
        parser.input.io = value
        parser
      end

      def self.register_error_handler(proc)
        warn('Parser.register_error_handler is deprecated.  Use Error.set_handler instead')
        if proc.nil?
          Error.reset_handler
        else
          Error.set_handler(&proc)
        end
      end

      def filename
        warn("Parser#filename is deprecated.  Use Parser#file instead")
        self.file
      end

      def filename=(value)
        warn("Parser#filename is deprecated.  Use Parser#file instead")
        self.file = value
      end

      def file
        input.file
      end

      def file=(value)
        input.file = value
      end

      def string
        input.string
      end

      def string=(value)
        input.string = value
      end

      def document
        input.document
      end

      def document=(value)
        input.document = value
      end

      def io
        input.io
      end

      def io=(value)
        input.io = value
      end
    end
  end
end