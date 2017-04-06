var keytar = require('../build/Release/keytar.node')

function checkRequired(val, name) {
  if (!val || val.length <= 0) {
    throw new Error(name + ' is required.');
  }
}

function callbackOrPromise(maybeCallback) {
  if (maybeCallback && typeof maybeCallback === 'function') {
    return {
      value: null,
      callback: function(err, val) {
        if (err) {
          maybeCallback(err)
        } else {
          maybeCallback(err, val)
        }
      }
    }
  } else if (maybeCallback) {
    throw new Error('Callback must be a function.')
  } else {
    var res, rej;
    var promise = new Promise(function(resolve, reject) {
      res = resolve
      rej = reject
    })
    return {
      value: promise,
      callback: function (err, val) {
        if (err) {
          rej(err)
        } else {
          res(val)
        }
      }
    }
  }
}

module.exports = {
  getPassword: function (service, account, callback) {
    checkRequired(service, 'Service')
    checkRequired(account, 'Account')

    var ret = callbackOrPromise(callback)

    keytar.getPassword(service, account, ret.callback)
    return ret.value;
  },

  addPassword: function (service, account, password, callback) {
    checkRequired(service, 'Service')
    checkRequired(account, 'Account')
    checkRequired(password, 'Password')

    var ret = callbackOrPromise(callback)

    keytar.addPassword(service, account, password, ret.callback)
    return ret.value
  },

  deletePassword: function (service, account, callback) {
    checkRequired(service, 'Service')
    checkRequired(account, 'Account')

    var ret = callbackOrPromise(callback)

    keytar.deletePassword(service, account, ret.callback)
    return ret.value
  },

  replacePassword: function (service, account, password, callback) {
    checkRequired(service, 'Service')
    checkRequired(account, 'Account')
    checkRequired(password, 'Password')

    var ret = callbackOrPromise(callback)

    keytar.deletePassword(service, account, function (err, deleted) {
      if (err) {
        return ret.callback(err)
      }
      keytar.addPassword(service, account, password, function (error) {
        ret.callback(error, deleted)
      })
    })
    return ret.value
  },

  findPassword: function (service, callback) {
    checkRequired(service, 'Service')

    var ret = callbackOrPromise(callback)

    keytar.findPassword(service, ret.callback)
    return ret.value
  }
}
