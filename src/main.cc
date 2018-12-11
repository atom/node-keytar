#include "nan.h"
#include "async.h"
#include "keytar.h"

using keytar::KEYTAR_OP_RESULT;

namespace {

NAN_METHOD(SetPassword) {
  SetPasswordWorker* worker = new SetPasswordWorker(
    *v8::String::Utf8Value(info[0]),
    *v8::String::Utf8Value(info[1]),
    *v8::String::Utf8Value(info[2]),
    new Nan::Callback(info[3].As<v8::Function>()));
  Nan::AsyncQueueWorker(worker);
}

NAN_METHOD(GetPassword) {
  GetPasswordWorker* worker = new GetPasswordWorker(
    *v8::String::Utf8Value(info[0]),
    *v8::String::Utf8Value(info[1]),
    new Nan::Callback(info[2].As<v8::Function>()));
  Nan::AsyncQueueWorker(worker);
}

NAN_METHOD(DeletePassword) {
  DeletePasswordWorker* worker = new DeletePasswordWorker(
    *v8::String::Utf8Value(info[0]),
    *v8::String::Utf8Value(info[1]),
    new Nan::Callback(info[2].As<v8::Function>()));
  Nan::AsyncQueueWorker(worker);
}

NAN_METHOD(FindPassword) {
  FindPasswordWorker* worker = new FindPasswordWorker(
    *v8::String::Utf8Value(info[0]),
    new Nan::Callback(info[1].As<v8::Function>()));
  Nan::AsyncQueueWorker(worker);
}

NAN_METHOD(FindCredentials) {
  FindCredentialsWorker* worker = new FindCredentialsWorker(
    *v8::String::Utf8Value(info[0]),
    new Nan::Callback(info[1].As<v8::Function>()));
  Nan::AsyncQueueWorker(worker);
}

NAN_METHOD(GetPasswordSync){
  std::string password,error;
  KEYTAR_OP_RESULT result =keytar::GetPassword(*v8::String::Utf8Value(info[0]),
                      *v8::String::Utf8Value(info[1]),&password,&error);
  // if (result == keytar::FAIL_ERROR) {
  //   SetErrorMessage(error.c_str());
  // } else 
  if (result == keytar::FAIL_NONFATAL) {
    info.GetReturnValue().Set(Nan::Null());
  } else {
    info.GetReturnValue().Set(Nan::New(password).ToLocalChecked());
  }
}

void Init(v8::Handle<v8::Object> exports) {
  Nan::SetMethod(exports, "getPassword", GetPassword);
  Nan::SetMethod(exports, "setPassword", SetPassword);
  Nan::SetMethod(exports, "deletePassword", DeletePassword);
  Nan::SetMethod(exports, "findPassword", FindPassword);
  Nan::SetMethod(exports, "findCredentials", FindCredentials);
  Nan::SetMethod(exports,"getPasswordSync",GetPasswordSync);
}

}  // namespace

NODE_MODULE(keytar, Init)
