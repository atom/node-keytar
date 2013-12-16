#include "nan.h"
using namespace v8;

#include "keytar.h"

namespace {

NAN_METHOD(AddPassword) {
  NanScope();
  bool success = keytar::AddPassword(*String::Utf8Value(args[0]),
                                     *String::Utf8Value(args[1]),
                                     *String::Utf8Value(args[2]));
  NanReturnValue(Boolean::New(success));
}

NAN_METHOD(GetPassword) {
  NanScope();
  std::string password;
  bool success = keytar::GetPassword(*String::Utf8Value(args[0]),
                                     *String::Utf8Value(args[1]),
                                     &password);
  if (success)
    NanReturnValue(String::New(password.data(), password.length()));
  else
    NanReturnNull();
}

NAN_METHOD(DeletePassword) {
  NanScope();
  bool success = keytar::DeletePassword(*String::Utf8Value(args[0]),
                                        *String::Utf8Value(args[1]));
  NanReturnValue(Boolean::New(success));
}

NAN_METHOD(FindPassword) {
  NanScope();
  std::string password;
  bool success = keytar::FindPassword(*String::Utf8Value(args[0]), &password);
  if (success)
    NanReturnValue(String::New(password.data(), password.length()));
  else
    NanReturnNull();
}

void Init(Handle<Object> exports) {
  NODE_SET_METHOD(exports, "getPassword", GetPassword);
  NODE_SET_METHOD(exports, "addPassword", AddPassword);
  NODE_SET_METHOD(exports, "deletePassword", DeletePassword);
  NODE_SET_METHOD(exports, "findPassword", FindPassword);
}

}  // namespace

NODE_MODULE(keytar, Init)
