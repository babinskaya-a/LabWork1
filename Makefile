CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I.
TARGET = labwork1

SRCS = main.cpp BmpHeader.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


run: $(TARGET) test.bmp
	./$(TARGET)

clean:
	rm -f *.o $(TARGET) test.bmp

.PHONY: all run clean

