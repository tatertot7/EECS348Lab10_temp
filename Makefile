# Compiler
CXX = g++
CXXFLAGS = -std=c++11 -Wall

# Targets and source files
TARGET = calculator
SRCS = main.cpp calculator.cpp
HDRS = calculator.hpp

# Build target
$(TARGET): $(SRCS) $(HDRS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

# Clean rule
clean:
	rm -f $(TARGET)
