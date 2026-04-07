# Error Handling

libxml-ruby reports errors through the `XML::Error` exception class. Errors from parsing, validation, and other operations all use this class.

## Catching Errors

```ruby
begin
  doc = XML::Parser.string('<broken').parse
rescue XML::Error => e
  puts e.message
end
```

## Error Properties

`XML::Error` provides detailed information:

```ruby
rescue XML::Error => e
  e.message    # human-readable message
  e.domain     # error domain (e.g., XML::Error::PARSER)
  e.code       # error code
  e.level      # severity (WARNING, ERROR, FATAL)
  e.file       # source file
  e.line       # line number
  e.node       # related node (if available)
  e.str1       # additional context
end
```

## Error Levels

```ruby
XML::Error::NONE      # no error
XML::Error::WARNING   # warning
XML::Error::ERROR     # recoverable error
XML::Error::FATAL     # fatal error
```

## Custom Error Handler

Set a global error handler to capture errors that don't raise exceptions (such as validation warnings):

```ruby
XML::Error.set_handler do |error|
  $stderr.puts "XML: #{error.message}"
end
```

To reset to the default handler:

```ruby
XML::Error.reset_handler
```
