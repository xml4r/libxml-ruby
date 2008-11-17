module LibXML
  module XML
    class SaxParser
      def filename=(value)
        warn("SaxParser.filename is deprecated.  Use SaxParser#file instead.")
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