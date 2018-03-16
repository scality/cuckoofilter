'use strict'

const t = require('tap')
const CuckooFilter = require('.')

const filter = new CuckooFilter()

t.equal(filter.add('hello world'), filter)
t.equal(filter.contain('hello world'), true)
t.equal(filter.contain('not present'), false)
