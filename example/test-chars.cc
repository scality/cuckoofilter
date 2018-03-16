#include "cuckoofilter.h"

#include <assert.h>
#include <math.h>

#include <iostream>
#include <vector>

using cuckoofilter::CuckooFilter;

class MyObj {
  public:
    std::string str;

    MyObj(const std::string str) {
      this->str = str;
    }
};

class MyHash {
  public:
    uint64_t operator()(MyObj* o) const {
      return cuckoofilter::HashUtil::BobHash(o->str);
    }
};

int main(int argc, char **argv) {
  size_t total_items = 1000000;

  CuckooFilter<MyObj*, 12, cuckoofilter::SingleTable, MyHash> filter(total_items);

  if (filter.Add(new MyObj("hello")) != cuckoofilter::Ok) {
    std::cout << "unable to add";
    return 0;
  }

  if (filter.Contain(new MyObj("hello")) == cuckoofilter::Ok) {
    std::cout << "ok";
  } else {
    std::cout << "not ok";
  }

  if (filter.Contain(new MyObj("not hello")) == cuckoofilter::Ok) {
    std::cout << "not ok";
  } else {
    std::cout << "ok";
  }

  if (filter.Contain(new MyObj("muaaha")) == cuckoofilter::Ok) {
    std::cout << "not ok";
  } else {
    std::cout << "ok";
  }

  return 0;
}
