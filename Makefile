# Compiler settings.
CC = g++
CFLAGS = -Wall -Werror -std=c++11 -g

# Includes.
INCLUDE = -Ilib/include -Isrc -Isrc/base -Isrc/interfaces -Isrc/shaders

# Libraries.
LIB_DIR = -Llib/lib
LIB_FILES = -lm -lpthread -lGL -lGLEW -lSDL2 -lSDL2_image -ljpeg -lpng -ltiff \
-lwebp -lSDL2main -ltiny_obj_loader -ltinyxml2

# Directories.
SRC_DIR = src
RELEASE_DIR = release
DEBUG_DIR = debug
BASE_DIR := $(SRC_DIR)/base
INTERFACE_DIR := $(SRC_DIR)/interfaces

# Output file.
OUTPUT_FILE := $(RELEASE_DIR)/orbits.exe

# List of targets.
TARGETS := camera window eventmanager mesh orbitalbody orbitalsystem shader \
shadersingleton

# Target files.
TARGET_CPP_FILES := $(addprefix $(SRC_DIR)/,$(addsuffix .cpp,$(TARGETS)))
TARGET_HDR_FILES := $(addprefix $(SRC_DIR)/,$(addsuffix .h,$(TARGETS)))
TARGET_OBJ_FILES := $(addprefix $(RELEASE_DIR)/,$(addsuffix .o,$(TARGETS)))

# Base files.
BASE_CPP_FILES := $(wildcard $(BASE_DIR)/*.cpp)
BASE_HDR_FILES := $(BASE_CPP_FILES:.cpp=.h)
BASE_OBJ_FILES := $(addprefix $(RELEASE_DIR)/,$(notdir $(BASE_CPP_FILES:.cpp=.o)))

# Interface files.
INTERFACE_CPP_FILES := $(wildcard $(INTERFACE_DIR)/*.cpp)
INTERFACE_HDR_FILES := $(INTERFACE_CPP_FILES:.cpp=.h)
INTERFACE_OBJ_FILES := $(addprefix $(RELEASE_DIR)/,$(notdir $(INTERFACE_CPP_FILES:.cpp=.o)))


$(TARGET_OBJ_FILES): $(RELEASE_DIR)/%.o: $(SRC_DIR)/%.cpp $(SRC_DIR)/%.h
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(BASE_OBJ_FILES): $(RELEASE_DIR)/%.o: $(BASE_DIR)/%.cpp $(BASE_DIR)/%.h
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(INTERFACE_OBJ_FILES): $(RELEASE_DIR)/%.o: $(INTERFACE_DIR)/%.cpp $(INTERFACE_DIR)/%.h
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(RELEASE_DIR)/main.o: $(SRC_DIR)/main.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@	

all: $(OUTPUT_FILE)
$(OUTPUT_FILE): $(TARGET_OBJ_FILES) $(BASE_OBJ_FILES) $(INTERFACE_OBJ_FILES) $(RELEASE_DIR)/main.o
	$(CC) -o $(OUTPUT_FILE) $(CFLAGS) $(TARGET_OBJ_FILES) $(BASE_OBJ_FILES) $(INTERFACE_OBJ_FILES) $(RELEASE_DIR)/main.o $(LIB_FILES)
	rm -f $(RELEASE_DIR)/*.o

clean:
	rm -f $(RELEASE_DIR)/*.o
	rm -f $(OUTPUT_FILE)
