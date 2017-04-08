var assert = require('chai').assert
var keytar = require('../')

describe("keytar", function() {
  var service = 'keytar tests'
  var account = 'buster'
  var password = 'secret'
  var account2 = 'buster2'
  var password2 = 'secret2'

  beforeEach(function() {
    return Promise.all([
      keytar.deletePassword(service, account),
      keytar.deletePassword(service, account2)
    ])
  })

  afterEach(function() {
    return Promise.all([
      keytar.deletePassword(service, account),
      keytar.deletePassword(service, account2)
    ])
  })

  describe("setPassword/getPassword(service, account)", function() {
    it("sets and yields the password for the service and account", function() {
      return keytar.setPassword(service, account, password)
      .then(function() {
        return keytar.getPassword(service, account)
      }).then(function(pass) {
        assert.equal(pass, password)
        return keytar.setPassword(service, account, password2)
      }).then(function() {
        return keytar.getPassword(service, account)
      })
      .then(function(pass) {
        assert.equal(pass, password2)
      })
    })

    it("yeilds null when the password was not found", function() {
      return keytar.getPassword(service, account)
      .then(function(result) {
        assert.equal(result, null)
      })
    })
  })

  describe("deletePassword(service, account)", function() {
    it("yields true when the password was deleted", function() {
      return keytar.setPassword(service, account, password)
      .then(function() {
        return keytar.deletePassword(service, account)
      })
      .then(function(result) {
        assert.equal(result, true)
        return keytar.deletePassword(service, account)
      })
      .then(function(result) {
        assert.equal(result, false)
      })
    })

    it("yields false when the password didn't exist", function() {
      return keytar.deletePassword(service, account)
      .then(function(result) {
        assert.equal(result, false)
        return keytar.setPassword(service, account, password)
      })
    })
  })

  describe("findPassword(service)", function() {
    it("yields a password for the service", function() {
      return Promise.all([
        keytar.setPassword(service, account, password),
        keytar.setPassword(service, account2, password2)
      ]).then(function() {
        return keytar.findPassword(service)
      })
      .then(function(result) {
        assert.include([password, password2], result)
      })
    })

    it("yields null when no password can be found", function() {
      return keytar.findPassword(service)
      .then(function(result) {
        assert.equal(result, null)
      })
    })
  })
})
