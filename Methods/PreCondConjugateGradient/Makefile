CXX = g++
CXXFLAGS = -std=c++20 -O2 
# -Wall -Wextra -Wpedantic
EXTRA =
LIBS = -L/opt/homebrew/lib
INCLUDE = -I/opt/homebrew/include/eigen3/
targets = main

all: $(targets)

% : %.cpp
	$(CXX) $< $(CXXFLAGS) $(EXTRA) $(INCLUDE) $(LIBS) -o $@

clean:
	rm -f *.o *~ $(targets) *.txt .tags

.PHONY: all
.PHONY: clean
