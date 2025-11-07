CXX=g++
CXXFLAGS=-std=c++17 -O2 -Wall -Wextra -pedantic -pthread
all: proc1_sem proc2_sem
proc1_sem: proc1_sem.cpp common.hpp
	$(CXX) $(CXXFLAGS) -o proc1_sem proc1_sem.cpp
proc2_sem: proc2_sem.cpp common.hpp
	$(CXX) $(CXXFLAGS) -o proc2_sem proc2_sem.cpp
clean:
	rm -f proc1_sem proc2_sem
