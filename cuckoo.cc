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
    InstanceAccessor("size", &Cuckoo::Size, NULL),
    InstanceAccessor("bytes", &Cuckoo::SizeInBytes, NULL)
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

  // do not add twice, we cannot support unlimited adding
  // it will fail after ~10 add with the same parameter
  // otherwise
  if (this->filter->Contain(str) == cuckoofilter::Ok) {
    return info.This();
  }

  const int res = this->filter->Add(str);

  if (res == cuckoofilter::Ok) {
    return info.This();
  } else if (res == cuckoofilter::NotEnoughSpace) {
    Napi::Error::New(env, "Not enough space to add this key").ThrowAsJavaScriptException();
    return env.Null();
  } else {
    Napi::Error::New(env, "something went wrong during add").ThrowAsJavaScriptException();
    return env.Null();
  }
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

  const int res = this->filter->Delete(str);

  if (res == cuckoofilter::Ok) {
    return info.This();
  } else if (res == cuckoofilter::NotFound) {
    // not found is ok too
    return info.This();
  } else {
    Napi::Error::New(env, "something went wrong during delete").ThrowAsJavaScriptException();
    return env.Null();
  }
}

Napi::Value Cuckoo::Size(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  return Napi::Number::New(env, this->filter->Size());
}

Napi::Value Cuckoo::SizeInBytes(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  return Napi::Number::New(env, this->filter->SizeInBytes());
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  return Cuckoo::Init(env, exports);
}

NODE_API_MODULE(cuckoo, Init)

