CXX = g++
CXXFLAGS = -std=c++20 -O3
# -Wall -Wextra -Wpedantic
EXTRA =
LIBS = -L/opt/homebrew/lib
INCLUDE = -I/opt/homebrew/include
targets = sparse_vector sparse_methods

all: $(targets)

% : %.cpp
	$(CXX) $< $(CXXFLAGS) $(EXTRA) $(INCLUDE) $(LIBS) -o $@

clean:
	rm -f *.o *~ $(targets) *.txt .tags

.PHONY: all
.PHONY: clean
