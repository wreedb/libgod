# libgod
The official reference implementation of the [God](https://wreedb.github.io/god) data serialization format.

# Building
Requires [meson](https://mesonbuild.com) and a C++ compiler capable of  
compiling at least C++ 23, such as [GCC >= 14](https://gcc.gnu.org) or [Clang/LLVM >= 18](https://clang.llvm.org)
```sh
meson setup .build --prefix=/some/where
meson configure .build # to adjust settings if desired

meson compile -C .build

# to run tests
meson test -C .build

```

If you are building with **Clang**, you may find it useful to provide the  
following argument to `meson setup` or `meson configure` in order to use  
[libc++](https://libcxx.llvm.org).
```sh
meson configure .build -D libcxx=true
```

If desired, to install:
```sh
meson install -C .build
```

This will install the library, the API headers and a `pkg-config` file for
convenience, which can be used like so:
```sh
c++ -std=c++23 $(pkg-config --cflags --libs libgod) \
    your-program.cpp -o your-program
```