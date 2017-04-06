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

  describe("addPassword(service, account, password)", function() {
    it("yields true when the service, account, and password are specified, and false when a password already exists", function() {
      return keytar.addPassword(service, account, password)
      .then(function(result) {
        assert.equal(result, true)
        return keytar.addPassword(service, account, "anything")
      })
      .then(function(result) {
        assert.equal(result, false)
      })
    })

    it("calls back with true when the service, account, and password are specified, and false when a password already exists", function(done) {
      keytar.addPassword(service, account, password, function(err, result) {
        if (err) return done(err)
        assert.equal(result, true)
        keytar.addPassword(service, account, "anything", function(err, result) {
          assert.equal(result, false)
          done(err)
        })
      })
    })
  })

  describe("getPassword(service, account)", function() {
    it("yields the password for the service and account", function() {
      return keytar.getPassword(service, account)
      .then(function(result) {
        assert.equal(result, null)
        return keytar.addPassword(service, account, password)
      })
      .then(function(result) {
        assert.equal(result, true)
        return keytar.addPassword(service, account, "anything")
      }).then(function(result) {
        assert.equal(result, false);
        return keytar.getPassword(service, account)
      })
      .then(function(pass) {
        assert.equal(pass, password)
      })
    })

    it('calls back with the password for the service and account', function(done) {
      keytar.addPassword(service, account, password, function(err) {
        if (err) return done(err)
        keytar.getPassword(service, account, function(err, pass) {
          assert.equal(pass, password)
          done(err)
        })
      })
    })
  })

  describe("deletePassword(service, account)", function() {
    it("yields true when the password was deleted, and false when it doesn't exist", function() {
      return keytar.deletePassword(service, account)
      .then(function(result) {
        assert.equal(result, false)
        return keytar.addPassword(service, account, password)
      })
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

    it("calls back with true when the password was deleted, and false when it doesn't exist", function(done) {
      keytar.deletePassword(service, account, function(err, result) {
        if (err) return done(err)
        assert.equal(result, false)
        keytar.addPassword(service, account, password, function(err, result) {
          if (err) return done(err)
          assert.equal(result, true)
          keytar.deletePassword(service, account, function(err, result) {
            assert.equal(result, true)
            done(err)
          })
        })
      })
    })
  })

  describe("replacePassword(service, account, password)", function() {
    it("yields true with the password was replaced, false if it was only added", function() {
      return keytar.replacePassword(service, account, password)
      .then(function(result) {
        assert.equal(result, false)
        return keytar.getPassword(service, account)
      })
      .then(function(result) {
        assert.equal(result, password)
        return keytar.replacePassword(service, account, "another password")
      })
      .then(function(result) {
        assert.equal(result, true)
        return keytar.getPassword(service, account)
      })
      .then(function(result) {
        assert.equal(result, "another password")
      })
    })

    it("calls back with true with the password was replaced, false if it was only added", function(done) {
      return keytar.replacePassword(service, account, password, function(err, result) {
        if (err) return done(err)
        assert.equal(result, false)
        keytar.getPassword(service, account, function(err, result) {
          if (err) return done(err)
          assert.equal(result, password)
          keytar.replacePassword(service, account, "another password", function(err, result) {
            if (err) return done(err)
            assert.equal(result, true)
            keytar.getPassword(service, account, function(err, result) {
              assert.equal(result, "another password")
              done(err)
            })
          })
        })
      })
    })
  })

  describe("findPassword(service)", function() {
    it("yields a password for the service", function() {
      return Promise.all([
        keytar.addPassword(service, account, password),
        keytar.addPassword(service, account2, password2)
      ]).then(function(results) {
        assert.equal(results[0], true)
        assert.equal(results[1], true)
        return keytar.findPassword(service)
      })
      .then(function(result) {
        assert.include([password, password2], result)
      })
    })

    it("calls back with a password for the service", function(done) {
      keytar.addPassword(service, account, password, function(err, result) {
        if (err) return done(err)
        assert.equal(result, true)
        keytar.addPassword(service, account2, password2, function(err, result) {
          if (err) return done(err)
          assert.equal(result, true)
          keytar.findPassword(service, function(err, result) {
            assert.include([password, password2], result)
            done(err)
          })
        })
      })
    })
  })
})
