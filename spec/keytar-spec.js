var assert = require('chai').assert
var keytar = require('../')

describe("keytar sync", function() {
  var service = 'keytar tests'
  var account = 'buster'
  var password = 'secret'
  var account2 = 'buster2'
  var password2 = 'secret2'

  beforeEach(function() {
    keytar.deletePasswordSync(service, account)
    keytar.deletePasswordSync(service, account2)
  })

  afterEach(function() {
    keytar.deletePasswordSync(service, account)
    keytar.deletePasswordSync(service, account2)
  })

  describe("addPasswordSync(service, account, password)", function() {
    it("returns true when the service, account, and password are specified", function() {
      assert.equal(keytar.addPasswordSync(service, account, password), true)
    })
  })

  describe("getPasswordSync(service, account, password)", function() {
    it("returns the password for service and account", function() {
      assert.equal(keytar.addPasswordSync(service, account, password), true)
      assert.equal(keytar.getPasswordSync(service, account), password)
    })
  })

  describe("deletePasswordSync(service, account)", function() {
    it("returns true when the password for the service and account has been deleted", function() {
      assert.equal(keytar.deletePasswordSync(service, account), false)
      assert.equal(keytar.addPasswordSync(service, account, password), true)
      assert.equal(keytar.deletePasswordSync(service, account), true)
      assert.equal(keytar.deletePasswordSync(service, account), false)
    })
  })

  describe("replacePasswordSync(service, account, password)", function() {
    it("returns true when the password for the service and account has been deleted and readded", function() {
      assert.equal(keytar.addPasswordSync(service, account, password), true)
      assert.equal(keytar.replacePasswordSync(service, account, 'another secret'), true)
      assert.equal(keytar.getPasswordSync(service, account), 'another secret')
    })
  })

  describe("findPasswordSync(service)", function() {
    it("returns a password of the service", function() {
      assert.equal(keytar.addPasswordSync(service, account, password), true)
      assert.equal(keytar.addPasswordSync(service, account2, password2), true)
      var found = keytar.findPasswordSync(service)
      assert.isAbove([password, password2].indexOf(found), -1)
    })
  })
})
