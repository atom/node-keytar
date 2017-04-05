var keytar = require('../build/Release/keytar.node')

function checkRequired(val, name) {
  if (!val || val.length <= 0) {
    throw new Error(name + ' is required.');
  }
}

module.exports = {
  getPasswordSync: function (service, account) {
    checkRequired(service, 'Service')
    checkRequired(account, 'Account')

    return keytar.getPasswordSync(service, account)
  },

  addPasswordSync: function (service, account, password) {
    checkRequired(service, 'Service')
    checkRequired(account, 'Account')
    checkRequired(password, 'Password')

    return keytar.addPasswordSync(service, account, password)
  },

  deletePasswordSync: function (service, account) {
    checkRequired(service, 'Service')
    checkRequired(account, 'Account')

    return keytar.deletePasswordSync(service, account)
  },

  replacePasswordSync: function (service, account, password) {
    checkRequired(service, 'Service')
    checkRequired(account, 'Account')
    checkRequired(password, 'Password')

    keytar.deletePasswordSync(service, account)
    return keytar.addPasswordSync(service, account, password)
  },

  findPasswordSync: function (service) {
    checkRequired(service, 'Service')

    return keytar.findPasswordSync(service)
  }
}
