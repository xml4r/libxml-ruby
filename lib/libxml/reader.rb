module LibXML
  module XML
    class Reader
      # call-seq:
      #    XML::Reader.document(document) -> XML::Reader
      #
      # Creates a new reader by parsing the specified document.
      #
      # Parameters:
      #
      #  document - A preparsed document.
      #
      # Example:
      #
      #  reader = XML::Reader.document(doc)
      def self.document(doc)
        input = XML::Input.new
        input.document = doc
        self.new(input)
      end

      # call-seq:
      #    XML::Reader.file(path) -> XML::Reader
      #    XML::Reader.file(path, :encoding => XML::Input::UTF_8,
      #                           :options => XML::Reader::Options::NOENT) -> XML::Reader
      #
      # Creates a new reader by parsing the specified file or uri.
      #
      # You may provide an optional hash table to control how the
      # parsing is performed.  Valid options are:
      #
      #  encoding - The document encoding, defaults to nil. Valid values
      #             are the encoding constants defined on XML::Input.
      #  options - Controls the execution of the reader, defaults to 0.
      #            Valid values are the constants defined on
      #            XML::Reader::Options.  Mutliple options can be combined
      #            by using Bitwise OR (|).
      def self.file(path, options = {})
        input = XML::Input.new
        input.file = path
        input.encoding = options[:encoding] if options[:encoding]
        input.options = options[:options] if options[:options]
        self.new(input)
      end

      # call-seq:
      #    XML::Reader.string(string)
      #    XML::Reader.string(string, :encoding => XML::Input::UTF_8,
      #                               :options => XML::Reader::Options::NOENT
      #                               :base_url="http://libxml.org") -> XML::Reader
      #
      # Creates a new reader by parsing the specified string.
      #
      # You may provide an optional hash table to control how the
      # parsing is performed.  Valid options are:
      #
      #  base_url - The base url for the parsed document.
      #  encoding - The document encoding, defaults to nil. Valid values
      #             are the encoding constants defined on XML::Input.
      #  options - Controls the execution of the reader, defaults to 0.
      #            Valid values are the constants defined on
      #            XML::Reader::Options.  Mutliple options can be combined
      #            by using Bitwise OR (|).
      def self.string(string, options = {})
        input = XML::Input.new
        input.string = string
        input.base_url = options[:base_url] if options[:base_url]
        input.encoding = options[:encoding] if options[:encoding]
        input.options = options[:options] if options[:options]
        self.new(input)
      end

      # call-seq:
      #    XML::Reader.io(io) -> XML::Reader
      #    XML::Reader.io(io, :encoding => XML::Input::UTF_8,
      #                       :options => XML::Reader::Options::NOENT
      #                       :base_url="http://libxml.org") -> XML::Reader
      #
      # Creates a new reader by parsing the specified io object.
      #
      # Parameters:
      #
      #  io - io object that contains the xml to reader
      #  base_url - The base url for the parsed document.
      #  encoding - The document encoding, defaults to nil. Valid values
      #             are the encoding constants defined on XML::Input.
      #  options - Controls the execution of the reader, defaults to 0.
      #            Valid values are the constants defined on
      #            XML::Reader::Options.  Mutliple options can be combined
      #            by using Bitwise OR (|).
      def self.io(io, options = {})
        input = XML::Input.new
        input.io = io
        input.base_url = options[:base_url] if options[:base_url]
        input.encoding = options[:encoding] if options[:encoding]
        input.options = options[:options] if options[:options]
        self.new(input)
      end

      def self.walker(doc)
        warn('Reader.walker is deprecated.  Use Reader.document instead')
        document(doc)
      end


      def reset_error_handler
        warn('reset_error_handler is deprecated.  Use Error.reset_handler instead')
        Error.reset_handler
      end

      def set_error_handler(&block)
        warn('set_error_handler is deprecated.  Use Error.set_handler instead')
        Error.set_handler(&block)
      end
    end
  end
end