keytar = require('../build/Release/keytar.node')

module.exports =
  getPasswordSync: (service, account) ->
    throw new Error("Service is required.") unless service?.length > 0
    throw new Error("Account is required.") unless account?.length > 0

    keytar.getPasswordSync(service, account)

  addPasswordSync: (service, account, password) ->
    throw new Error("Service is required.") unless service?.length > 0
    throw new Error("Account is required.") unless account?.length > 0
    throw new Error("Password is required.") unless password?.length > 0

    keytar.addPasswordSync(service, account, password)

  deletePasswordSync: (service, account) ->
    throw new Error("Service is required.") unless service?.length > 0
    throw new Error("Account is required.") unless account?.length > 0

    keytar.deletePasswordSync(service, account)

  replacePasswordSync: (service, account, password) ->
    throw new Error("Service is required.") unless service?.length > 0
    throw new Error("Account is required.") unless account?.length > 0
    throw new Error("Password is required.") unless password?.length > 0

    keytar.deletePasswordSync(service, account)
    keytar.addPasswordSync(service, account, password)

  findPasswordSync: (service) ->
    throw new Error("Service is required.") unless service?.length > 0

    keytar.findPasswordSync(service)
