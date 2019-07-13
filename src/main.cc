#include "nan.h"
#include "async.h"
#include "keytar.h"

namespace {

NAN_METHOD(SetPassword) {
  if (!info[0]->IsString()) {
    Nan::ThrowTypeError("Parameter 'service' must be a string");
    return;
  }

  Nan::Utf8String serviceNan(info[0]);
  std::string service(*serviceNan, serviceNan.length());

  if (!info[1]->IsString()) {
    Nan::ThrowTypeError("Parameter 'username' must be a string");
    return;
  }

  Nan::Utf8String usernameNan(info[1]);
  std::string username(*usernameNan, usernameNan.length());

  if (!info[2]->IsString()) {
    Nan::ThrowTypeError("Parameter 'password' must be a string");
    return;
  }

  Nan::Utf8String passwordNan(info[2]);
  std::string password(*passwordNan, passwordNan.length());

  SetPasswordWorker* worker = new SetPasswordWorker(
    service,
    username,
    password,
    new Nan::Callback(info[3].As<v8::Function>()));
  Nan::AsyncQueueWorker(worker);
}

NAN_METHOD(SetPasswordSync) {
  if (!info[0]->IsString()) {
    Nan::ThrowTypeError("Parameter 'service' must be a string");
    return;
  }

  Nan::Utf8String serviceNan(info[0]);
  std::string service(*serviceNan, serviceNan.length());

  if (!info[1]->IsString()) {
    Nan::ThrowTypeError("Parameter 'username' must be a string");
    return;
  }

  Nan::Utf8String usernameNan(info[1]);
  std::string username(*usernameNan, usernameNan.length());

  if (!info[2]->IsString()) {
    Nan::ThrowTypeError("Parameter 'password' must be a string");
    return;
  }

  Nan::Utf8String passwordNan(info[2]);
  std::string password(*passwordNan, passwordNan.length());

  std::string error;
  keytar::KEYTAR_OP_RESULT result = keytar::SetPassword(service,
                                                        username,
                                                        password,
                                                        &error);
  if (result == keytar::FAIL_ERROR) {
    Nan::ThrowError(Nan::New(error.c_str()).ToLocalChecked());
  }
}

NAN_METHOD(GetPassword) {
  if (!info[0]->IsString()) {
    Nan::ThrowTypeError("Parameter 'service' must be a string");
    return;
  }

  Nan::Utf8String serviceNan(info[0]);
  std::string service(*serviceNan, serviceNan.length());

  if (!info[1]->IsString()) {
    Nan::ThrowTypeError("Parameter 'username' must be a string");
    return;
  }

  Nan::Utf8String usernameNan(info[1]);
  std::string username(*usernameNan, usernameNan.length());

  GetPasswordWorker* worker = new GetPasswordWorker(
    service,
    username,
    new Nan::Callback(info[2].As<v8::Function>()));
  Nan::AsyncQueueWorker(worker);
}

NAN_METHOD(GetPasswordSync) {
  if (!info[0]->IsString()) {
    Nan::ThrowTypeError("Parameter 'service' must be a string");
    return;
  }

  Nan::Utf8String serviceNan(info[0]);
  std::string service(*serviceNan, serviceNan.length());

  if (!info[1]->IsString()) {
    Nan::ThrowTypeError("Parameter 'username' must be a string");
    return;
  }

  Nan::Utf8String usernameNan(info[1]);
  std::string username(*usernameNan, usernameNan.length());

  std::string password;
  std::string error;
  keytar::KEYTAR_OP_RESULT result = keytar::GetPassword(service,
                                                username,
                                                &password,
                                                &error);

  if (result == keytar::FAIL_ERROR) {
    Nan::ThrowError(Nan::New(error.c_str()).ToLocalChecked());
  } else if (result == keytar::FAIL_NONFATAL) {
    info.GetReturnValue().Set(Nan::Null());
  } else {
    info.GetReturnValue().Set(
      Nan::New<v8::String>(
        password.data(),
        password.length()).ToLocalChecked());
  }
}

NAN_METHOD(DeletePassword) {
  if (!info[0]->IsString()) {
    Nan::ThrowTypeError("Parameter 'service' must be a string");
    return;
  }

  Nan::Utf8String serviceNan(info[0]);
  std::string service(*serviceNan, serviceNan.length());

  if (!info[1]->IsString()) {
    Nan::ThrowTypeError("Parameter 'username' must be a string");
    return;
  }

  Nan::Utf8String usernameNan(info[1]);
  std::string username(*usernameNan, usernameNan.length());

  DeletePasswordWorker* worker = new DeletePasswordWorker(
    service,
    username,
    new Nan::Callback(info[2].As<v8::Function>()));
  Nan::AsyncQueueWorker(worker);
}

NAN_METHOD(DeletePasswordSync) {
  if (!info[0]->IsString()) {
    Nan::ThrowTypeError("Parameter 'service' must be a string");
    return;
  }

  Nan::Utf8String serviceNan(info[0]);
  std::string service(*serviceNan, serviceNan.length());

  if (!info[1]->IsString()) {
    Nan::ThrowTypeError("Parameter 'username' must be a string");
    return;
  }

  Nan::Utf8String usernameNan(info[1]);
  std::string username(*usernameNan, usernameNan.length());

  std::string error;
  keytar::KEYTAR_OP_RESULT result = keytar::DeletePassword(service,
    username, &error);
  if (result == keytar::FAIL_ERROR) {
    Nan::ThrowError(Nan::New(error.c_str()).ToLocalChecked());
  } else if (result == keytar::FAIL_NONFATAL) {
    info.GetReturnValue().Set(Nan::New<v8::Boolean>(false));
  } else {
    info.GetReturnValue().Set(Nan::New<v8::Boolean>(true));
  }
}

NAN_METHOD(FindPassword) {
  if (!info[0]->IsString()) {
    Nan::ThrowTypeError("Parameter 'service' must be a string");
    return;
  }

  Nan::Utf8String serviceNan(info[0]);
  std::string service(*serviceNan, serviceNan.length());

  FindPasswordWorker* worker = new FindPasswordWorker(
    service,
    new Nan::Callback(info[1].As<v8::Function>()));
  Nan::AsyncQueueWorker(worker);
}

NAN_METHOD(FindPasswordSync) {
  if (!info[0]->IsString()) {
    Nan::ThrowTypeError("Parameter 'service' must be a string");
    return;
  }

  Nan::Utf8String serviceNan(info[0]);
  std::string service(*serviceNan, serviceNan.length());

  std::string password;
  std::string error;
  keytar::KEYTAR_OP_RESULT result = keytar::FindPassword(service,
                                                 &password,
                                                 &error);
  if (result == keytar::FAIL_ERROR) {
    Nan::ThrowError(Nan::New(error.c_str()).ToLocalChecked());
  } else if (result == keytar::FAIL_NONFATAL) {
    info.GetReturnValue().Set(Nan::Null());
  } else {
    info.GetReturnValue().Set(
      Nan::New<v8::String>(
        password.data(),
        password.length()).ToLocalChecked());
  }
}

NAN_METHOD(FindCredentials) {
  if (!info[0]->IsString()) {
    Nan::ThrowTypeError("Parameter 'service' must be a string");
    return;
  }

  Nan::Utf8String serviceNan(info[0]);
  std::string service(*serviceNan, serviceNan.length());

  FindCredentialsWorker* worker = new FindCredentialsWorker(
    service,
    new Nan::Callback(info[1].As<v8::Function>()));
  Nan::AsyncQueueWorker(worker);
}

NAN_METHOD(FindCredentialsSync) {
  if (!info[0]->IsString()) {
    Nan::ThrowTypeError("Parameter 'service' must be a string");
    return;
  }

  Nan::Utf8String serviceNan(info[0]);
  std::string service(*serviceNan, serviceNan.length());

  std::string error;
  std::vector<keytar::Credentials> credentials;
  keytar::KEYTAR_OP_RESULT result = keytar::FindCredentials(service,
                                                    &credentials,
                                                    &error);
  if (result == keytar::FAIL_ERROR) {
    Nan::ThrowError(Nan::New(error.c_str()).ToLocalChecked());
  } else if (result == keytar::FAIL_NONFATAL) {
    info.GetReturnValue().Set(Nan::New<v8::Array>(0));
  } else {
    v8::Local<v8::Array> val = Nan::New<v8::Array>(credentials.size());
    unsigned int idx = 0;
    std::vector<keytar::Credentials>::iterator it;
    for (it = credentials.begin(); it != credentials.end(); it++) {
      keytar::Credentials cred = *it;
      v8::Local<v8::Object> obj = Nan::New<v8::Object>();

      v8::Local<v8::String> account = Nan::New<v8::String>(
        cred.first.data(),
        cred.first.length()).ToLocalChecked();

      v8::Local<v8::String> password = Nan::New<v8::String>(
        cred.second.data(),
        cred.second.length()).ToLocalChecked();

#ifndef _WIN32
#pragma GCC diagnostic ignored "-Wunused-result"
#endif
      obj->Set(
        Nan::GetCurrentContext(),
        Nan::New("account").ToLocalChecked(),
        account);
#ifndef _WIN32
#pragma GCC diagnostic ignored "-Wunused-result"
#endif
      obj->Set(
        Nan::GetCurrentContext(),
        Nan::New("password").ToLocalChecked(),
        password);

      Nan::Set(val, idx, obj);
      ++idx;
    }
    info.GetReturnValue().Set(val);
  }
}

NAN_MODULE_INIT(Init) {
  Nan::SetMethod(target, "getPassword", GetPassword);
  Nan::SetMethod(target, "getPasswordSync", GetPasswordSync);
  Nan::SetMethod(target, "setPassword", SetPassword);
  Nan::SetMethod(target, "setPasswordSync", SetPasswordSync);
  Nan::SetMethod(target, "deletePassword", DeletePassword);
  Nan::SetMethod(target, "deletePasswordSync", DeletePasswordSync);
  Nan::SetMethod(target, "findPassword", FindPassword);
  Nan::SetMethod(target, "findPasswordSync", FindPasswordSync);
  Nan::SetMethod(target, "findCredentials", FindCredentials);
  Nan::SetMethod(target, "findCredentialsSync", FindCredentialsSync);
}

}  // namespace

#if NODE_MAJOR_VERSION >= 10
NAN_MODULE_WORKER_ENABLED(keytar, Init)
#else
NODE_MODULE(keytar, Init)
#endif
