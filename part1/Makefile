CXX=g++
CXXFLAGS=-std=c++17 -O2 -Wall -Wextra -pedantic -pthread

all: two_procs
two_procs: two_procs.cpp
	$(CXX) $(CXXFLAGS) -o two_procs two_procs.cpp

clean:
	rm -f two_procs
