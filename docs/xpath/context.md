# XPath Context

An `XML::XPath::Context` holds the state for XPath evaluation — registered namespaces, the context node, and an optional object cache. For one-off queries, `doc.find` and `node.find` create a context automatically. For repeated queries, creating a context explicitly avoids redundant setup.

## Creating a Context

```ruby
context = XML::XPath::Context.new(doc)
```

## Registering Namespaces

```ruby
context.register_namespace('atom', 'http://www.w3.org/2005/Atom')
context.register_namespace('dc', 'http://purl.org/dc/elements/1.1/')

entries = context.find('//atom:entry')
```

You can also register from a hash, array, or directly from a node's namespace declarations:

```ruby
# From a hash
context.register_namespaces('atom' => 'http://www.w3.org/2005/Atom')

# From an array of "prefix:uri" strings
context.register_namespaces(['atom:http://www.w3.org/2005/Atom'])

# From a node's namespace declarations
context.register_namespaces_from_node(doc.root)
```

## Setting the Context Node

By default the context node is the document root. Change it to evaluate XPath relative to a different node:

```ruby
context.node = doc.root.find_first('//chapter')
sections = context.find('section')  # relative to chapter
```

## Caching

Enable the XPath object cache to reuse internal XPath objects across queries, reducing allocations:

```ruby
context.enable_cache     # default cache size
context.enable_cache(100) # specific size

# When done
context.disable_cache
```

## Return Types

`context.find` returns different types depending on the XPath expression:

| XPath expression | Ruby return type |
|-----------------|-----------------|
| `//book` | `XML::XPath::Object` (node set) |
| `count(//book)` | `Float` |
| `string(//title)` | `String` |
| `1 = 1` | `true` or `false` |

An `XML::XPath::Object` for a node set is enumerable:

```ruby
results = context.find('//book')
results.each { |node| puts node.name }
results.length
results.first
results[0]
```
