'use strict'

const t = require('tap')
const CuckooFilter = require('.')

const filter = new CuckooFilter()

t.equal(filter.add('hello world'), filter)
t.equal(filter.contain('hello world'), true)

// adding twice
t.equal(filter.add('hello world'), filter)
t.equal(filter.contain('hello world'), true)

// another added thing
t.equal(filter.add('hello matteo'), filter)
t.equal(filter.contain('hello matteo'), true)

t.equal(filter.contain('hello world'), true)
t.equal(filter.contain('not present'), false)

// removing once does delete it
t.equal(filter.delete('hello world'), filter)
t.equal(filter.contain('hello world'), false)

// removing twice deletes it too
t.equal(filter.delete('hello world'), filter)
t.equal(filter.contain('hello world'), false)

// removing twice
t.equal(filter.contain('hello world'), false)

// removing a value only added once
t.equal(filter.delete('hello matteo'), filter)
t.equal(filter.contain('hello matteo'), false)

;(function () {
  var n = 10
  var key = 'something repeated'
  var i

  for (i = 0; i < n; i++) {
    filter.add(key)
  }

  t.equal(filter.contain(key), true)

  for (i = 0; i < n; i++) {
    filter.delete(key)
    t.equal(filter.contain(key), false)
  }
})()
