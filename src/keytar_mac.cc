#include <node.h>
#include <v8.h>
#include <string>
#include <Security/Security.h>

using namespace std;
using namespace v8;

extern Handle<Value> GetPassword(const Arguments& args);
extern Handle<Value> AddPassword(const Arguments& args);
extern Handle<Value> DeletePassword(const Arguments& args);

void init(Handle<Object> exports) {
  exports->Set(String::NewSymbol("getPassword"),
      FunctionTemplate::New(GetPassword)->GetFunction());
  exports->Set(String::NewSymbol("addPassword"),
      FunctionTemplate::New(AddPassword)->GetFunction());
  exports->Set(String::NewSymbol("deletePassword"),
      FunctionTemplate::New(DeletePassword)->GetFunction());
}

NODE_MODULE(keytar, init)

Handle<Value> AddPassword(const Arguments& args) {
  String::Utf8Value utf8Service(Local<String>::Cast(args[0]));
  string service(*utf8Service);

  String::Utf8Value utf8Account(Local<String>::Cast(args[1]));
  string account(*utf8Account);

  String::Utf8Value utf8Password(Local<String>::Cast(args[2]));
  string password(*utf8Password);

  OSStatus status = SecKeychainAddGenericPassword(NULL,
                                                  service.length(),
                                                  service.data(),
                                                  account.length(),
                                                  account.data(),
                                                  password.length(),
                                                  password.data(),
                                                  NULL);
  HandleScope scope;
  return scope.Close(v8::Boolean::New(status == errSecSuccess));
}

Handle<Value> GetPassword(const Arguments& args) {
  String::Utf8Value utf8Service(Local<String>::Cast(args[0]));
  string service(*utf8Service);

  String::Utf8Value utf8Account(Local<String>::Cast(args[1]));
  string account(*utf8Account);

  UInt32 passwordLength;
  void *password;
  OSStatus status = SecKeychainFindGenericPassword(NULL,
                                                  service.length(),
                                                  service.data(),
                                                  account.length(),
                                                  account.data(),
                                                  &passwordLength,
                                                  &password,
                                                  NULL);
  HandleScope scope;
  if (status != errSecSuccess)
    return scope.Close(Null());

  return scope.Close(String::NewSymbol((const char*)password, passwordLength));
}

Handle<Value> DeletePassword(const Arguments& args) {
  String::Utf8Value utf8Service(Local<String>::Cast(args[0]));
  string service(*utf8Service);

  String::Utf8Value utf8Account(Local<String>::Cast(args[1]));
  string account(*utf8Account);

  String::Utf8Value utf8Password(Local<String>::Cast(args[2]));
  string password(*utf8Password);

  SecKeychainItemRef item;
  OSStatus status = SecKeychainFindGenericPassword(NULL,
                                                  service.length(),
                                                  service.data(),
                                                  account.length(),
                                                  account.data(),
                                                  NULL,
                                                  NULL,
                                                  &item);
  if (status == errSecSuccess) {
    status = SecKeychainItemDelete(item);
    CFRelease(item);
  }

  HandleScope scope;
  return scope.Close(v8::Boolean::New(status == errSecSuccess));
}
