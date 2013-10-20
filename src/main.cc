#include <node.h>
using namespace v8;

#include "keytar.h"

namespace {

Handle<Value> AddPassword(const Arguments& args) {
  bool success = keytar::AddPassword(*String::Utf8Value(args[0]),
                                     *String::Utf8Value(args[1]),
                                     *String::Utf8Value(args[2]));
  return Boolean::New(success);
}

Handle<Value> GetPassword(const Arguments& args) {
  std::string password;
  bool success = keytar::GetPassword(*String::Utf8Value(args[0]),
                                     *String::Utf8Value(args[1]),
                                     &password);
  if (success)
    return String::New(password.data(), password.length());
  else
    return Null();
}

Handle<Value> DeletePassword(const Arguments& args) {
  bool success = keytar::DeletePassword(*String::Utf8Value(args[0]),
                                        *String::Utf8Value(args[1]));
  return Boolean::New(success);
}

void Init(Handle<Object> exports) {
  NODE_SET_METHOD(exports, "getPassword", GetPassword);
  NODE_SET_METHOD(exports, "addPassword", AddPassword);
  NODE_SET_METHOD(exports, "deletePassword", DeletePassword);
}

}  // namespace

NODE_MODULE(keytar, Init)
