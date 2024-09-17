CXX = g++
CXXFLAGS = -std=c++11

main: main.cpp
	$(CXX)	$(CXXFLAGS) main.cpp  -o main

