# basalt

`basalt` is a Microsoft BASIC compiler. It parses `.bas` source and emits ANSI C (`-a ansi-c`), Z80 assembly (`-a z80`), or, when built with LLVM, LLVM bitcode (`-a llvm`).

## Build

Dependencies: CMake 3.16 or newer, a C++17 compiler, Flex, and GNU Bison 3 or newer. macOS ships Bison 2.3, which is too old:

```sh
brew install bison
```

CMake looks in the Homebrew keg (`/opt/homebrew/opt/bison` or `/usr/local/opt/bison`) before `/usr/bin/bison`.

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build
```

`ctest` compiles each `verify/test_*.bas` program with the ANSI C backend, runs it, and diffs stdout (after the `--START--` line) against `verify/expected/`.

## Z80 tests

Z80 tests are off unless you have an assembler and a CP/M runner. They compare the same golden files as the C backend, including the cases that used to be skipped (`test_3`, `test_8`, `test_9`, `test_10`).

```sh
cmake -S . -B build -DBASALT_Z80_TESTS=ON \
  -DBASALT_Z80ASM=/path/to/z80asm \
  -DBASALT_CPM="/path/to/cpm-runner"
cmake --build build
ctest --test-dir build
```

`BASALT_CPM` is a command prefix. The generated `.com` path is appended.

## LLVM backend

```sh
cmake -S . -B build -DBASALT_LLVM=ON -DLLVM_DIR=/path/to/lib/cmake/llvm
cmake --build build
ctest --test-dir build
```

The LLVM tests assemble bitcode, link it with `basaltrt.c`, and compare the same goldens.
