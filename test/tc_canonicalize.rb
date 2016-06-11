# encoding: UTF-8
require './test_helper'

class TestCanonicalize < Minitest::Test
  # (www.w3.org) 3.1 PIs, Comments, and Outside of Document Element
  # http://www.w3.org/TR/xml-c14n#Example-OutsideDoc
  def test_canonicalize_with_w3c_c14n_3_1
    given_doc = XML::Document.file('c14n/given/example-1.xml')

    # With Comments
    expected_with_comments = IO.read('c14n/result/with-comments/example-1')
    assert_equal(expected_with_comments, given_doc.canonicalize(:comments => true))

    # Without Comments
    expected_without_comments = IO.read('c14n/result/without-comments/example-1')
    assert_equal(expected_without_comments, given_doc.canonicalize)
    assert_equal(expected_without_comments, given_doc.canonicalize(:comments => false))

    # Without Comments (XML_C14N_1_1)
    expected_1_1_without_comments = IO.read('c14n/result/1-1-without-comments/example-1')
    mode = XML::Document::XML_C14N_1_1
    assert_equal(expected_1_1_without_comments, given_doc.canonicalize(:mode => mode))
  end#test_canonicalize_with_w3c_c14n_3_1


  # (www.w3.org) 3.2 Whitespace in Document Content
  # http://www.w3.org/TR/xml-c14n#Example-WhitespaceInContent
  def test_canonicalize_with_w3c_c14n_3_2
    given_doc = XML::Document.file('c14n/given/example-2.xml')
    expected = IO.read('c14n/result/without-comments/example-2')
    assert_equal(expected, given_doc.canonicalize)

    expected_1_1_without_comments = IO.read('c14n/result/1-1-without-comments/example-2')
    mode = XML::Document::XML_C14N_1_1
    assert_equal(expected_1_1_without_comments, given_doc.canonicalize(:mode => mode))
  end

  # (www.w3.org) 3.3 Start and End Tags
  # http://www.w3.org/TR/xml-c14n#Example-SETags
  # (2012-02-20) Test failing due to missing 'attr' in 'e9' node.
  #   - Embedded DTD not parsed out of XML, therefore default attributes are
  #     not applied to canonicalization.
  def test_canonicalize_with_w3c_c14n_3_3
    given_doc = XML::Document.file('c14n/given/example-3.xml')
    expected = IO.read('c14n/result/without-comments/example-3')
    assert_equal(expected, given_doc.canonicalize)

    expected_1_1_without_comments = IO.read('c14n/result/1-1-without-comments/example-3')
    mode = XML::Document::XML_C14N_1_1
    assert_equal(expected_1_1_without_comments, given_doc.canonicalize(:mode => mode))
  end

  # (www.w3.org) 3.4 Character Modifications and Character References
  # http://www.w3.org/TR/xml-c14n#Example-Chars
  def test_canonicalize_with_w3c_c14n_3_4
    given_doc = XML::Document.file('c14n/given/example-4.xml')
    expected = IO.read('c14n/result/without-comments/example-4')
    assert_equal(expected, given_doc.canonicalize)

    expected_1_1_without_comments = IO.read('c14n/result/1-1-without-comments/example-4')
    mode = XML::Document::XML_C14N_1_1
    assert_equal(expected_1_1_without_comments, given_doc.canonicalize(:mode => mode))
  end

  # (www.w3.org) 3.5 Entity References
  # http://www.w3.org/TR/xml-c14n#Example-Entities
  # (2012-02-20) Failing likely due to a logic error
  #   - libxml2(c14n.c:1788) XML_ENTITY_REF_NODE is invalid node for parsing.
  def test_canonicalize_with_w3c_c14n_3_5
    given_doc = XML::Document.file('c14n/given/example-5.xml')

    # With Comments
    expected_with_comments = IO.read('c14n/result/with-comments/example-5')

    # TODO - CANNOT COMPLETE TEST unless libxml2 supports additional node types.
    #assert_equal(expected_with_comments, given_doc.canonicalize(:comments => true))

    # Without Comments
    expected_without_comments = IO.read('c14n/result/without-comments/example-5')
    # TODO - CANNOT COMPLETE TEST unless libxml2 supports additional node types.
    #assert_equal(expected_without_comments, given_doc.canonicalize(:comments => false))
    expected_1_1_without_comments = IO.read('c14n/result/1-1-without-comments/example-5')
    mode = XML::Document::XML_C14N_1_1

    # TODO - CANNOT COMPLETE TEST unless libxml2 supports additional node types.
    #assert_equal(expected_1_1_without_comments, given_doc.canonicalize(:mode => mode))
  end

  # (www.w3.org) 3.6 UTF-8 Encoding
  # http://www.w3.org/TR/xml-c14n#Example-UTF8
  def test_canonicalize_with_w3c_c14n_3_6
    given_doc = XML::Document.file('c14n/given/example-6.xml')
    if defined?(Encoding)
      expected = IO.read('c14n/result/without-comments/example-6', :encoding => Encoding::UTF_8)
    else
      expected = IO.read('c14n/result/without-comments/example-6')
    end
    assert_equal(expected, given_doc.canonicalize)

    if defined?(Encoding)
      expected_1_1_without_comments = IO.read('c14n/result/1-1-without-comments/example-6', :encoding => Encoding::UTF_8)
    else
      expected_1_1_without_comments = IO.read('c14n/result/1-1-without-comments/example-6')
    end
    mode = XML::Document::XML_C14N_1_1
    assert_equal(expected_1_1_without_comments, given_doc.canonicalize(:mode => mode))
  end

  # (www.w3.org) 3.7 Document Subsets
  # http://www.w3.org/TR/xml-c14n#Example-DocSubsets
  def test_canonicalize_with_w3c_c14n_3_7
    # Non Canonicalized Document
    given_doc = XML::Document.file('c14n/given/example-7.xml')
    expected = IO.read('c14n/result/without-comments/example-7')

    e1_node = given_doc.find_first('ietf:e1', 'ietf:http://www.ietf.org')

    # Select current node, all child nodes, all attributes and namespace nodes
    subdoc_nodes = e1_node.find("(.//.|.//@id|namespace::*)")

    # TODO - This fails because the namespace nodes aren't taken into account
    # assert_equal(expected, given_doc.canonicalize(:nodes => subdoc_nodes))
  end
end
