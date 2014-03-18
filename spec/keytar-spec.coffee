keytar = require '../lib/keytar'

describe "keytar", ->
  service = 'keytar tests'
  account = 'buster'
  password = 'secret'
  account2 = 'buster2'
  password2 = 'secret2'

  beforeEach ->
    keytar.deletePassword(service, account)
    keytar.deletePassword(service, account2)

  afterEach ->
    keytar.deletePassword(service, account)
    keytar.deletePassword(service, account2)

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

  describe "replacePassword(service, account, password)", ->
    it "returns true when the password for the service and account has been deleted and readded", ->
      expect(keytar.addPassword(service, account, password)).toBe true
      expect(keytar.replacePassword(service, account, 'another secret')).toBe true
      expect(keytar.getPassword(service, account)).toBe 'another secret'

  describe "findPassword(service)", ->
    it "returns a password of the service", ->
      expect(keytar.addPassword(service, account, password)).toBe true
      expect(keytar.addPassword(service, account2, password2)).toBe true
      found = keytar.findPassword(service)
      expect([password, password2].indexOf(found)).toBeGreaterThan -1
