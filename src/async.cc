#include <string>
#include <vector>

#include "napi.h"
#include "keytar.h"
#include "async.h"

using keytar::KEYTAR_OP_RESULT;

/* There is currently no reliable way in Napi to identify if it is safe to
 * call into Javascript. The crash in https://github.com/microsoft/vscode/issues/111288
 * occurs because the async worker callbacks are triggered while the node environment
 * is being freed. In the callbacks we try to resolve Javascript promises and if the promise
 * resolution fails, node-addon-api tries to throw create Napi::Error object which will
 * crash.
 * I have tried the following apis, but none of them provide the signal required
 * to avoid entering JS
 * 1) napi_set_instance_data - whose finalizer_cb should be called when env is torn down
 * 2) napi_add_env_cleanup_hook
 * 3) Try accessing node_napi_env->can_call_into_js() - but node_napi_env is not exposed
 * 4) Attempt to check with v8::Isolate::GetCurrentContext()
 * Finally landed on this simple utility that should fail when the environment is being
 * destroyed.
 */
bool CanCallIntoJS(Napi::Env env) {
  Napi::HandleScope scope(env);
  napi_deferred deferred;
  napi_value promise;
  napi_status status = napi_create_promise(env, &deferred, &promise);
  if (status != napi_ok)
    return false;
  return true;
}

SetPasswordWorker::SetPasswordWorker(
  const std::string& service,
  const std::string& account,
  const std::string& password,
  const Napi::Env &env
) : AsyncWorker(env),
    service(service),
    account(account),
    password(password),
    deferred(Napi::Promise::Deferred::New(env)) {}

SetPasswordWorker::~SetPasswordWorker() {}

Napi::Promise SetPasswordWorker::Promise() {
  return deferred.Promise();
}

void SetPasswordWorker::Execute() {
  std::string error;
  KEYTAR_OP_RESULT result = keytar::SetPassword(service,
                                                account,
                                                password,
                                                &error);
  if (result == keytar::FAIL_ERROR) {
    SetError(error.c_str());
  }
}

void SetPasswordWorker::OnOK() {
  Napi::HandleScope scope(Env());
  deferred.Resolve(Env().Undefined());
}

void SetPasswordWorker::OnError(Napi::Error const &error) {
  Napi::HandleScope scope(Env());
  deferred.Reject(error.Value());
}


GetPasswordWorker::GetPasswordWorker(
  const std::string& service,
  const std::string& account,
  const Napi::Env &env
) : AsyncWorker(env),
    service(service),
    account(account),
    deferred(Napi::Promise::Deferred::New(env)) {}

GetPasswordWorker::~GetPasswordWorker() {}

Napi::Promise GetPasswordWorker::Promise() {
  return deferred.Promise();
}

void GetPasswordWorker::Execute() {
  std::string error;
  KEYTAR_OP_RESULT result = keytar::GetPassword(service,
                                                account,
                                                &password,
                                                &error);
  if (result == keytar::FAIL_ERROR) {
    SetError(error.c_str());
  } else if (result == keytar::FAIL_NONFATAL) {
    success = false;
  } else {
    success = true;
  }
}

void GetPasswordWorker::OnOK() {
  Napi::HandleScope scope(Env());
  if (!CanCallIntoJS(Env()))
    return;
  Napi::Value val = Env().Null();
  if (success) {
    val = Napi::String::New(Env(), password.data(),
                            password.length());
  }
  deferred.Resolve(val);
}

void GetPasswordWorker::OnError(Napi::Error const &error) {
  Napi::HandleScope scope(Env());
  if (!CanCallIntoJS(Env()))
    return;
  deferred.Reject(error.Value());
}

DeletePasswordWorker::DeletePasswordWorker(
  const std::string& service,
  const std::string& account,
  const Napi::Env &env
) : AsyncWorker(env),
    service(service),
    account(account),
    deferred(Napi::Promise::Deferred::New(env)) {}

DeletePasswordWorker::~DeletePasswordWorker() {}

Napi::Promise DeletePasswordWorker::Promise() {
  return deferred.Promise();
}

