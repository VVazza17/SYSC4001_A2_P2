CXX=g++
CXXFLAGS=-std=c++17 -O2 -Wall -Wextra -pedantic -pthread
all: proc1_shm proc2_shm
proc1_shm: proc1_shm.cpp common.hpp
	$(CXX) $(CXXFLAGS) -o proc1_shm proc1_shm.cpp
proc2_shm: proc2_shm.cpp common.hpp
	$(CXX) $(CXXFLAGS) -o proc2_shm proc2_shm.cpp
clean:
	rm -f proc1_shm proc2_shm
