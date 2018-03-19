Cuckoo Filter
============

Cuckoo filter is a Bloom filter replacement for approximated set-membership queries. While Bloom filters are well-known space-efficient data structures to serve queries like "if item x is in a set?", they do not support deletion. Their variances to enable deletion (like counting Bloom filters) usually require much more space.

Cuckoo ﬁlters provide the ﬂexibility to add and remove items dynamically. A cuckoo filter is based on cuckoo hashing (and therefore named as cuckoo filter).  It is essentially a cuckoo hash table storing each key's fingerprint. Cuckoo hash tables can be highly compact, thus a cuckoo filter could use less space than conventional Bloom ﬁlters, for applications that require low false positive rates (< 3%).
For details about the algorithm and citations please use:

["Cuckoo Filter: Practically Better Than Bloom"](http://www.cs.cmu.edu/~binfan/papers/conext14_cuckoofilter.pdf) in proceedings of ACM CoNEXT 2014 by Bin Fan, Dave Andersen and Michael Kaminsky

This is Node.js binding for the amazing
[cuckoofilter](https://github.com/efficient/cuckoofilter) C++ library.

## Install

```
$ npm install cuckoofilter
```

## Usage

Here is a simple example for the basic usage of cuckoo filter.

```js
const CuckooFilter = require('cuckoofilter')

const filter = new CuckooFilter(1024)

filter.add('hello world') // returns the filter
filter.contain('hello world') // returns true
filter.size // returns 1
filter.delete('hello world') // returns the filter
filter.size // now returns 0, as we have deleted
```

## API

A cuckoo filter supports following operations:

*  `add(item)`: insert an item to the filter
*  `contain(item)`: return if item is already in the filter. Note that this method may return false positive results like Bloom filters
*  `delete(item)`: delete the given item from the filter. Note that to use this method, it must be ensured that this item is in the filter (e.g., based on records on external storage); otherwise, a false item may be deleted.
*  `size`: return the total number of items currently in the filter
*  `bytes`: return the filter size in bytes

`item` must be a string, but `Buffer` could be supported in the future.
Feel free to send a PR.

## Benchmarks

This library is fast:

```
cuckoofilter add x 1,097,031 ops/sec ±2.12% (82 runs sampled)
cuckoofilter contain x 3,286,894 ops/sec ±2.08% (84 runs sampled)
cuckoo-filter add x 2,267 ops/sec ±13.26% (16 runs sampled)
cuckoo-filter contains x 31,680 ops/sec ±4.85% (70 runs sampled)
bloomfilter add x 773,499 ops/sec ±2.38% (86 runs sampled)
bloomfilter contains x 1,866,273 ops/sec ±2.20% (85 runs sampled)
```

## License

Apache-2.0
