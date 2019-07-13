var assert = require('chai').assert
var keytar = require('../')

describe("keytar", function() {
  var service = 'keytar tests'
  var service2 = 'other tests'
  var account = 'buster'
  var password = 'secret'
  var account2 = 'buster2'
  var password2 = 'secret2'

  var object = {}
  object.toString = function () {
    throw new Error("Whoops! Time to seg fault")
  }

  beforeEach(async function() {
    await keytar.deletePassword(service, account),
    await keytar.deletePassword(service, account2)
    await keytar.deletePassword(service2, account)
  })

  afterEach(async function() {
    await keytar.deletePassword(service, account),
    await keytar.deletePassword(service, account2)
    await keytar.deletePassword(service2, account)
  })

  describe("setPassword/getPassword(service, account)", function() {
    it("sets and yields the password for the service and account", async function() {
      await keytar.setPassword(service, account, password)
      assert.equal(await keytar.getPassword(service, account), password)
      await keytar.setPassword(service, account, password2)
      assert.equal(await keytar.getPassword(service, account), password2)
    })

    it("yields null when the password was not found", async function() {
      assert.equal(await keytar.getPassword(service, account), null)
    })

    describe("error handling", function () {
      describe('setPassword', () => {
        it("handles when an object is provided for service", async function () {
          try {
            await keytar.setPassword(object, account, password)
          } catch (err) {
            assert.equal(err.message, "Parameter 'service' must be a string")
          }
        })

        it("handles when an object is provided for username", async function () {
          try {
            await keytar.setPassword(service, object, password)
          } catch (err) {
            assert.equal(err.message, "Parameter 'username' must be a string")
          }
        })

        it("handles when an object is provided for password", async function () {
          try {
            await keytar.setPassword(service, account, object)
          } catch (err) {
            assert.equal(err.message, "Parameter 'password' must be a string")
          }
        })
      })

      describe('getPassword', () => {
        it("handles when an object is provided for service", async function () {
          try {
            await keytar.getPassword(object, account)
          } catch (err) {
            assert.equal(err.message, "Parameter 'service' must be a string")
          }
        })

        it("handles when an object is provided for username", async function () {
          try {
            await keytar.getPassword(service, object)
          } catch (err) {
            assert.equal(err.message, "Parameter 'username' must be a string")
          }
        })
      })
    })

    describe("Unicode support", function() {
      const service = "se®vi\u00C7e"
      const account = "shi\u0191\u2020ke\u00A5"
      const password = "p\u00E5ssw\u00D8®\u2202"

      it("handles unicode strings everywhere", async function() {
        await keytar.setPassword(service, account, password)
        assert.equal(await keytar.getPassword(service, account), password)
      })

      afterEach(async function() {
        await keytar.deletePassword(service, account)
      })
    })
  })

  describe("setPasswordSync/getPasswordSync(service, account)", function() {
    it("sets and yields the password for the service and account", function() {
      keytar.setPasswordSync(service, account, password)
      assert.equal(keytar.getPasswordSync(service, account), password)
      keytar.setPasswordSync(service, account, password2)
      assert.equal(keytar.getPasswordSync(service, account), password2)
    })

    it("yields null when the password was not found", function() {
      assert.equal(keytar.getPasswordSync(service, account), null)
    })

    describe("error handling", function () {
      describe('setPasswordSync', () => {
        it("handles when an object is provided for service", function () {
          try {
            keytar.setPasswordSync(object, account, password)
          } catch (err) {
            assert.equal(err.message, "Parameter 'service' must be a string")
          }
        })

        it("handles when an object is provided for username", function () {
          try {
            keytar.setPasswordSync(service, object, password)
          } catch (err) {
            assert.equal(err.message, "Parameter 'username' must be a string")
          }
        })

        it("handles when an object is provided for password", function () {
          try {
            keytar.setPasswordSync(service, account, object)
          } catch (err) {
            assert.equal(err.message, "Parameter 'password' must be a string")
          }
        })
      })

      describe('getPasswordSync', () => {
        it("handles when an object is provided for service", function () {
          try {
            keytar.getPasswordSync(object, account)
          } catch (err) {
            assert.equal(err.message, "Parameter 'service' must be a string")
          }
        })

        it("handles when an object is provided for username", function () {
          try {
            keytar.getPasswordSync(service, object)
          } catch (err) {
            assert.equal(err.message, "Parameter 'username' must be a string")
          }
        })
      })
    })

    describe("Unicode support", function() {
      const service = "se®vi\u00C7e"
      const account = "shi\u0191\u2020ke\u00A5"
      const password = "p\u00E5ssw\u00D8®\u2202"

      it("handles unicode strings everywhere", async function() {
        await keytar.setPassword(service, account, password)
        assert.equal(await keytar.getPassword(service, account), password)
      })

      it("handles unicode strings everywhere (Sync)", async function() {
        keytar.setPasswordSync(service, account, password)
        assert.equal(keytar.getPasswordSync(service, account), password)
      })

      afterEach(async function() {
        await keytar.deletePassword(service, account)
      })
    })
  })

  describe("deletePassword(service, account)", function() {
    it("yields true when the password was deleted", async function() {
      await keytar.setPassword(service, account, password)
      assert.equal(await keytar.deletePassword(service, account), true)
    })

    it("yields false when the password didn't exist", async function() {
      assert.equal(await keytar.deletePassword(service, account), false)
    })

    describe("error handling", function () {
      it("handles when an object is provided for service", async function () {
        try {
          await keytar.deletePassword(object, account)
        } catch (err) {
          assert.equal(err.message, "Parameter 'service' must be a string")
        }
      })

      it("handles when an object is provided for username", async function () {
        try {
          await keytar.deletePassword(service, object)
        } catch (err) {
          assert.equal(err.message, "Parameter 'username' must be a string")
        }
      })
    })
  })

  describe("deletePasswordSync(service, account)", function() {
    it("yields true when the password was deleted", function() {
      keytar.setPasswordSync(service, account, password)
      assert.equal(keytar.deletePasswordSync(service, account), true)
    })

    it("yields false when the password didn't exist", function() {
      assert.equal(keytar.deletePasswordSync(service, account), false)
    })

    describe("error handling", function () {
      it("handles when an object is provided for service", function () {
        try {
          keytar.deletePasswordSync(object, account)
        } catch (err) {
          assert.equal(err.message, "Parameter 'service' must be a string")
        }
      })

      it("handles when an object is provided for username", function () {
        try {
          keytar.deletePasswordSync(service, object)
        } catch (err) {
          assert.equal(err.message, "Parameter 'username' must be a string")
        }
      })
    })
  })

  describe("findPassword(service)", function() {
    it("yields a password for the service", async function() {
      await keytar.setPassword(service, account, password),
      await keytar.setPassword(service, account2, password2)
      assert.include([password, password2], await keytar.findPassword(service))
    })

    it("yields null when no password can be found", async function() {
      assert.equal(await keytar.findPassword(service), null)
    })

    it("handles when an object is provided for service", async function () {
      try {
        await keytar.findPassword(object)
      } catch (err) {
        assert.equal(err.message, "Parameter 'service' must be a string")
      }
    })
  })

  describe("findPasswordSync(service)", function() {
    it("yields a password for the service", function() {
      keytar.setPasswordSync(service, account, password),
      keytar.setPasswordSync(service, account2, password2)
      assert.include([password, password2], keytar.findPasswordSync(service))
    })

    it("yields null when no password can be found", function() {
      assert.equal(keytar.findPasswordSync(service), null)
    })

    it("handles when an object is provided for service", function () {
      try {
        keytar.findPasswordSync(object)
      } catch (err) {
        assert.equal(err.message, "Parameter 'service' must be a string")
      }
    })
  })

  describe('findCredentials(service)', function() {
    it('yields an array of the credentials', async function() {
      await keytar.setPassword(service, account, password)
      await keytar.setPassword(service, account2, password2)
      await keytar.setPassword(service2, account, password)

      const found = await keytar.findCredentials(service)
      const sorted = found.sort(function(a, b) {
        return a.account.localeCompare(b.account)
      })

      assert.deepEqual([{account: account, password: password}, {account: account2, password: password2}], sorted)
    });

    it('returns an empty array when no credentials are found', async function() {
      const accounts = await keytar.findCredentials(service)
      assert.deepEqual([], accounts)
    })

    it("handles when an object is provided for service", async function () {
      try {
        await keytar.findCredentials(object)
      } catch (err) {
        assert.equal(err.message, "Parameter 'service' must be a string")
      }
    })

    describe("Unicode support", function() {
      const service = "se®vi\u00C7e"
      const account = "shi\u0191\u2020ke\u00A5"
      const password = "p\u00E5ssw\u00D8®\u2202"

      it("handles unicode strings everywhere", async function() {
        await keytar.setPassword(service, account, password)
        await keytar.setPassword(service, account2, password2)

        const found = await keytar.findCredentials(service)
        const sorted = found.sort(function(a, b) {
          return a.account.localeCompare(b.account)
        })

        assert.deepEqual([{account: account2, password: password2}, {account: account, password: password}], sorted)
      })

      afterEach(async function() {
        await keytar.deletePassword(service, account)
      })
    })
  })

  describe('findCredentialsSync(service)', function() {
    it('yields an array of the credentials', function() {
      keytar.setPasswordSync(service, account, password)
      keytar.setPasswordSync(service, account2, password2)
      keytar.setPasswordSync(service2, account, password)

      const found = keytar.findCredentialsSync(service)
      const sorted = found.sort(function(a, b) {
        return a.account.localeCompare(b.account)
      })

      assert.deepEqual([{account: account, password: password}, {account: account2, password: password2}], sorted)
    });

    it('returns an empty array when no credentials are found', function() {
      const accounts = keytar.findCredentialsSync(service)
      assert.deepEqual([], accounts)
    })

    it("handles when an object is provided for service", function () {
      try {
        keytar.findCredentialsSync(object)
      } catch (err) {
        assert.equal(err.message, "Parameter 'service' must be a string")
      }
    })

    describe("Unicode support", function() {
      const service = "se®vi\u00C7e"
      const account = "shi\u0191\u2020ke\u00A5"
      const password = "p\u00E5ssw\u00D8®\u2202"

      it("handles unicode strings everywhere", async function() {
        keytar.setPasswordSync(service, account, password)
        keytar.setPasswordSync(service, account2, password2)

        const found = keytar.findCredentialsSync(service)
        const sorted = found.sort(function(a, b) {
          return a.account.localeCompare(b.account)
        })

        assert.deepEqual([{account: account2, password: password2}, {account: account, password: password}], sorted)
      })

      afterEach(async function() {
        await keytar.deletePassword(service, account)
      })
    })
  })
})
