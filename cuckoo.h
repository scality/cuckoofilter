#ifndef CUCKOO_H
#define CUCKOO_H

#include <napi.h>
#include <openssl/md5.h>
#include "cuckoofilter.h"

class NapiStringHash {
  public:
  uint64_t operator()(Napi::String n_str) const {
    const std::string str(n_str);
    union {
      unsigned char md5buf[16];
      uint64_t hash;
    } u;
    MD5((const unsigned char *)str.data(), str.length(), u.md5buf);
    return u.hash;
  }
};

class Cuckoo : public Napi::ObjectWrap<Cuckoo> {
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  Cuckoo(const Napi::CallbackInfo& info);
  ~Cuckoo();

 private:
  Napi::Value Add(const Napi::CallbackInfo& info);
  Napi::Value Contain(const Napi::CallbackInfo& info);
  Napi::Value Delete(const Napi::CallbackInfo& info);
  Napi::Value Size(const Napi::CallbackInfo& info);
  Napi::Value SizeInBytes(const Napi::CallbackInfo& info);

  static Napi::FunctionReference constructor;
  cuckoofilter::CuckooFilter<Napi::String, 12, cuckoofilter::SingleTable, NapiStringHash> *filter;
};

#endif
