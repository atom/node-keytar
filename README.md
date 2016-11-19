# keytar - Node module to manage system keychain

[![Build
Status](https://travis-ci.org/atom/node-keytar.svg?branch=master)](https://travis-ci.org/atom/node-keytar)

A native Node module to get, add, replace, and delete passwords in system's
keychain. On OS X the passwords are managed by the Keychain, on Linux they are
managed by the Secret Service API/libsecret, and on Windows they are managed by Credential Vault.

## Installing

```sh
npm install keytar
```

### On Linux

Currently this library uses `libsecret` so you may need to install it before `npm install`ing.

Depending on your distribution, you will need to run the following command:

* Debian/Ubuntu: `sudo apt-get install libsecret-1-dev`
* Red Hat-based: `sudo yum install libsecret-devel`
* Arch Linux: `sudo pacman -S libsecret`

## Building
  * Clone the repository
  * Run `npm install`
  * Run `grunt` to compile the native and CoffeeScript code
  * Run `grunt test` to run the specs

## Docs

```coffeescript
keytar = require 'keytar'
```

### getPassword(service, account)

Get the stored password for the `service` and `account`.

`service` - The string service name.

`account` - The string account name.

Returns the string password or `null` on failures.

### addPassword(service, account, password)

Add the `password` for the `service` and `account` to the keychain.

`service` - The string service name.

`account` - The string account name.

`password` - The string password.

Returns `true` on success, `false` on failure.

### deletePassword(service, account)

Delete the stored password for the `service` and `account`.

`service` - The string service name.

`account` - The string account name.

Returns `true` if a password has been deleted, `false` on failure.

### replacePassword(service, account, password)

Replace the `password` for the `service` and `account` in the keychain.

This is a simple convenience function that internally calls
`deletePassword(service, account)` followed by
`addPassword(service, account, password)`.

`service` - The string service name.

`account` - The string account name.

`password` - The string password.

Returns `true` on success, `false` on failure.

### findPassword(service)

Find a password for the `service` in the keychain.

`service` - The string service name.

Returns the string password or `null` on failures.
