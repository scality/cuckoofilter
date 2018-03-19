'use strict'

const t = require('tap')
const CuckooFilter = require('.')

const filter = new CuckooFilter(1024)

t.equal(filter.add('hello world'), filter)
t.equal(filter.contain('hello world'), true)
t.equal(filter.size, 1)

// adding twice
t.equal(filter.add('hello world'), filter)
t.equal(filter.contain('hello world'), true)
t.equal(filter.size, 1)

// another added thing
t.equal(filter.add('hello matteo'), filter)
t.equal(filter.contain('hello matteo'), true)
t.equal(filter.size, 2)

t.equal(filter.contain('hello world'), true)
t.equal(filter.contain('not present'), false)

// removing once does delete it
t.equal(filter.delete('hello world'), filter)
t.equal(filter.contain('hello world'), false)
t.equal(filter.size, 1)

// removing twice deletes it too
t.equal(filter.delete('hello world'), filter)
t.equal(filter.contain('hello world'), false)
t.equal(filter.size, 1)

// removing twice
t.equal(filter.contain('hello world'), false)

// removing a value only added once
t.equal(filter.delete('hello matteo'), filter)
t.equal(filter.contain('hello matteo'), false)
t.equal(filter.size, 0)

;(function () {
  var n = 10
  var key = 'something repeated'
  var i

  for (i = 0; i < n; i++) {
    filter.add(key)
  }

  t.equal(filter.size, 1)

  t.equal(filter.contain(key), true)

  for (i = 0; i < n; i++) {
    filter.delete(key)
    t.equal(filter.contain(key), false)
  }

  t.equal(filter.size, 0)
})()

// current size with the given parameters
t.equal(filter.bytes, 3072)
