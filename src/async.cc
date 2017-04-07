#include <string>

#include "nan.h"
#include "keytar.h"
#include "async.h"

using keytar::KEYTAR_OP_RESULT;

SetPasswordWorker::SetPasswordWorker(
  const std::string& service,
  const std::string& account,
  const std::string& password,
  Nan::Callback* callback
) : AsyncWorker(callback),
    service(service),
    account(account),
    password(password) {}

SetPasswordWorker::~SetPasswordWorker() {}

void SetPasswordWorker::Execute() {
  std::string error;
  KEYTAR_OP_RESULT result = keytar::SetPassword(service,
                                                account,
                                                password,
                                                &error);
  if (result == keytar::FAIL_ERROR) {
    SetErrorMessage(error.c_str());
  }
}



GetPasswordWorker::GetPasswordWorker(
  const std::string& service,
  const std::string& account,
  Nan::Callback* callback
) : AsyncWorker(callback),
    service(service),
    account(account) {}

GetPasswordWorker::~GetPasswordWorker() {}

void GetPasswordWorker::Execute() {
  std::string error;
  KEYTAR_OP_RESULT result = keytar::GetPassword(service,
                                                account,
                                                &password,
                                                &error);
  if (result == keytar::FAIL_ERROR) {
    SetErrorMessage(error.c_str());
  } else if (result == keytar::FAIL_NONFATAL) {
    success = false;
  } else {
    success = true;
  }
}

void GetPasswordWorker::HandleOKCallback() {
  Nan::HandleScope scope;
  v8::Local<v8::Value> val = Nan::Null();
  if (success) {
    val = Nan::New<v8::String>(password.data(),
                               password.length()).ToLocalChecked();
  }
  v8::Local<v8::Value> argv[] = {
    Nan::Null(),
    val
  };

  callback->Call(2, argv);
}



DeletePasswordWorker::DeletePasswordWorker(
  const std::string& service,
  const std::string& account,
  Nan::Callback* callback
) : AsyncWorker(callback),
    service(service),
    account(account) {}

DeletePasswordWorker::~DeletePasswordWorker() {}

void DeletePasswordWorker::Execute() {
  std::string error;
  KEYTAR_OP_RESULT result = keytar::DeletePassword(service, account, &error);
  if (result == keytar::FAIL_ERROR) {
    SetErrorMessage(error.c_str());
  } else if (result == keytar::FAIL_NONFATAL) {
    success = false;
  } else {
    success = true;
  }
}

void DeletePasswordWorker::HandleOKCallback() {
  Nan::HandleScope scope;
  v8::Local<v8::Boolean> val =
    Nan::New<v8::Boolean>(success);
  v8::Local<v8::Value> argv[] = {
    Nan::Null(),
    val
  };

  callback->Call(2, argv);
}



FindPasswordWorker::FindPasswordWorker(
  const std::string& service,
  Nan::Callback* callback
) : AsyncWorker(callback),
    service(service) {}

FindPasswordWorker::~FindPasswordWorker() {}

void FindPasswordWorker::Execute() {
  std::string error;
  KEYTAR_OP_RESULT result = keytar::FindPassword(service,
                                                 &password,
                                                 &error);
  if (result == keytar::FAIL_ERROR) {
    SetErrorMessage(error.c_str());
  } else if (result == keytar::FAIL_NONFATAL) {
    success = false;
  } else {
    success = true;
  }
}

void FindPasswordWorker::HandleOKCallback() {
  Nan::HandleScope scope;
  v8::Local<v8::Value> val = Nan::Null();
  if (success) {
    val = Nan::New<v8::String>(password.data(),
                               password.length()).ToLocalChecked();
  }
  v8::Local<v8::Value> argv[] = {
    Nan::Null(),
    val
  };

  callback->Call(2, argv);
}
