# Makefile standarized flags

GNU Make provides a reference for the implicitly used flags in the Makefile system. I include snippets of some of the relevant ones here.

For reference, the full documentation can be found [here](https://www.gnu.org/software/make/manual/html_node/Implicit-Variables.html). Additionally, for how the implicit rules are utilized, refer to the Makefile documentation [here](https://www.gnu.org/software/make/manual/html_node/Catalogue-of-Rules.html#Catalogue-of-Rules).

- [Makefile standarized flags](#makefile-standarized-flags)
  - [CC](#cc)
  - [CXX](#cxx)
  - [CPP](#cpp)
  - [CFLAGS](#cflags)
  - [CXXFLAGS](#cxxflags)
  - [CPPFLAGS](#cppflags)
  - [LDFLAGS](#ldflags)
  - [LDLIBS](#ldlibs)
- [Remark on these flags](#remark-on-these-flags)

## CC
Program for compiling C programs; default `cc`. I use `clang` instead.

## CXX
Program for compiling C++ programs; default `g++`. I use `clang++` instead.

## CPP
*(normally CC/CXX use it implicitly)*
Program for running the C preprocessor, with results to standard output; default `$(CC) -E`.

## CFLAGS
Extra flags to give to the C compiler. E.g., `-g`, `-O2`, `-Wall`.

## CXXFLAGS
Extra flags to give to the C++ compiler. E.g., `-g`, `-O2`, `-Wall`.

## CPPFLAGS
Extra flags to give to the C preprocessor and programs that use it (the C and Fortran compilers). E.g., `-I.`, `-I$(srcdir)`, `-DHAVE_CONFIG_H`.

## LDFLAGS
Extra flags to give to compilers when they are supposed to invoke the linker, `ld`, such as `-L`. Libraries (`-lfoo`) should be added to the LDLIBS variable instead. E.g., `-L/usr/local/lib`.

## LDLIBS
Library flags or names given to compilers when they are supposed to invoke the linker, ‘ld’. LOADLIBES is a deprecated (but still supported) alternative to LDLIBS. Non-library linker flags, such as -L, should go in the LDFLAGS variable. E.g., `-lmylib`.

# Remark on these flags

Ultimately, the order they are included in the compilation command within the Makefile corresponds to the compiler and linker being used. The reference to the clang compiler can be found [here](https://clang.llvm.org/get_started.html).


