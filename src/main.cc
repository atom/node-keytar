#include "nan.h"
#include "keytar.h"

void AddPassword(const Nan::FunctionCallbackInfo<v8::Value>& info) {
    bool success = keytar::AddPassword(
                       *v8::String::Utf8Value(info[0]),
                       *v8::String::Utf8Value(info[1]),
                       *v8::String::Utf8Value(info[2]));
    info.GetReturnValue().Set(Nan::New<v8::Boolean>(success));
}

void GetPassword(const Nan::FunctionCallbackInfo<v8::Value>& info) {
    std::string password;

    bool success = keytar::GetPassword(
                       *v8::String::Utf8Value(info[0]),
                       *v8::String::Utf8Value(info[1]),
                       &password);

    if (success) {
        v8::Local<v8::String> val = Nan::New<v8::String>(password.data(),
                                    password.length()).ToLocalChecked();
        info.GetReturnValue().Set(val);
    } else {
        info.GetReturnValue().Set(Nan::Null());
    }
}

void DeletePassword(const Nan::FunctionCallbackInfo<v8::Value>& info) {
    bool success = keytar::DeletePassword(*v8::String::Utf8Value(info[0]),
                                          *v8::String::Utf8Value(info[1]));

    info.GetReturnValue().Set(Nan::New<v8::Boolean>(success));
}

void FindPassword(const Nan::FunctionCallbackInfo<v8::Value>& info) {
    std::string password;
    bool success = keytar::FindPassword(*v8::String::Utf8Value(info[0]),
                                        &password);

    if (success) {
        v8::Local<v8::String> val = Nan::New<v8::String>(password.data(),
                                    password.length()).ToLocalChecked();
        info.GetReturnValue().Set(val);
    } else {
        info.GetReturnValue().Set(Nan::Null());
    }
}

void Init(v8::Local<v8::Object> exports) {
    exports->Set(Nan::New("addPassword").ToLocalChecked(),
                 Nan::New<v8::FunctionTemplate>(AddPassword)->GetFunction());
    exports->Set(Nan::New("getPassword").ToLocalChecked(),
                 Nan::New<v8::FunctionTemplate>(GetPassword)->GetFunction());
    exports->Set(Nan::New("deletePassword").ToLocalChecked(),
                 Nan::New<v8::FunctionTemplate>(DeletePassword)->GetFunction());
    exports->Set(Nan::New("findPassword").ToLocalChecked(),
                 Nan::New<v8::FunctionTemplate>(FindPassword)->GetFunction());
}

NODE_MODULE(keytar, Init)
