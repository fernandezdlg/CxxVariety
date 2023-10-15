# CxxVariety

This repository contains the implementation of several methods in modern C++, some implementations using libraries such as MPI, OpenMP, BLAS, Eigen, etc. are also included. Partly, it serves as a reference for myself, but it is also intended to be useful for others.

The objective of this repository is to provide general and simple examples of how C++ can be used to perform computations relevant for scientific simulations.

## Requirements

The following libraries are required to compile some of the examples:

* [Eigen3]
* [OpenMP] _(in the future)_
* [MPI] _(in the future)_

## Compilation

The repository includes relevant Makefiles for compiling the examples. The flags and include paths most probably need to be set according to the system where the examples are compiled.

Most programs are compiled and run in an M1 Pro (ARM64) chip with the LLVM v17 compilers installed via Homebrew. The programs involving CUDA are compiled in a Linux system with Intel x86-64 CPUs and NVIDIA GPUs.

Compiler details:
```
Homebrew clang version 17.0.2
Target: arm64-apple-darwin23.0.0
Thread model: posix
InstalledDir: /opt/homebrew/opt/llvm/bin
```

In the future, a Dockerized compilation environment is planned to be included.
