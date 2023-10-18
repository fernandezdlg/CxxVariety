CXX = clang++-17
CXXFLAGS = -std=c++20 -O2 -Wextra -Wpedantic

# clang already looks at most of the right places within the container
EXTRA =
LIBS =
INCLUDE = -I/usr/include/eigen3
targets = main

all: $(targets)

% : %.cpp
	$(CXX) $< $(CXXFLAGS) $(EXTRA) $(INCLUDE) $(LIBS) -o $@

clean:
	rm -f *.o *~ $(targets) *.txt .tags

.PHONY: all
.PHONY: clean
