keytar = require('bindings')('keytar.node')

module.exports =
  getPassword: (service, account) ->
    throw new Error("Service is required.") unless service?.length > 0
    throw new Error("Account is required.") unless account?.length > 0

    keytar.getPassword(service, account)

  addPassword: (service, account, password) ->
    throw new Error("Service is required.") unless service?.length > 0
    throw new Error("Account is required.") unless account?.length > 0
    throw new Error("Password is required.") unless password?.length > 0

    keytar.addPassword(service, account, password)

  deletePassword: (service, account) ->
    throw new Error("Service is required.") unless service?.length > 0
    throw new Error("Account is required.") unless account?.length > 0

    keytar.deletePassword(service, account)

  replacePassword: (service, account, password) ->
    throw new Error("Service is required.") unless service?.length > 0
    throw new Error("Account is required.") unless account?.length > 0
    throw new Error("Password is required.") unless password?.length > 0

    keytar.deletePassword(service, account)
    keytar.addPassword(service, account, password)

  findPassword: (service) ->
    throw new Error("Service is required.") unless service?.length > 0

    keytar.findPassword(service)
