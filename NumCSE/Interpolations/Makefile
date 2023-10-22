CXX = clang++
CXXFLAGS = -std=c++20 -stdlib=libc++ -O2 -Wextra -Wpedantic

EXTRA =
LIBS = -L/opt/homebrew/opt/llvm/lib/c++ -Wl,-rpath,/opt/homebrew/opt/llvm/lib/c++
INCLUDE = -I/opt/homebrew/include/eigen3 -I.
targets = interpolations

all: $(targets)

% : %.cpp
	$(CXX) $< $(CXXFLAGS) $(EXTRA) $(INCLUDE) $(LIBS) -o $@

clean:
	rm -f *.o *~ $(targets) *.txt .tags

.PHONY: all
.PHONY: clean
