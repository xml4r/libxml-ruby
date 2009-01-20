module LibXML
  module XML
    class HTMLParser
      # call-seq:
      #    XML::HtmlParser.document(document) -> XML::HtmlParser
      #
      # Creates a new reader by parsing the specified document.
      #
      # Parameters:
      #
      #  document - A preparsed document.
      #
      # Example:
      #
      #  reader = XML::HtmlParser.document(doc)
      def self.document(doc)
        parser = self.new
        parser.input.document = doc
        parser
      end

      # call-seq:
      #    XML::HtmlParser.file(path) -> XML::HtmlParser
      #    XML::HtmlParser.file(path, :encoding => XML::Input::UTF_8,
      #                           :options => XML::HtmlParser::Options::NOENT) -> XML::HtmlParser
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
      #            XML::HtmlParser::Options.  Mutliple options can be combined
      #            by using Bitwise OR (|).
      def self.file(path, options = {})
        parser = self.new
        parser.input.file = path
        input.encoding = options[:encoding] if options[:encoding]
        input.options = options[:options] if options[:options]
        parser
      end

      # call-seq:
      #    XML::HtmlParser.io(io) -> XML::HtmlParser
      #    XML::HtmlParser.io(io, :encoding => XML::Input::UTF_8,
      #                       :options => XML::HtmlParser::Options::NOENT
      #                       :base_url="http://libxml.org") -> XML::HtmlParser
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
      #            XML::HtmlParser::Options.  Mutliple options can be combined
      #            by using Bitwise OR (|).
      def self.io(io, options = {})
        parser = self.new
        parser.input.io = io
        input.base_url = options[:base_url] if options[:base_url]
        input.encoding = options[:encoding] if options[:encoding]
        input.options = options[:options] if options[:options]
        parser
      end

      # call-seq:
      #    XML::HtmlParser.string(string)
      #    XML::HtmlParser.string(string, :encoding => XML::Input::UTF_8,
      #                               :options => XML::HtmlParser::Options::NOENT
      #                               :base_url="http://libxml.org") -> XML::HtmlParser
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
      #            XML::HtmlParser::Options.  Mutliple options can be combined
      #            by using Bitwise OR (|).
      def self.string(string, options = {})
        parser = self.new
        parser.input.string = string
        input.base_url = options[:base_url] if options[:base_url]
        input.encoding = options[:encoding] if options[:encoding]
        input.options = options[:options] if options[:options]
        parser
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