CXX = clang++
CXXFLAGS = -std=c++20 -O2 
# -Wall -Wextra -Wpedantic
EXTRA =
LIBS = -L/opt/homebrew/lib
INCLUDE = -I/opt/homebrew/include/eigen3/
targets = simple matMult cg least_squares choleskyQR

all: $(targets)

% : %.cpp
	$(CXX) $< $(CXXFLAGS) $(EXTRA) $(INCLUDE) $(LIBS) -o $@

clean:
	rm -f *.o *~ $(targets) *.txt .tags

.PHONY: all
.PHONY: clean
