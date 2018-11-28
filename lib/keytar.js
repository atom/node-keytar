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

var path = '../build/Release/keytar.node'
var keytar

function getKeytar() {
  if(!keytar) {
    keytar = require(path)
  }

  return keytar
}

module.exports = {
  setPath: function(keytar) {
    path = keytar;
  },

  getPassword: function (service, account) {
    checkRequired(service, 'Service')
    checkRequired(account, 'Account')

    return callbackPromise(callback => getKeytar().getPassword(service, account, callback))
  },

  setPassword: function (service, account, password) {
    checkRequired(service, 'Service')
    checkRequired(account, 'Account')
    checkRequired(password, 'Password')

    return callbackPromise(callback => getKeytar().setPassword(service, account, password, callback))
  },

  deletePassword: function (service, account) {
    checkRequired(service, 'Service')
    checkRequired(account, 'Account')

    return callbackPromise(callback => getKeytar().deletePassword(service, account, callback))
  },

  findPassword: function (service) {
    checkRequired(service, 'Service')

    return callbackPromise(callback => getKeytar().findPassword(service, callback))
  },

  findCredentials: function (service) {
    checkRequired(service, 'Service')

    return callbackPromise(callback => getKeytar().findCredentials(service, callback))
  }
}
