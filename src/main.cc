#include "nan.h"
using namespace v8;

#include "async.h"
#include "keytar.h"

namespace {

NAN_METHOD(AddPassword) {
  AddPasswordWorker* worker = new AddPasswordWorker(
    *String::Utf8Value(info[0]),
    *String::Utf8Value(info[1]),
    *String::Utf8Value(info[2]),
    new Nan::Callback(info[3].As<Function>()));
  Nan::AsyncQueueWorker(worker);
}

NAN_METHOD(GetPassword) {
  GetPasswordWorker* worker = new GetPasswordWorker(
    *String::Utf8Value(info[0]),
    *String::Utf8Value(info[1]),
    new Nan::Callback(info[2].As<Function>()));
  Nan::AsyncQueueWorker(worker);
}

NAN_METHOD(DeletePassword) {
  DeletePasswordWorker* worker = new DeletePasswordWorker(
    *String::Utf8Value(info[0]),
    *String::Utf8Value(info[1]),
    new Nan::Callback(info[2].As<Function>()));
  Nan::AsyncQueueWorker(worker);
}

NAN_METHOD(FindPassword) {
  FindPasswordWorker* worker = new FindPasswordWorker(
    *String::Utf8Value(info[0]),
    new Nan::Callback(info[1].As<Function>()));
  Nan::AsyncQueueWorker(worker);
}

void Init(Handle<Object> exports) {
  Nan::SetMethod(exports, "getPassword", GetPassword);
  Nan::SetMethod(exports, "addPassword", AddPassword);
  Nan::SetMethod(exports, "deletePassword", DeletePassword);
  Nan::SetMethod(exports, "findPassword", FindPassword);
}

}  // namespace

NODE_MODULE(keytar, Init)
