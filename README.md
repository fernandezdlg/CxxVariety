# CxxVariety

This repository contains the implementation of several methods in modern C++, some implementations using libraries such as `MPI`, `OpenMP`, `BLAS`, `Eigen`, etc. are also included. Partly, it serves as a reference for myself, but it is also intended to be useful for others.

The objective of this repository is to provide general and simple examples of how C++ can be used to perform computations relevant for scientific simulations.

## Requirements

The following libraries are required to compile some of the examples:

* `Eigen3`
* `OpenMP` _(in the future)_
* `MPI` _(in the future)_

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

# Dockerized compilation

You will find a `Dockerfile` in this repository which can be used to build an image and run a container designed for compiling the examples in this repository. The image is based on the **Ubuntu 22.04 LTS** distribution and includes the following packages:

* `build-essential`
* `cmake`
* `git`
* `libeigen3-dev`
* LLVM v17 compilers
<!-- * libopenmpi-dev
* openmpi-bin
* libblas-dev
* liblapack-dev
* libopenblas-dev
* libopenblas-base -->

This container is run with the following commands, it will mount an interactive container with bash, where the current directory is binded to /src to not copy the src files repeatedly.

## To create image

```
docker build -t devel/devel:latest -f Dockerfile .
```

## To run image

```
docker run -it --rm --name=devel \
    --mount type=bind,source=${PWD},target=/src \
    devel/devel:latest \
    bash
```

Within this interactive terminal, you can run `clang-17 --version` to corroborate the LLVM-17 installation was successful. E.g., I get the output

```
Ubuntu clang version 17.0.3 (++20231017063209+2e00f4ca4e91-1~exp1~20231017063224.54)
Target: aarch64-unknown-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
```
