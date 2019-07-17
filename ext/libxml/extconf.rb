#!/usr/bin/env ruby

require 'mkmf'

LIBXML2_VERSION = '2.9.9'.freeze
LIBXML2_SHA256 = '94fb70890143e3c6549f265cee93ec064c80a84c42ad0f23e85ee1fd6540a871'.freeze

def darwin?
  RbConfig::CONFIG['target_os'] =~ /darwin/
end

def crash(str)
  puts(" extconf failure: #{str}")
  exit 1
end

# The gem version constraint in the Rakefile is not respected at install time.
# Keep this version in sync with the one in the Rakefile !
require 'rubygems'
gem 'mini_portile2', '~> 2.4.0'
require 'mini_portile2'
message "Using mini_portile version #{MiniPortile::VERSION}\n"

if darwin?
  have_header('iconv.h') || crash('missing iconv.h')
  have_library('iconv') || crash('missing libiconv')
end
have_library('z', 'gzdopen', 'zlib.h') || crash('missing zlib')
have_library('lzma') || crash('missing lzma')

libxml2_recipe = MiniPortile.new('libxml2', LIBXML2_VERSION)
libxml2_recipe.target = "ports"
libxml2_recipe.host = RbConfig::CONFIG["host"]
# The source tar.gz file is only downloaded once
libxml2_recipe.files = [{
                          url: "http://xmlsoft.org/sources/#{libxml2_recipe.name}-#{libxml2_recipe.version}.tar.gz",
                          sha256: LIBXML2_SHA256
                        }]
libxml2_recipe.configure_options = [
    "--libdir=#{File.join(libxml2_recipe.path, "lib")}",
    "--without-python",
    "--without-readline",
    "--with-c14n",
    "--with-debug",
    "--with-threads",
    "--disable-shared",
    "--enable-static",
    "--with-iconv",
    *(darwin? ? ["RANLIB=/usr/bin/ranlib", "AR=/usr/bin/ar"] : "")
  ]
ENV['CFLAGS'] = "-fPIC #{ENV['CFLAGS']}"

message "Building with libxml2-#{LIBXML2_VERSION}"

checkpoint = "#{libxml2_recipe.target}/#{libxml2_recipe.name}-#{libxml2_recipe.version}-#{libxml2_recipe.host}.installed"
unless File.exist?(checkpoint)
  libxml2_recipe.cook
  FileUtils.touch checkpoint
end
libxml2_recipe.activate
# .activate is supposed to do this, but it doesn't
$LIBPATH = ["#{libxml2_recipe.path}/lib"] | $LIBPATH
append_cflags("-I#{File.join(libxml2_recipe.path, 'include', 'libxml2')}")

have_header('libxml/parser.h') || crash('parser.h not found')
have_library('xml2', 'xmlParseDoc ') || crash('libxml2 not found')
have_func('rb_io_bufwrite', 'ruby/io.h')

create_header()
create_makefile('libxml_ruby')
