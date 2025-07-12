# encoding: UTF-8

module LibXML
  module XML
    class HTMLParser
      # call-seq:
      #    XML::HTMLParser.file(path) -> XML::HTMLParser
      #    XML::HTMLParser.file(path, encoding: XML::Encoding::UTF_8,
      #                               options: XML::HTMLParser::Options::NOENT) -> XML::HTMLParser
      #
      # Creates a new parser by parsing the specified file or uri.
      #
      # Parameters:
      #
      #  path - Path to file to parse
      #  encoding - The document encoding, defaults to nil. Valid values
      #             are the encoding constants defined on XML::Encoding.
      #  options - Parser options.  Valid values are the constants defined on
      #            XML::HTMLParser::Options.  Mutliple options can be combined
      #            by using Bitwise OR (|).
      def self.file(path, encoding: nil, options: nil)
        context = XML::HTMLParser::Context.file(path)
        context.encoding = encoding if encoding
        context.options = options if options
        self.new(context)
      end

      # call-seq:
      #    XML::HTMLParser.io(io) -> XML::HTMLParser
      #    XML::HTMLParser.io(io, encoding: XML::Encoding::UTF_8,
      #                           options: XML::HTMLParser::Options::NOENT
      #                           base_uri: "http://libxml.org") -> XML::HTMLParser
      #
      # Creates a new reader by parsing the specified io object.
      #
      # Parameters:
      #
      #  io - io object that contains the xml to parser
      #  base_uri - The base url for the parsed document.
      #  encoding - The document encoding, defaults to nil. Valid values
      #             are the encoding constants defined on XML::Encoding.
      #  options - Parser options.  Valid values are the constants defined on
      #            XML::HTMLParser::Options.  Mutliple options can be combined
      #            by using Bitwise OR (|).
      def self.io(io, base_uri: nil, encoding: nil, options: nil)
        context = XML::HTMLParser::Context.io(io)
        context.base_uri = base_uri if base_uri
        context.encoding = encoding if encoding
        context.options = options if options
        self.new(context)
      end

      # call-seq:
      #    XML::HTMLParser.string(string)
      #    XML::HTMLParser.string(string, encoding: XML::Encoding::UTF_8,
      #                                   options: XML::HTMLParser::Options::NOENT
      #                                   base_uri: "http://libxml.org") -> XML::HTMLParser
      #
      # Creates a new parser by parsing the specified string.
      #
      # Parameters:
      #
      #  string - String to parse
      #  base_uri - The base url for the parsed document.
      #  encoding - The document encoding, defaults to nil. Valid values
      #             are the encoding constants defined on XML::Encoding.
      #  options - Parser options.  Valid values are the constants defined on
      #            XML::HTMLParser::Options.  Mutliple options can be combined
      #            by using Bitwise OR (|).
      def self.string(string, base_uri: nil, encoding: nil, options: nil)
        context = XML::HTMLParser::Context.string(string)
        context.base_uri = base_uri if base_uri
        context.encoding = encoding if encoding
        context.options = options if options
        self.new(context)
      end

      # :enddoc:

      def file=(value)
        warn("XML::HTMLParser#file is deprecated.  Use XML::HTMLParser.file instead")
        @context = XML::HTMLParser::Context.file(value)
      end

      def io=(value)
        warn("XML::HTMLParser#io is deprecated.  Use XML::HTMLParser.io instead")
        @context = XML::HTMLParser::Context.io(value)
      end

      def string=(value)
        warn("XML::HTMLParser#string is deprecated.  Use XML::HTMLParser.string instead")
        @context = XML::HTMLParser::Context.string(value)
      end
    end
  end
end
