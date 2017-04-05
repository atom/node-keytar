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
      expect(keytar.addPasswordSync(service, account, password)).toBe(true)
    })
  })

  describe("getPasswordSync(service, account, password)", function() {
    it("returns the password for service and account", function() {
      expect(keytar.addPasswordSync(service, account, password)).toBe(true)
      expect(keytar.getPasswordSync(service, account)).toBe(password)
    })
  })

  describe("deletePasswordSync(service, account)", function() {
    it("returns true when the password for the service and account has been deleted", function() {
      expect(keytar.deletePasswordSync(service, account)).toBe(false)
      expect(keytar.addPasswordSync(service, account, password)).toBe(true)
      expect(keytar.deletePasswordSync(service, account)).toBe(true)
      expect(keytar.deletePasswordSync(service, account)).toBe(false)
    })
  })

  describe("replacePasswordSync(service, account, password)", function() {
    it("returns true when the password for the service and account has been deleted and readded", function() {
      expect(keytar.addPasswordSync(service, account, password)).toBe(true)
      expect(keytar.replacePasswordSync(service, account, 'another secret')).toBe(true)
      expect(keytar.getPasswordSync(service, account)).toBe('another secret')
    })
  })

  describe("findPasswordSync(service)", function() {
    it("returns a password of the service", function() {
      expect(keytar.addPasswordSync(service, account, password)).toBe(true)
      expect(keytar.addPasswordSync(service, account2, password2)).toBe(true)
      var found = keytar.findPasswordSync(service)
      expect([password, password2].indexOf(found)).toBeGreaterThan(-1)
    })
  })
})
