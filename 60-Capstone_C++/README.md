# Capstone C++

## 2025-12-14 Motivation

The pure C version is still a bit wordy at nearly 300 lines. The C++ APIs should reduce that.

## Status

* 2025-12-14 Chip info
* 2025-12-15 Working input
* 2025-12-15 Working output

## Build

```text
g++ -Wall -o follow_input follow_input.cpp -lgpiodcxx
```

## Errata

* <https://github.com/brgl/libgpiod/tree/master/bindings/cxx/examples> C++ examples.
* (It has been a long time since I wrote any C++. And C++ takes a long time to build on a Pi Zero.)
* C++ version is presently 111 LOC and the C version is 286 LOC. The C++ variant has almost no error handling and will exit on C++ exceptions.
