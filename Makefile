CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -Iinclude -Isrc
LDFLAGS = -lncurses

SRC_DIR = src
BUILD_DIR = build

# Все cpp рекурсивно
SRCS = $(shell find $(SRC_DIR) -name "*.cpp")
OBJS = $(SRCS:%.cpp=$(BUILD_DIR)/%.o)

TARGET = $(BUILD_DIR)/app

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $(TARGET)

$(BUILD_DIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

run: all
	./$(TARGET)

debug: all
	./$(TARGET) --debug
