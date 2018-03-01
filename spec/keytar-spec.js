var assert = require('chai').assert
var keytar = require('../')

describe("keytar", function() {
  var service = 'keytar tests'
  var service2 = 'other tests'
  var account = 'buster'
  var password = 'secret'
  var account2 = 'buster2'
  var password2 = 'secret2'

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

  describe("deletePassword(service, account)", function() {
    it("yields true when the password was deleted", async function() {
      await keytar.setPassword(service, account, password)
      assert.equal(await keytar.deletePassword(service, account), true)
    })

    it("yields false when the password didn't exist", async function() {
      assert.equal(await keytar.deletePassword(service, account), false)
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
  });
})
