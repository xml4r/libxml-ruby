module LibXML
  module XML
    class Parser
      class << self
        attr_reader :error_handler
      
        # Register the attached block as the handler for parser errors.
        #
        # Parser.register_error_handler {|msg| <do stuff>}
        # Parser.register_error_handler(lambda {|msg| <do stuff>})
        # Parser.register_error_handler(nil)
        #
        # A message describing parse errors is passed to the block.
        # Libxml passes error messages to the handler in parts, one per call.
        # A typical error results in six calls to this proc, with arguments:
        # 
        #   "Entity: line 1: ", 
        #   "parser ", 
        #   "error : ", 
        #   "Opening and ending tag mismatch: foo line 1 and foz\n",
        #   "<foo><bar/></foz>\n",
        #   "                 ^\n"
        # 
        # Note that the error handler is shared by all threads.
        def register_error_handler(value = nil, &block)
          # Value may be nil or a proc.
          tmp = self.error_handler
          @error_handler = block_given? ? block : value
          tmp
        end

        def xml_error_func_handler(msg)
          if not self.error_handler
            $stderr.puts(msg)
          else
            self.error_handler.call(msg)
          end
        end
      end
    end
  end
end