CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic
TARGET   := inventory
SRCS     := main.cpp Item.cpp InventoryManager.cpp
OBJS     := $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: run clean
