module XML
  class Parser
    class << self
      attr_accessor :error_handler
      # Register the attached block as the handler for parser errors.
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
        tmp = error_handler
        if block_given?
          error_handler = block
        else
          error_handler = nil
        end
        tmp
      end

      def xml_error_func_handler(msg)
        if not error_handler
          $stderr.puts(msg)
        else
          error_handler.call(msg)
        end
      end
    end
  end
end
