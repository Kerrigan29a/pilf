# PILF = (Process Instrumentation with Lua) Framework
by Javier Escalada Gómez

[![Build Status](https://travis-ci.org/Kerrigan29a/pilf.svg)](https://travis-ci.org/Kerrigan29a/pilf)
[![Coverage Status](https://img.shields.io/coveralls/Kerrigan29a/pilf.svg)](https://coveralls.io/r/Kerrigan29a/pilf)

# Build

Download the code with submodules (from branch develop)

```bash
git clone -b develop https://github.com/kerrigan29a/pilf.git
cd pilf
```

To test the code, build it with Debug mode

```bash

mkdir build && cd build && cmake .. -DCMAKE_BUILD_TYPE=Debug && make && make install && ctest
```

To build the code, you can use the Release mode

```bash

mkdir build && cd build && cmake .. -DCMAKE_BUILD_TYPE=Release && make && make install
```


# Todo
- [ ] Windows support
- [x] Linux support (Travis is failing due to the installed version of Cmake)
- [x] Darwin support

# [License](LICENSE.md)
