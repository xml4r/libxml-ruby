---
layout: post
title: Libxml - Installation
---
## Installation
Libxml-Ruby supports installation via both the standard Ruby extconf install and "Rubygems":http://rubygems.rubyforge.org/ . In either case the requirements are the same - only the installation process differs.

## Prerequisites
libxml requires a few other libraries to be installed in order to build and function properly.

	  * libm      (math routines: very standard)
	  * libz      (zlib)
	  * libiconv
	  * libxml2
	

We recommend GCC 4 be used for the build.

## Installing with gems
Installation via Gems is very simple - just issue the appropriate command, e.g:

      gem install -r libxml-ruby
    

Note that depending on your setup you may need to run this as root:

      su -c 'gem install -r libxml-ruby'
	  

During installation Gems should build and install the extension to the correct location. If there are errors at this stage, Gem will report them and offer you the option to delete the extension.

## Installing from the tarball
Installation from the source tarball is only slightly more complex. Untar the archive somewhere appropriate, and change to the new directory this creates. If your environment requires no special configuration, you can simply type:

      $ rake install
	  

to build, test, and install the extension.

If you do need to pass options to extconf (to support a non-standard libxml2 install location, for example) you will need to use make manually instead:

      $ cd ext/xml
      $ ruby extconf.rb [your-options]
      $ make
      $ [su -c ']make install[']
	  

Once the makefile is generated, the rest of the Rake tasks will function as intended, so you could run 'rake test' prior to installing, for example.

## Installation problems
If you experience problems during your build, please report them to the "mailing list":http://rubyforge.org/mail/?group_id=494 after searching the "archive":http://rubyforge.org/pipermail/libxml-devel/ .
