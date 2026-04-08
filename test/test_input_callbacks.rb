# encoding: UTF-8

require_relative './test_helper'

class InputCallbacksGCStressRepro
  def run
    scheme_name = 'input-callback-gc://'

    callback_class = Class.new do
      def self.document_query(_uri)
        '<root/>'
      end
    end
    LibXML::XML::InputCallbacks.add_scheme(scheme_name, callback_class)
    callback_class = nil

    document = LibXML::XML::Document.file("#{scheme_name}fixture.xml")
    document.root.name
  ensure
    LibXML::XML::InputCallbacks.remove_scheme(scheme_name)
  end
end

class TestInputCallbacks < Minitest::Test
  def setup
    GC.stress = true
  end

  def teardown
    GC.stress = false
  end

  def test_add_scheme_auto_registers_and_survives_gc_stress
    assert_equal('root', InputCallbacksGCStressRepro.new.run)
  end
end
