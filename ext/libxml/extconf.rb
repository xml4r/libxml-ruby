#!/usr/bin/env ruby

require 'mkmf'

def crash(str)
  printf(" extconf failure: %s\n", str)
  exit 1
end

xc = with_config('xml2-config')
if xc
  cflags = `#{xc} --cflags`.chomp
  if $? != 0
    cflags = nil
  else
    libs = `#{xc} --libs`.chomp
    if $? != 0
      libs = nil
    else
      $CFLAGS += ' ' + cflags
      $libs = libs + " " + $libs
    end
  end
else
  dir_config('xml2')
  pkg_config('libxml-2.0')
end

include_dirs = []
lib_dirs = []

if (msystem_prefix = ENV['MSYSTEM_PREFIX']) && !msystem_prefix.empty?
  include_dirs.concat([
    "#{msystem_prefix}/include/libxml2",
    "#{msystem_prefix}/include"
  ])
  lib_dirs << "#{msystem_prefix}/lib"
end

include_dirs.concat([
  '/opt/include/libxml2',
  '/opt/local/include/libxml2',
  '/opt/homebrew/opt/libxml2/include/libxml2',
  '/usr/local/include/libxml2',
  '/usr/include/libxml2',
  '/usr/local/include',
  '/usr/local/opt/libxml2/include/libxml2'
])

lib_dirs.concat([
  '/opt/lib',
  '/opt/local/lib',
  '/opt/homebrew/opt/libxml2/lib',
  '/usr/lib',
  '/usr/local/lib',
  '/usr/local/opt/libxml2/lib'
])

found_header = find_header('libxml/xmlversion.h', *include_dirs)

found_lib = find_library('xml2', 'xmlParseDoc',
                         *lib_dirs)

found_lib ||= find_library('libxml2', 'xmlParseDoc',
                           *lib_dirs)

if !found_header || !found_lib
    crash(<<~EOL)
      Cannot find libxml2.

      Install the library or try one of the following options to extconf.rb:
  
        --with-xml2-config=/path/to/xml2-config
        --with-xml2-dir=/path/to/libxml2
        --with-xml2-lib=/path/to/libxml2/lib
        --with-xml2-include=/path/to/libxml2/include
    EOL
end

# Optional zlib support via libxml2; defines HAVE_ZLIB_H if available.
unless have_header("zlib.h")
  message "zlib not found: building without compression support\n"
end

create_header()
create_makefile('libxml_ruby')
