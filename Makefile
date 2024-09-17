# Define the compiler and flags
CXX = g++
CXXFLAGS = -std=c++11

# Define the target executable
TARGET = main

# Define the source files and object files
SRCS = main.cpp \
       files/TaskManager.cpp \
       files/RoundRobin.cpp \
       files/ShortestJobFirst.cpp \
       files/PriorityScheduling.cpp \
       files/MultiLevelQueue.cpp

# Define the header files
HEADERS = Headers/TaskManager.hpp \
          Headers/RoundRobin.hpp \
          Headers/ShortestJobFirst.hpp \
          Headers/PriorityScheduling.hpp \
          Headers/MultiLevelQueue.hpp

# Derive object files from source files
OBJS = $(SRCS:.cpp=.o)

# Default target
all: $(TARGET)

# Rule to build the target executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

# Rule to build object files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean target to remove build artifacts
clean:
	rm -f $(TARGET) $(OBJS)

# Phony targets
.PHONY: all clean
