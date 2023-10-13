# CxxVariety

This repository contains the implementation of several methods in modern C++, some implementations using libraries such as MPI, OpenMP, BLAS, Eigen, etc. are also included.

The objective of this repository is to provide general and simple examples of how C++ can be used to perform computations relevant for scientific simulations.

## Requirements

The following libraries are required to compile the examples:

* [Eigen3] Installed with the Homebrew package manager in macOS.

## Compilation

The repository includes relevant Makefiles for compiling the examples. The flags and include paths most probably need to be set according to the system where the examples are compiled. Most programs are compiled and run in a macOS M1 Pro (ARM64) chip with the clang compilers from LLVM v17 (installed via Homebrew). The programs involving CUDA are compiled in a Linux system with Intel x86-64 CPUs and NVIDIA GPUs.

