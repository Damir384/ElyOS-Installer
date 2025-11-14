CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -Iinclude
LDFLAGS = -lncurses

SRC_DIR = src
BUILD_DIR = build
LIB_DIR = lib

LIB_NAME = libdialogs.a
LIB = $(LIB_DIR)/$(LIB_NAME)
TARGET = $(BUILD_DIR)/app

# Автоматически найдём все .cpp
ALL_SRC = $(wildcard $(SRC_DIR)/*.cpp)

# dialogs.cpp → в библиотеку
LIB_SRC = $(SRC_DIR)/dialogs.cpp

# Остальные .cpp → приложение
APP_SRC = $(filter-out $(LIB_SRC), $(ALL_SRC))

# Соответствующие .o
LIB_OBJ = $(LIB_SRC:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
APP_OBJ = $(APP_SRC:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

all: $(TARGET)

$(LIB): $(LIB_OBJ)
	mkdir -p $(LIB_DIR)
	ar rcs $(LIB) $(LIB_OBJ)

$(TARGET): $(LIB) $(APP_OBJ)
	$(CXX) $(APP_OBJ) -L$(LIB_DIR) -ldialogs $(LDFLAGS) -o $(TARGET)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(LIB_DIR)

run: all
	./$(TARGET)

debug: all
	./$(TARGET) --debug
