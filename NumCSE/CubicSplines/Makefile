# For this programs, the MathGL library is required. For MacOS, it can be installed with Macports:
# sudo port install mathgl. Note the compiling flags are set accordingly.

CXX = clang++
CXXFLAGS = -std=c++20 -O2 -Wextra -Wpedantic

EXTRA =
LIBS = -L/opt/local/lib -lmgl
INCLUDE = -I.
INCLUDE+= -I/opt/homebrew/include/eigen3 -I/opt/local/include
targets = main

all: $(targets)

% : %.cpp
	$(CXX) $< $(CXXFLAGS) $(EXTRA) $(INCLUDE) $(LIBS) -o $@

clean:
	rm -f *.o *~ $(targets) *.txt .tags

.PHONY: all
.PHONY: clean
