\page specification Specification
\tableofcontents

# Netspeak query language

The Netspeak query syntax as described here should be used as reference. There
might be other syntax information out there, e.g. at netspeak.org, which
provides some syntactical simplifications in form of easier to use wildcards or
operators. However, these modified syntaxes are just front-ends and do not work
with the original Netspeak interface. Here is the truth:

```
?   is a placeholder for exactly one word and can be sequenced to search for
    exaclty two, three, four ... words.

    Example:    how to ? this
                -> how to use this
                -> how to do this
                -> how to cite this

*   is a placeholder for zero or many words.

    Example:    see * works
                -> see how it works
                -> see if it works
                -> see what works

[]  compares options, i.e. it checks each word or phrase between these
    brackets plus the so called empty word at that position in the query.

    Example:    it's [ great well "so good" ]
                -> it's
                -> it's great
                -> it's well
                -> it's so good

{}  checks the order, i.e. it tries to find each permutation of the given
    sequence of words or phrases at that position in the query.

    Example:    for { "very important people" only }
                -> for very important people only
                -> for only very important people

#   searches for alternatives of the word following. This operator requests
    the optional Netspeak hash-dictionary component and uses [] to compare
    each retrieved alternative (except that the empty word is not checked).
    The mapping from word to alternatives is completely up to the user when
    building Netspeak, for netspeak.org we use this operator for a synonym
    search providing the Wordnet dictionary.

    Example:    waiting for #response
                -> waiting for response
                -> waiting for answer
                -> waiting for reply
```

You can combine the introduced wildcards and operators as you want, but with the
exception that you may not place any wildcard within bracket operators. Also
nested brackets are not allowed. As you can see in the examples above you can
quote phrases to be handled as one entity is `[]` and `{}`.

# Index configuration files

Netspeak loads indexes using configuration files. These are small `.properties` files that contain information about the location and directory structure of the index, metadata (like name and language), and runtime parameters like the cache size.

The following section will explain all supported keys.

## Metadata

The following keys hold metadata about the corpus used to create the current index.

- `corpus.key = string` _(required)_

  A unique key to identify the corpus. Two corpora with the same key are always assumed to be equal.

- `corpus.name = string` _(required)_

  The human-readable name of the corpus.

- `corpus.language = string` _(required)_

  The [ISO 639-1 code](https://en.wikipedia.org/wiki/ISO_639-1) of the language of the corpus.

## Performance

The following keys are parameters used to fine-tune performance.

- `cache.capacity = size_t` _(optional)_

  This sets the capacity of Netspeak's main cache: the norm query cache. This LFU cache stores the outputs of the query processor. It has a static capacity that limits the maximum number of items.

  The default cache capacity of the current implementation is 1 million. At this capacity, an empty cache will use about 100MB and a full cache will use about 3GB of memory (depends on the cached queries). Other implementation may use different defaults.

- `search.max-norm-queries = uint32` _(optional)_

  The maximum number of norm queries the queries normalizer is allowed to create.

  Norm queries are the fundamental query units of Netspeak. Each norm queries (if not cached) requires at least one random IO access. The maximum should be chosen with regards to hardware the index is stored on. A maximum of <=1000 for HDDs and ~10k for SDDs is recommended.

  Choosing a large maximum can cause queries containing regexes and synonyms to become very slow (seconds per query; uncached).

  The default is implementation defined. The current implementation has a default of 1000.

- `search.regex.max-matches = uint32` _(optional)_

  The maximum number of regex matches. The current implementation replaces regex queries with a set of matching words (e.g. `route?` may be replaced with `[ router routed ]`). This parameter sets the maximum amount of words each regex query can be replaced with.

  Choosing a large maximum can cause queries containing regexes to become very slow. This is only a maximum and the implementation may choose to use fewer words to (hopefully) stay fast.

  You can set this to 0 to disable regex queries.

  The default is implementation defined. The current implementation has a default of 100.

- `search.regex.max-time = uint32` _(optional)_

  The maximum amount of time in milliseconds Netspeak is allowed to search for regex matches. Choosing a generous duration will lead to more accurate search results but may degrade performance.

  The default is implementation-defined but will generally around be a few milliseconds.

## Paths

The following keys are paths to locate the index.

- `path.to.phrase-index = path` _(optional)_ <br>
  `path.to.phrase-corpus = path` _(optional)_ <br>
  `path.to.phrase-dictionary = path` _(optional)_ <br>
  `path.to.postlist-index = path` _(optional)_ <br>
  `path.to.hash-dictionary = path` _(optional)_ <br>
  `path.to.regex-vocabulary = path` _(optional)_ <br>

  Paths to the individual components of a Netspeak index.

- `path.to.home = path` _(optional)_

  This will set all unset `path.to.xxxx` values to `<path.to.home>/xxxx`.

  If a Netspeak index follows the default index format (all indexes created by Netspeak do), then `path.to.home` can be used to set all index paths at once.

  Note: "unset" means not present in this configuration file. Keys from the `extends` file are not considered.

\note All paths (if not absolute) are relative to the directory the configuration file lives in.

## Casing

- `query.lower-case = bool` _(optional)_

  Whether all queries will be lower-cased. This option is useful if the index only contains lower-case phrases.

  The default is `false`.

## Other

- `extends = path` _(optional)_

  The path of another configuration files.

  All keys that are not present in the current configuration file will be retrieved from this one instead. This can be used to implement inheritance.


# Logging

The `serve` and `proxy` sub-command both support logging. This means that they will log every request they receive and every error produced.

## Tracking ID

Tracking is implemented via optional client metadata. Clients can provide a number to track their session to help improve the service.

The tracking number has to be provided as client metadata using the key `netspeak-tracking-id`. The value of that key has to be a 128bit hexadecimal number matching the following regex: `^[0-9A-Fa-f]{32}$`.

There are no requirements for the actual number but it is recommended for them to be randomly generated.
