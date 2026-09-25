
SRC_DIR = ./examples
INC_DIRS = ./include
BUILD_DIR = ./build
CFLAGS = -ggdb -Wall -Wextra

SHELL = /bin/sh
CC = gcc

INC_FLAGS := $(addprefix -I,$(INC_DIRS))

.SUFFIXES:
.SUFFIXES: .c .o

.PHONY: all
all: main

main: $(SRC_DIR)/main.c
	$(CC) $(CFLAGS) $(INC_FLAGS) $(SRC_DIR)/main.c -o $(BUILD_DIR)/main

.PHONY:clean
clean:
	rm $(addprefix $(BUILD_DIR)/,$(shell ls -A -U $(BUILD_DIR)))