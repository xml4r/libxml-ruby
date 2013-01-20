# encoding: UTF-8

module LibXML
  module XML
    class Error

      # This must be defined before other class constants, so only the
      # constants in the native C-interface is defined.
      ERROR_CODE_MAP = Hash.new.tap do |map|
        (constants -
         [:NO_ERROR, :PARSER, :TREE, :NAMESPACE, :DTD, :HTML, :MEMORY, :OUTPUT, :IO, :FTP, :HTTP, :XINCLUDE, :XPATH, :XPOINTER, :REGEXP, :DATATYPE, :SCHEMASP, :SCHEMASV, :RELAXNGP, :RELAXNGV, :CATALOG, :C14N, :XSLT, :VALID, :CHECK, :WRITER, :MODULE, :I18N, :SCHEMATRONV] - #Domains
         [:NONE, :WARNING, :ERROR, :FATAL] # Levels
         ).each do |code|
          map[const_get(code)] = code.to_s.gsub(/XML_ERR_/, '')
        end
      end
      
      # Verbose error handler
      VERBOSE_HANDLER = lambda do |error|
        STDERR << error.to_s << "\n"
        STDERR.flush
      end

      # Quiet error handler
      QUIET_HANDLER = lambda do |error|
      end
      
      def ==(other)
        eql?(other)
      end
      
      def eql?(other)
        self.code == other.code and
        self.domain == other.domain and
        self.message == other.message and
        self.level == other.level and
        self.file == other.file and
        self.line == other.line and
        self.str1 == other.str1 and
        self.str2 == other.str2 and
        self.str3 == other.str3 and
        self.int1 == other.int1 and
        self.int2 == other.int2 and
        self.ctxt == other.ctxt and
        self.node == other.node
      end

      def level_to_s
        case self.level
          when NONE
            ''
          when WARNING
            'Warning:'
          when ERROR
            'Error:'
          when FATAL
            'Fatal error:'
        end
      end

      def domain_to_s
        const_map = Hash.new
        domains = self.class.constants.grep(/XML_FROM/)
        domains.each do |domain|
          human_name = domain.gsub(/XML_FROM_/, '')
          const_map[self.class.const_get(domain)] = human_name
        end

        const_map[self.domain]
      end

      def code_to_s
        ERROR_CODE_MAP[self.code]
      end

      def to_s
        msg = super
        msg = msg ? msg.strip: ''

        if self.line
          sprintf("%s %s at %s:%d.", self.level_to_s, msg,
                                     self.file, self.line)
        else
          sprintf("%s %s.", self.level_to_s, msg)
        end
      end
    end
  end
end

LibXML::XML::Error.set_handler(&LibXML::XML::Error::VERBOSE_HANDLER)