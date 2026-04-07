# XPath and Namespaces

Namespaces are the most common source of confusion with XPath. An element in a namespace will not be found by a bare name — you must register a prefix and use it in the query.

## The Problem

Given this document:

```xml
<feed xmlns="http://www.w3.org/2005/Atom">
  <title>My Feed</title>
  <entry>
    <title>First Post</title>
  </entry>
</feed>
```

This XPath will find nothing:

```ruby
doc.find('//title')  # => empty, because "title" is in the Atom namespace
```

## The Solution: Register a Prefix

```ruby
# Pass a namespace string — "prefix:uri"
doc.find('//atom:title', 'atom:http://www.w3.org/2005/Atom')
```

The prefix you choose (`atom` here) does not have to match the document — it's just a local binding for the XPath query. The URI must match exactly.

## Multiple Namespaces

### Array Form

```ruby
doc.find('//atom:entry/dc:creator', [
  'atom:http://www.w3.org/2005/Atom',
  'dc:http://purl.org/dc/elements/1.1/'
])
```

### Hash Form

```ruby
doc.find('//atom:entry/dc:creator', {
  'atom' => 'http://www.w3.org/2005/Atom',
  'dc'   => 'http://purl.org/dc/elements/1.1/'
})
```

## Default Namespace Shortcut

If you query the same namespace-heavy document repeatedly, set a default prefix:

```ruby
doc.root.namespaces.default_prefix = 'atom'

# Now queries automatically use the 'atom' prefix
doc.find('//atom:title')
doc.find('//atom:entry')
```

## Complex Namespace Example

```xml
<soap:Envelope xmlns:soap="http://schemas.xmlsoap.org/soap/envelope/">
  <soap:Body>
    <getResponse xmlns="http://services.example.com">
      <IDList xmlns="http://services.example.com">
        <ns1:Item xmlns:ns1="http://domain.example.com"/>
      </IDList>
    </getResponse>
  </soap:Body>
</soap:Envelope>
```

```ruby
# soap: is defined on the root, so it's automatically available
doc.find('/soap:Envelope')

# ns1: is defined deeper, register it explicitly
doc.find('//ns1:Item', 'ns1:http://domain.example.com')

# The default namespace on getResponse needs a prefix
doc.find('//svc:getResponse', 'svc:http://services.example.com')

# Combine multiple registrations
doc.find('/soap:Envelope/soap:Body/svc:getResponse/svc:IDList/ns1:Item', [
  'svc:http://services.example.com',
  'ns1:http://domain.example.com'
])
```
