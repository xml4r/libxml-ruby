#!/usr/bin/env ruby

require 'mkmf'
require 'rubygems'
gem 'mini_portile2', '~> 2.5.0' # Keep this version in sync with the one in the gemspec!
require 'mini_portile2'
message "Using mini_portile2 version #{MiniPortile::VERSION}\n"

LIBXML2_VERSION = '2.9.12'.freeze
LIBXML2_SHA256 = 'c8d6681e38c56f172892c85ddc0852e1fd4b53b4209e7f4ebf17f7e2eae71d92'.freeze

def darwin?
  RbConfig::CONFIG['target_os'] =~ /darwin/
end

def crash(str)
  puts(" extconf failure: #{str}")
  exit 1
end

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
append_cflags("-I#{File.join(libxml2_recipe.path, 'include', 'libxml2')}")

have_header('libxml/parser.h') || crash('parser.h not found')
have_library('xml2', 'xmlParseDoc ') || crash('libxml2 not found')

create_header()
create_makefile('libxml_ruby')
