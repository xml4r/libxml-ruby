require 'libxml'
require 'test/unit'

# TODO this is woefully inadequate

class TC_XML_SaxParser2 < Test::Unit::TestCase
  def setup()
    @xp = XML::SaxParser.new
  end

  def teardown()
    @xp = nil
  end

  def test_string_without_callbacks
    @xp.string = File.read(File.join(File.dirname(__FILE__), 'model/saxtest.xml'))
    assert_equal true, @xp.parse
  end

  def test_file_without_callbacks
    @xp.filename = File.join(File.dirname(__FILE__), 'model/saxtest.xml')
    assert_equal true, @xp.parse
  end

  def test_callbacks_with_string
    @xp.callbacks = TestCaseCallbacks.new
    @xp.string = File.read(File.join(File.dirname(__FILE__), 'model/saxtest.xml'))
    do_test
  end

  def test_callbacks_with_file
    @xp.callbacks = TestCaseCallbacks.new
    @xp.filename = File.join(File.dirname(__FILE__), 'model/saxtest.xml')
    do_test
  end

  def do_test
    @xp.parse

    assert_equal [1], @xp.callbacks.test[:startdoc]
    assert_equal [[2,'test',{'uga'=>'booga','foo'=>'bar'}],[3,'fixnum',{}],[6,'fixnum',{}]],
                 @xp.callbacks.test[:startel]
    assert_equal [[4,'one'],[7,'two'],[9,"\n  "],[11,"\n  "],[13,"\n  "],[15,"\n"]],
                 @xp.callbacks.test[:chars]
    assert_equal [[10, ' msg ']], @xp.callbacks.test[:comment]
    assert_equal [[12, 'custom', 'foo="bar"']], @xp.callbacks.test[:pinstr]
    assert_equal [[14, 'here it goes']], @xp.callbacks.test[:cdata]
    assert_equal [[5,'fixnum'],[8,'fixnum'],[16,'test']], @xp.callbacks.test[:endel]
    assert_equal [17], @xp.callbacks.test[:enddoc]
  end
end # TC_XML_Sax_Parser
