var keytar = require('../build/Release/keytar.node')

function checkRequired(val, name) {
  if (!val || val.length <= 0) {
    throw new Error(name + ' is required.');
  }
}

function callbackPromise(callback) {
  if (typeof callback === 'function') {
    return new Promise(function(resolve, reject) {
      callback((err, val) => {
        if (err) {
          reject(err)
        } else {
          resolve(val)
        }
      })
    })
  } else {
    throw new Error('Callback required')
  }
}

module.exports = {
  getPassword: function (service, account) {
    checkRequired(service, 'Service')
    checkRequired(account, 'Account')

    return callbackPromise(callback => keytar.getPassword(service, account, callback))
  },

  getPasswordSync: function (service, account) { 
    checkRequired(service, 'Service')
    checkRequired(account, 'Account')

    return keytar.getPasswordSync(service, account)
  },

  setPassword: function (service, account, password) {
    checkRequired(service, 'Service')
    checkRequired(account, 'Account')
    checkRequired(password, 'Password')

    return callbackPromise(callback => keytar.setPassword(service, account, password, callback))
  },

  setPasswordSync: function (service, account, password) {
    checkRequired(service, 'Service')
    checkRequired(account, 'Account')
    checkRequired(password, 'Password')

    return keytar.setPasswordSync(service, account, password)
  },

  deletePassword: function (service, account) {
    checkRequired(service, 'Service')
    checkRequired(account, 'Account')

    return callbackPromise(callback => keytar.deletePassword(service, account, callback))
  },

  deletePasswordSync: function (service, account) {
    checkRequired(service, 'Service')
    checkRequired(account, 'Account')

    return keytar.deletePasswordSync(service, account)
  },

  findPassword: function (service) {
    checkRequired(service, 'Service')

    return callbackPromise(callback => keytar.findPassword(service, callback))
  },

  findPasswordSync: function (service) {
    checkRequired(service, 'Service')

    return keytar.findPasswordSync(service)
  },

  findCredentials: function (service) {
    checkRequired(service, 'Service')

    return callbackPromise(callback => keytar.findCredentials(service, callback))
  },

  findCredentialsSync: function (service) {
    checkRequired(service, 'Service')

    return keytar.findCredentialsSync(service)
  }
}
