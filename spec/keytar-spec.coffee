keytar = require '../lib/keytar'

describe "keytar", ->
  service = 'keytar tests'
  account = 'buster'
  password = 'secret'
  account2 = 'buster2'
  password2 = 'secret2'

  beforeEach ->
    keytar.deletePasswordSync(service, account)
    keytar.deletePasswordSync(service, account2)

  afterEach ->
    keytar.deletePasswordSync(service, account)
    keytar.deletePasswordSync(service, account2)

  describe "addPasswordSync(service, account, password)", ->
    it "returns true when the service, account, and password are specified", ->
      expect(keytar.addPasswordSync(service, account, password)).toBe true

  describe "getPasswordSync(service, account, password)", ->
    it "returns the password for service and account", ->
      expect(keytar.addPasswordSync(service, account, password)).toBe true
      expect(keytar.getPasswordSync(service, account)).toBe password

  describe "deletePasswordSync(service, account)", ->
    it "returns true when the password for the service and account has been deleted", ->
      expect(keytar.deletePasswordSync(service, account)).toBe false
      expect(keytar.addPasswordSync(service, account, password)).toBe true
      expect(keytar.deletePasswordSync(service, account)).toBe true
      expect(keytar.deletePasswordSync(service, account)).toBe false

  describe "replacePasswordSync(service, account, password)", ->
    it "returns true when the password for the service and account has been deleted and readded", ->
      expect(keytar.addPasswordSync(service, account, password)).toBe true
      expect(keytar.replacePasswordSync(service, account, 'another secret')).toBe true
      expect(keytar.getPasswordSync(service, account)).toBe 'another secret'

  describe "findPasswordSync(service)", ->
    it "returns a password of the service", ->
      expect(keytar.addPasswordSync(service, account, password)).toBe true
      expect(keytar.addPasswordSync(service, account2, password2)).toBe true
      found = keytar.findPasswordSync(service)
      expect([password, password2].indexOf(found)).toBeGreaterThan -1
