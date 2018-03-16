{
  "targets": [
    {
      "target_name": "cuckoo",
      "cflags!": [ "--std=c++11 -fno-exceptions -fno-strict-aliasing" ],
      "cflags_cc!": [ "--std=c++11 -fno-exceptions -fno-strict-aliasing" ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        "src/"
      ],
      "dependencies": ["<!(node -p \"require('node-addon-api').gyp\")"],
      'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
      "sources": [
        "./src/hashutil.cc",
        "./cuckoo.cc"
      ],
    }
  ]
}