void DeletePasswordWorker::Execute() {
  std::string error;
  KEYTAR_OP_RESULT result = keytar::DeletePassword(service, account, &error);
  if (result == keytar::FAIL_ERROR) {
    SetError(error.c_str());
  } else if (result == keytar::FAIL_NONFATAL) {
    success = false;
  } else {
    success = true;
  }
}

void DeletePasswordWorker::OnOK() {
  Napi::HandleScope scope(Env());
  if (!CanCallIntoJS(Env()))
    return;
  deferred.Resolve(Napi::Boolean::New(Env(), success));
}

void DeletePasswordWorker::OnError(Napi::Error const &error) {
  Napi::HandleScope scope(Env());
  if (!CanCallIntoJS(Env()))
    return;
  deferred.Reject(error.Value());
}

FindPasswordWorker::FindPasswordWorker(
  const std::string& service,
  const Napi::Env &env
) : AsyncWorker(env),
    service(service),
    deferred(Napi::Promise::Deferred::New(env)) {}

FindPasswordWorker::~FindPasswordWorker() {}

Napi::Promise FindPasswordWorker::Promise() {
  return deferred.Promise();
}

void FindPasswordWorker::Execute() {
  std::string error;
  KEYTAR_OP_RESULT result = keytar::FindPassword(service,
                                                 &password,
                                                 &error);
  if (result == keytar::FAIL_ERROR) {
    SetError(error.c_str());
  } else if (result == keytar::FAIL_NONFATAL) {
    success = false;
  } else {
    success = true;
  }
}

void FindPasswordWorker::OnOK() {
  Napi::HandleScope scope(Env());
  if (!CanCallIntoJS(Env()))
    return;
  Napi::Value val = Env().Null();
  if (success) {
    val = Napi::String::New(Env(), password.data(),
                               password.length());
  }
  deferred.Resolve(val);
}

void FindPasswordWorker::OnError(Napi::Error const &error) {
  Napi::HandleScope scope(Env());
  if (!CanCallIntoJS(Env()))
    return;
  deferred.Reject(error.Value());
}

FindCredentialsWorker::FindCredentialsWorker(
  const std::string& service,
  const Napi::Env &env
) : AsyncWorker(env),
    service(service),
    deferred(Napi::Promise::Deferred::New(env)) {}

FindCredentialsWorker::~FindCredentialsWorker() {}

Napi::Promise FindCredentialsWorker::Promise() {
  return deferred.Promise();
}

void FindCredentialsWorker::Execute() {
  std::string error;
  KEYTAR_OP_RESULT result = keytar::FindCredentials(service,
                                                    &credentials,
                                                    &error);
  if (result == keytar::FAIL_ERROR) {
    SetError(error.c_str());
  } else if (result == keytar::FAIL_NONFATAL) {
    success = false;
  } else {
    success = true;
  }
}

void FindCredentialsWorker::OnOK() {
  Napi::HandleScope scope(Env());
  if (!CanCallIntoJS(Env()))
    return;
  Napi::Env env = Env();

  if (success) {
    Napi::Array val = Napi::Array::New(env, credentials.size());
    unsigned int idx = 0;
    std::vector<keytar::Credentials>::iterator it;
    for (it = credentials.begin(); it != credentials.end(); it++) {
      keytar::Credentials cred = *it;
      Napi::Object obj = Napi::Object::New(env);

      Napi::String account = Napi::String::New(env,
        cred.first.data(),
        cred.first.length());

      Napi::String password = Napi::String::New(env,
        cred.second.data(),
        cred.second.length());

#ifndef _WIN32
#pragma GCC diagnostic ignored "-Wunused-result"
#endif
      obj.Set("account", account);
#ifndef _WIN32
#pragma GCC diagnostic ignored "-Wunused-result"
#endif
      obj.Set("password", password);

      (val).Set(idx, obj);
      ++idx;
    }

    deferred.Resolve(val);
  } else {
    deferred.Resolve(Napi::Array::New(env, 0));
  }
}

void FindCredentialsWorker::OnError(Napi::Error const &error) {
  Napi::HandleScope scope(Env());
  if (!CanCallIntoJS(Env()))
    return;
  deferred.Reject(error.Value());
}
