# encoding: UTF-8

module LibXML
  module XML
    class Parser
      # call-seq:
      #    XML::Parser.document(document) -> XML::Parser
      #
      # Creates a new parser for the specified document.
      #
      # Parameters:
      #
      #  document - A preparsed document.
      def self.document(doc)
        context = XML::Parser::Context.document(doc)
        self.new(context)
      end

      # call-seq:
      #    XML::Parser.file(path) -> XML::Parser
      #    XML::Parser.file(path, encoding: XML::Encoding::UTF_8,
      #                           options: XML::Parser::Options::NOENT) -> XML::Parser
      #
      # Creates a new parser for the specified file or uri.
      #
      # Parameters:
      #
      #  path - Path to file
      #  base_uri - The base url for the parsed document.
      #  encoding - The document encoding, defaults to nil. Valid values
      #             are the encoding constants defined on XML::Encoding.
      #  options - Parser options.  Valid values are the constants defined on
      #            XML::Parser::Options.  Mutliple options can be combined
      #            by using Bitwise OR (|).
      def self.file(path, base_uri: nil, encoding: nil, options: nil)
        context = XML::Parser::Context.file(path)
        context.base_uri = base_uri if base_uri
        context.encoding = encoding if encoding
        context.options = options if options
        self.new(context)
      end

      # call-seq:
      #    XML::Parser.io(io) -> XML::Parser
      #    XML::Parser.io(io, encoding: XML::Encoding::UTF_8,
      #                       options: XML::Parser::Options::NOENT
      #                       base_uri: "http://libxml.org") -> XML::Parser
      #
      # Creates a new parser for the specified io object.
      #
      # Parameters:
      #
      #  io - io object that contains the xml to parser
      #  base_uri - The base url for the parsed document.
      #  encoding - The document encoding, defaults to nil. Valid values
      #             are the encoding constants defined on XML::Encoding.
      #  options - Parser options.  Valid values are the constants defined on
      #            XML::Parser::Options.  Mutliple options can be combined
      #            by using Bitwise OR (|).
      def self.io(io, base_uri: nil, encoding: nil, options: nil)
        context = XML::Parser::Context.io(io)
        context.base_uri = base_uri if base_uri
        context.encoding = encoding if encoding
        context.options = options if options
        self.new(context)
      end

      # call-seq:
      #    XML::Parser.string(string)
      #    XML::Parser.string(string, encoding: XML::Encoding::UTF_8,
      #                               options: XML::Parser::Options::NOENT
      #                               base_uri: "http://libxml.org") -> XML::Parser
      #
      # Creates a new parser by parsing the specified string.
      #
      # Parameters:
      #
      #  string - The string to parse
      #  base_uri - The base url for the parsed document.
      #  encoding - The document encoding, defaults to nil. Valid values
      #             are the encoding constants defined on XML::Encoding.
      #  options - Parser options.  Valid values are the constants defined on
      #            XML::Parser::Options.  Multiple options can be combined
      #            by using Bitwise OR (|).
      def self.string(string, base_uri: nil, encoding: nil, options: nil)
        context = XML::Parser::Context.string(string)
        context.base_uri = base_uri if base_uri
        context.encoding = encoding if encoding
        context.options = options if options
        self.new(context)
      end

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