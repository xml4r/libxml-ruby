module LibXML
  module XML
    class SaxParser
      # call-seq:
      #    XML::SaxParser.file(path) -> XML::SaxParser
      #    XML::SaxParser.file(path, :encoding => XML::Input::UTF_8,
      #                           :options => XML::SaxParser::Options::NOENT) -> XML::SaxParser
      #
      # Creates a new parser by parsing the specified file or uri.
      #
      # You may provide an optional hash table to control how the
      # parsing is performed.  Valid options are:
      #
      #  encoding - The document encoding, defaults to nil. Valid values
      #             are the encoding constants defined on XML::Input.
      #  options - Controls the execution of the parser, defaults to 0.
      #            Valid values are the constants defined on
      #            XML::SaxParser::Options.  Mutliple options can be combined
      #            by using Bitwise OR (|).
      def self.file(path, options = {})
        parser = self.new
        parser.input.file = path
        parser.input.encoding = options[:encoding]
        parser.input.options = options[:options] || 0
        parser
      end

      # call-seq:
      #    XML::SaxParser.io(io) -> XML::SaxParser
      #    XML::SaxParser.io(io, :encoding => XML::Input::UTF_8,
      #                       :options => XML::SaxParser::Options::NOENT
      #                       :base_url="http://libxml.org") -> XML::SaxParser
      #
      # Creates a new reader by parsing the specified io object.
      #
      # Parameters:
      #
      #  io - io object that contains the xml to parser
      #  base_url - The base url for the parsed document.
      #  encoding - The document encoding, defaults to nil. Valid values
      #             are the encoding constants defined on XML::Input.
      #  options - Controls the execution of the parser, defaults to 0.
      #            Valid values are the constants defined on
      #            XML::SaxParser::Options.  Mutliple options can be combined
      #            by using Bitwise OR (|).
      def self.io(io, options = {})
        parser = self.new
        parser.input.io = io
        parser.input.base_url = options[:base_url]
        parser.input.encoding = options[:encoding]
        parser.input.options = options[:options] || 0
        parser
      end

      # call-seq:
      #    XML::SaxParser.string(string)
      #    XML::SaxParser.string(string, :encoding => XML::Input::UTF_8,
      #                               :options => XML::SaxParser::Options::NOENT
      #                               :base_url="http://libxml.org") -> XML::SaxParser
      #
      # Creates a new parser by parsing the specified string.
      #
      # You may provide an optional hash table to control how the
      # parsing is performed.  Valid options are:
      #
      #  base_url - The base url for the parsed document.
      #  encoding - The document encoding, defaults to nil. Valid values
      #             are the encoding constants defined on XML::Input.
      #  options - Controls the execution of the parser, defaults to 0.
      #            Valid values are the constants defined on
      #            XML::SaxParser::Options.  Mutliple options can be combined
      #            by using Bitwise OR (|).
      def self.string(string, options = {})
        parser = self.new
        parser.input.string = string
        parser.input.base_url = options[:base_url]
        parser.input.encoding = options[:encoding]
        parser.input.options = options[:options] || 0
        parser
      end

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