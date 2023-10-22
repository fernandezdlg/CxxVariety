CXX = clang++
CXXFLAGS = -std=c++20 -O2 -Wextra -Wpedantic

EXTRA =
LIBS = -L/opt/local/lib -lmgl
INCLUDE = -I.
INCLUDE+= -I/opt/local/include
targets = main

all: $(targets)

% : %.cpp
	$(CXX) $< $(CXXFLAGS) $(EXTRA) $(INCLUDE) $(LIBS) -o $@

clean:
	rm -f *.o *~ $(targets) *.txt .tags

.PHONY: all
.PHONY: clean
