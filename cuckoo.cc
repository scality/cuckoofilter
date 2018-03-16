#include "./cuckoo.h"

Napi::FunctionReference Cuckoo::constructor;

Napi::String Method(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  return Napi::String::New(env, "world");
}

Napi::Object Cuckoo::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "CuckooFilter", {
    InstanceMethod("add", &Cuckoo::Add),
    InstanceMethod("contain", &Cuckoo::Contain),
    InstanceMethod("delete", &Cuckoo::Delete),
    InstanceMethod("size", &Cuckoo::Size),
    InstanceMethod("sizeInBytes", &Cuckoo::SizeInBytes)
  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("CuckooFilter", func);
  return exports;
}

Cuckoo::Cuckoo(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Cuckoo>(info)  {
  this->filter = new cuckoofilter::CuckooFilter<Napi::String, 12, cuckoofilter::SingleTable, NapiStringHash>(1000000);
}

Cuckoo::~Cuckoo() {
  delete this->filter;
}

Napi::Value Cuckoo::Add(const Napi::CallbackInfo& info) {
  const Napi::Env env = info.Env();

  if (info.Length() != 1) {
    Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "Only strings are supported").ThrowAsJavaScriptException();
    return env.Null();
  }

  Napi::String str = info[0].As<Napi::String>();

  if (this->filter->Add(str) != cuckoofilter::Ok) {
    Napi::Error::New(env, "Only strings are supported").ThrowAsJavaScriptException();
    return env.Null();
  }

  return info.This();
}

Napi::Value Cuckoo::Contain(const Napi::CallbackInfo& info) {
  const Napi::Env env = info.Env();

  if (info.Length() != 1) {
    Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "Only strings are supported").ThrowAsJavaScriptException();
    return env.Null();
  }

  Napi::String str = info[0].As<Napi::String>();

  if (this->filter->Contain(str) == cuckoofilter::Ok) {
    return Napi::Boolean::New(env, true);
  } else {
    return Napi::Boolean::New(env, false);
  }
}

Napi::Value Cuckoo::Delete(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  return Napi::String::New(env, "world");
}

Napi::Value Cuckoo::Size(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  return Napi::String::New(env, "world");
}

Napi::Value Cuckoo::SizeInBytes(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  return Napi::String::New(env, "world");
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  return Cuckoo::Init(env, exports);
}

NODE_API_MODULE(cuckoo, Init)

