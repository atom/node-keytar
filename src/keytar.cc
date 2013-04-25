#include <node.h>
#include <v8.h>

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
