require "#{File.dirname(__FILE__)}/../ext/xml/libxml" unless defined?(XML)
require 'test/unit'

# TODO this is woefully inadequate

class TC_XML_Parser < Test::Unit::TestCase
  def setup()
    @xp = XML::SaxParser.new
    @test = Hash.new { |h,k| h[k] = [] }
    
    i = 0
       
    @xp.on_start_document { @test[:startdoc] << i+=1 }
    @xp.on_start_element { |name, attr_hash| @test[:startel] << [i+=1,name,attr_hash] }
    @xp.on_characters { |chars| @test[:chars] << [i+=1,chars] }
    @xp.on_comment { |msg| @test[:comment] << [i+=1,msg] }
    @xp.on_processing_instruction { |target, data| @test[:pinstr] << [i+=1, target, data] }
    @xp.on_cdata_block { |cdata| @test[:cdata] << [i+=1,cdata] }
    @xp.on_end_element { |name| @test[:endel] << [i+=1,name] }
    @xp.on_end_document { @test[:enddoc] << i+=1 }    
  end

  def teardown()
    @xp = nil
  end
  
  def test_callbacks_with_string
    @xp.string = File.read(File.join(File.dirname(__FILE__), 'model/saxtest.xml'))
    do_test
  end
    
  def test_callbacks_with_file
    @xp.filename = File.join(File.dirname(__FILE__), 'model/saxtest.xml')
    do_test
  end
    
  def do_test
    @xp.parse
    
    assert_equal [1], @test[:startdoc]
    assert_equal [[2,'test',{'uga'=>'booga','foo'=>'bar'}],[3,'fixnum',{}],[6,'fixnum',{}]],
                 @test[:startel]
    assert_equal [[4,'one'],[7,'two'],[9,"\n  "],[11,"\n  "],[13,"\n  "],[15,"\n"]],
                 @test[:chars]
    assert_equal [[10, ' msg ']], @test[:comment] 
    assert_equal [[12, 'custom', 'foo="bar"']], @test[:pinstr]
    assert_equal [[14, 'here it goes']], @test[:cdata]
    assert_equal [[5,'fixnum'],[8,'fixnum'],[16,'test']], @test[:endel]
    assert_equal [17], @test[:enddoc]    
  end
end # TC_XML_Sax_Parser
