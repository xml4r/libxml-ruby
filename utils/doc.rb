#!/usr/local/bin/ruby -w

raise "Need an class to rubydoc-ify" if ARGV.length == 0

base_modules = []
ObjectSpace.each_object(Module) { |o| base_modules.push(o) }

for arg in ARGV
  require arg
end

new_modules = []
ObjectSpace.each_object(Module) { |o| new_modules.push(o) }
modules = new_modules - base_modules


def make_constants_node(c)
  consts = c.constants
  for a in c.ancestors
    next if a == c
    consts -= a.constants
  end

  constants = XML::Node.new('constants')
  
  consts.each do |const|
    variable = XML::Node.new('variable')
    varname = variable << XML::Node.new('varname')
    varname << const
    variable << XML::Node.new('varvalue')
    variable << XML::Node.new('vardesc')
    constants << variable
  end

  return constants
end

def make_class_public_node(c)
  class_public = XML::Node.new('class_public')
  docmeta = class_public << XML::Node.new('docmeta')
  title = docmeta << XML::Node.new('title')
  title << "Public class Methods for #{c}"  
  sect = class_public << XML::Node.new('sect')
  sect << XML::Node.new('para')
  methods = sect << XML::Node.new('methods')

  c.singleton_methods.each do |meth|
    method = XML::Node.new('method')
    meth_name = method << XML::Node.new('methname')
    meth_name << meth
    method << XML::Node.new('methdesc')
    methparams = method << XML::Node.new('methparams')
    0.upto(c.method(meth).arity) do 
      methparam = XML::Node.new('methparam')
      methparam << XML::Node.new('paramtype')
      methparam << XML::Node.new('paramdesc')
      methparams << methparam
    end
    methods << method
  end

  return class_public
end

def make_instance_public_node(c)
  instance_public = XML::Node.new('instance_public')
  docmeta = instance_public << XML::Node.new('docmeta')
  title = docmeta << XML::Node.new('title')
  title << "Public instance Methods for #{c}"  
  sect = instance_public << XML::Node.new('sect')
  methods = sect << XML::Node.new('methods')

  c.public_instance_methods.each do |meth|
    method = XML::Node.new('method')
    meth_name = method << XML::Node.new('methname')
    meth_name << meth
    method << XML::Node.new('methdesc')
    methparams = method << XML::Node.new('methparams')
    0.upto(c.instance_method(meth).arity) do 
      methparam = XML::Node.new('methparam')
      methparam << XML::Node.new('paramtype')
      methparam << XML::Node.new('paramdesc')
      methparams << methparam
    end
    methods << method
  end

  return instance_public
end


def make_class_node(c)
  class_node = XML::Node.new('class')
  class_node['name'] = c.to_s
  class_node << make_constants_node(c)
  class_node << make_class_public_node(c)
  class_node << make_instance_public_node(c)

  return class_node
end


rubydoc = XML::Document.new('1.0')
rubydoc.root = XML::Node.new('rubydoc')
rubydoc['xmlns:xlink'] = "http://www.w3.org/1999/xlink"
meta = rubydoc.root << XML::Node.new('meta')
pgkname = meta << XML::Node.new('pkgname')
pgkname << ARGV.first
rubynet_url = meta << XML::Node.new('rubynet_url')
rubynet_url['xlink:type'] = 'simple'
rubynet_url['xlink:href'] = 'http://www.rubynet.org/modules/xml/libxslt/'
rubynet_url << 'http://www.rubynet.org/modules/xml/libxslt'
authors = meta << XML::Node.new('authors')
author = authors << XML::Node.new('author')
author << XML::Node.new('name', 'Sean Chittenden')
author << XML::Node.new('email', 'sean@chittenden.org')
author << XML::Node.new('irc_network', 'Open Projects')
author << XML::Node.new('irc_channel', '#ruby-lang')
author << XML::Node.new('irc_handle', 'seanc')

maintainers = meta << XML::Node.new('maintainers')
maintainer = maintainers << XML::Node.new('maintainer')
maintainer << XML::Node.new('name')
maintainer << XML::Node.new('email')
maintainer << XML::Node.new('irc_network')
maintainer << XML::Node.new('irc_channel')
maintainer << XML::Node.new('irc_handle')

version = meta << XML::Node.new('version')
version << XML::Node.new('major_version',0)
version << XML::Node.new('minor_version',3)
version << XML::Node.new('micro_version',0)

meta << XML::Node.new('copyright')
meta << XML::Node.new('license')

introduction = rubydoc.root << XML::Node.new('introduction')
docmeta = introduction << XML::Node.new('docmeta')
title = docmeta << XML::Node.new('title')
title << "libxml's global variables"
sect = introduction << XML::Node.new('sect')
sect << XML::Node.new('para')


tutorial = rubydoc.root << XML::Node.new('tutorial')
docmeta = tutorial << XML::Node.new('docmeta')
title = docmeta << XML::Node.new('title')
title << "libxml's global variables"
sect = tutorial << XML::Node.new('sect')
sect << XML::Node.new('para')

install = rubydoc.root << XML::Node.new('install')
docmeta = install << XML::Node.new('docmeta')
title = docmeta << XML::Node.new('title')
title << "libxml's global variables"
sect = install << XML::Node.new('sect')
sect << XML::Node.new('para')


api = rubydoc.root << XML::Node.new('api')
global_vars = api << XML::Node.new('global_vars')
docmeta = global_vars << XML::Node.new('docmeta')
title = docmeta << XML::Node.new('title')
title << "libxml's global variables"
sect = global_vars << XML::Node.new('sect')
para = sect << XML::Node.new('para')

modules.each do |cla|
  api << make_class_node(cla)
end


doc = rubydoc.to_s.to_a
doc.unshift(doc.first.dup)
doc[1] = '<!DOCTYPE rubydoc SYSTEM "rubydoc.dtd">' + "\n"
print doc.join('')
