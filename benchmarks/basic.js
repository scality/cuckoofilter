'use strict'

const benchmark = require('benchmark')
const suite = benchmark.Suite()
const Cuckoo = require('..')
const CuckooFilter = require('cuckoo-filter').CuckooFilter
const BloomFilter = require('bloomfilter').BloomFilter

const cuckoo = new Cuckoo(1024 * 8)
const cuckoofilter = new CuckooFilter(200, 4, 2)

const bloom = new BloomFilter(
  32 * 256,
  16
)

cuckoo.add('hello world')
cuckoofilter.add('hello world')
bloom.add('hello world')

suite.add('cuckoofilter add', function () {
  cuckoo.add('hello world' + Math.random())
})

suite.add('cuckoofilter contain', function () {
  cuckoo.contain('hello world')
})

suite.add('cuckoo-filter add', function () {
  cuckoofilter.add('hello world' + Math.random())
})

suite.add('cuckoo-filter contains', function () {
  cuckoofilter.contains('hello world')
})

suite.add('bloomfilter add', function () {
  bloom.add('hello world' + Math.random())
})

suite.add('bloomfilter contains', function () {
  bloom.test('hello world')
})

suite
  .on('cycle', function (event) {
    console.log(String(event.target))
  })
  .on('complete', function () {})
  .run()
