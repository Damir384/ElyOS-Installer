# Компилятор и флаги
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -Iinclude
LDFLAGS = -lncurses

# Папки
SRC_DIR = src
BUILD_DIR = build
LIB_DIR = lib

# Файлы
LIB_NAME = libdialogs.a
LIB = $(LIB_DIR)/$(LIB_NAME)
TARGET = $(BUILD_DIR)/app

# Исходники
LIB_SRC = $(SRC_DIR)/dialogs.cpp
APP_SRC = $(SRC_DIR)/main.cpp

# Объектники
LIB_OBJ = $(BUILD_DIR)/dialogs.o
APP_OBJ = $(BUILD_DIR)/main.o

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
