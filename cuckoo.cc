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
  this->filter = new cuckoofilter::CuckooFilter<size_t, 12>(1000000);
}

Cuckoo::~Cuckoo() {
  delete this->filter;
}

Napi::Value Cuckoo::Add(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  return Napi::String::New(env, "world");
}

Napi::Value Cuckoo::Contain(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  return Napi::String::New(env, "world");
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

