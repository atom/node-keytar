# Keytar - Node module to manage system keychain

[![Travis Build Status](https://travis-ci.org/atom/node-keytar.svg?branch=master)](https://travis-ci.org/atom/node-keytar)
[![Windows Build Status](https://ci.appveyor.com/api/projects/status/github/atom/node-keytar?svg=true)](https://ci.appveyor.com/project/Atom/node-keytar)
[![Dependency Status](https://david-dm.org/atom/node-keytar.svg)](https://david-dm.org/atom/node-keytar)

A native Node module to get, add, replace, and delete passwords in system's keychain. On macOS the passwords are managed by the Keychain, on Linux they are managed by the Secret Service API/libsecret, and on Windows they are managed by Credential Vault.

## Installing

```sh
npm install keytar-extra
```

### On Linux

Currently this library uses `libsecret` so you may need to install it before running `npm install`.

Depending on your distribution, you will need to run the following command:

* Debian/Ubuntu: `sudo apt-get install libsecret-1-dev`
* Red Hat-based: `sudo yum install libsecret-devel`
* Arch Linux: `sudo pacman -S libsecret`

## Building

  * Clone the repository
  * Run `npm install`
  * Run `npm test` to run the tests

## Docs

```javascript
const keytar = require('keytar-extra')
```

### Differences in Behavior

- The asynchronous functions in Keytar return Promises, which resolve with data upon successful completion and reject with an error if one occurs.

- The synchronous functions in Keytar return plain values, and throw errors when they encounter issues.

***

### `getPassword(service, account)`

Get the stored password for the `service` and `account`.

`service` - The string service name.

`account` - The string account name.

Returns a `Promise` yielding the string password, or `null` if an entry for the given service and account was not found.

***

### `getPasswordSync(service, account)`

Get the stored password for the `service` and `account`.

`service` - The string service name.

`account` - The string account name.

Yields the string password or `null` if an entry for the given service and account was not found.

***

### `setPassword(service, account, password)`

Save the `password` for the `service` and `account` to the keychain. Adds a new entry if necessary, or updates an existing entry if one exists.

`service` - The string service name.

`account` - The string account name.

`password` - The string password.

Returns a `Promise` yielding nothing, and resolving only after password has been set.

***

### `setPasswordSync(service, account, password)`

Save the `password` for the `service` and `account` to the keychain. Adds a new entry if necessary, or updates an existing entry if one exists.

`service` - The string service name.

`account` - The string account name.

`password` - The string password.

Returns nothing.

***

### `deletePassword(service, account)`

Delete the stored password for the `service` and `account`.

`service` - The string service name.

`account` - The string account name.

Returns a `Promise`, resolving `true` if a password was deleted, or `false` if an entry with the given service and account was not found.

***

### `deletePasswordSync(service, account)`

Delete the stored password for the `service` and `account`.

`service` - The string service name.

`account` - The string account name.

Returns `true` if a password was deleted, or `false` if an entry with the given service and account was not found.

***

### `findCredentials(service)`

Find all accounts and password for the `service` in the keychain.

`service` - The string service name.

Returns a `Promise`, resolving an array of `{ account: 'foo', password: 'bar' }`.

***

### `findCredentialsSync(service)`

Find all accounts and password for the `service` in the keychain.

`service` - The string service name.

Returns an array of `{ account: 'foo', password: 'bar' }`.

***

### `findPassword(service)`

Find a password for the `service` in the keychain. This is ideal for scenarios where an `account` is not required.

`service` - The string service name.

Returns a `Promise`, which resolves to the string password, or `null` if an entry for the given service was not found.

***

### `findPasswordSync(service)`

Find a password for the `service` in the keychain. This is ideal for scenarios where an `account` is not required.

`service` - The string service name.

Returns the string password, or `null` if an entry for the given service was not found.

# License [ MIT ]

```
Copyright (c) 2013 GitHub Inc.

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
```