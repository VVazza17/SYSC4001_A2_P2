CXX=g++
CXXFLAGS=-std=c++17 -O2 -Wall -Wextra -pedantic -pthread
all: proc1 proc2
proc1: proc1.cpp
	$(CXX) $(CXXFLAGS) -o proc1 proc1.cpp
proc2: proc2.cpp
	$(CXX) $(CXXFLAGS) -o proc2 proc2.cpp
clean:
	rm -f proc1 proc2
