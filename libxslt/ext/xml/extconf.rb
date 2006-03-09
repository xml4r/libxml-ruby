#!/usr/local/bin/ruby -w

# $Id$
#
# See the LICENSE file for copyright and distribution information

require "mkmf"

$preload = nil
$LIBPATH.push(Config::CONFIG['libdir'])

def crash(str)
  print(" extconf failure: %s\n", str)
  exit 1
end

dir_config('xml2')
dir_config('xslt')
#dir_config('libxml-ruby', '../../../libxml', '../../../libxml/ext/xml')

unless have_library('m', 'atan')
  # try again for gcc 4.0
  saveflags = $CFLAGS
  $CFLAGS += ' -fno-builtin'
  unless have_library('m', 'atan')
    crash('need libm')
  end
  $CFLAGS = saveflags
end

unless have_library("z", "inflate")
  crash("need zlib")
else
  $defs.push('-DHAVE_ZLIB_H')
end

unless (have_library('xml2', 'xmlXPtrNewRange') or
        find_library('xml2', '/opt/lib', '/usr/local/lib', '/usr/lib')) and 
       (have_header('libxml/xmlversion.h') or
        find_header('libxml/xmlversion.h', 
                    '/opt/include/libxml2', 
                    '/usr/local/include/libxml2', 
                    '/usr/include/libxml2'))
  crash(<<EOL)
need libxml2.

        Install the library or try one of the following options to extconf.rb:

        --with-xml2-dir=/path/to/libxml2
        --with-xml2-lib=/path/to/libxml2/lib
        --with-xml2-include=/path/to/libxml2/include
EOL
end

unless (have_library('xslt','xsltApplyStylesheet') or
        find_library('xslt', '/opt/lib', '/usr/local/lib', '/usr/lib')) and 
       (have_header('xslt.h') or
        find_header('xslt.h', 
                    '/opt/include/libxslt', 
                    '/usr/local/include/libxslt', 
                    '/usr/include/libxslt'))
  crash(<<EOL)
need libxslt.

        Install the library or try one of the following options to extconf.rb:

        --with-xslt-dir=/path/to/libxslt
        --with-xslt-lib=/path/to/libxslt/lib
        --with-xslt-include=/path/to/libxslt/include
EOL
end

unless have_header('libxml-ruby/libxml.h') 
  crash(<<EOL)
need headers for libxml-ruby.

        If you downloaded a release, this is a bug - please inform
        libxml-devel@rubyforge.org including the release version and
        download URL you obtained it from.

        If you checked libxslt-ruby out from CVS, you will need to
        obtain the headers from CVS (using the same version tag if
        applicable) and place them in directory 'ext/xml/libxml-ruby'.
EOL
end

$LDFLAGS << ' -lexslt'
#$LDFLAGS << ' -lxml'
#$LDFLAGS << ' ' + `xslt-config --libs`.chomp
#$LDFLAGS << ' ' + `xml2-config --libs`.chomp
#$CFLAGS << ' ' + `xslt-config --cflags`.chomp
#$CFLAGS << ' ' + `xml2-config --cflags`.chomp
$CFLAGS = '-g -Wall ' + $CFLAGS + ' ' + $INCFLAGS


create_header()
create_makefile("xml/libxslt")
