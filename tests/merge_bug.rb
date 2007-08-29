#!/usr/local/bin/ruby

require 'libxml_test'
require 'xml/libxml' # needs the ruby code too

class XML::Node
  def first_child_element
    result = nil
    if self.children?
      self.each_child{|node_child| result = node_child if ((result.nil?) && node_child.element?) }
    end
    result
  end
end

#
# Method which generates crash
#
def merge(forms)
  starting_form = forms.shift
  forms.inject(starting_form) do |master_form,form| 
    master_form.find("//body").first << form.find("//body").first.first_child_element
    master_form
  end
end

#
# Run
#  Must tell it how many times to merge a document
#  On my linux box, it takes about 350 to crash it
#  On my OS X box, it takes about 150
#

if (ARGV[0].to_i > 0)
  count = ARGV[0].to_i 
else
  count = 500
end


merge_list = []

count.times do
  merge_list << XML::Parser.string(File.read(File.join(File.dirname(__FILE__), 'model', 'merge_bug_data.xml'))).parse
end

result = merge(merge_list)
puts "Merged!"

#File.open(File.join(File.dirname(__FILE__), 'model', '/generated_form.xml'), "w") do |f|
#  puts "Trying to write to file" 
#  f << result
#  puts "After trying to write to file"
#end

puts "nothing left to do but print this message"
