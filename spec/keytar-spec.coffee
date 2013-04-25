keytar = require '../lib/keytar'

describe "keytar", ->
  service = 'keytar tests'
  account = 'buster'
  password = 'secret'

  beforeEach ->
    keytar.deletePassword(service, account)

  describe "addPassword(service, account, password)", ->
    it "returns true when the service, account, and password are specified", ->
      expect(keytar.addPassword(service, account, password)).toBe true

  describe "getPassword(service, account, password)", ->
    it "returns the password for service and account", ->
      expect(keytar.addPassword(service, account, password)).toBe true
      expect(keytar.getPassword(service, account)).toBe password

  describe "deletePassword(service, account)", ->
    it "returns true when the password for the service and account has been deleted", ->
      expect(keytar.deletePassword(service, account)).toBe false
      expect(keytar.addPassword(service, account, password)).toBe true
      expect(keytar.deletePassword(service, account)).toBe true
      expect(keytar.deletePassword(service, account)).toBe false
