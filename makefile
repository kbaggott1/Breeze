# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -g

# Source and header directories
SRC_DIR = source
HEADER_DIR = headers

# Source files
SRCS := $(wildcard $(SRC_DIR)/*.c)

# Executable name
EXEC = compiler.out

build: headers
	$(CC) $(CFLAGS) -I$(HEADER_DIR) $(SRCS) -o $(EXEC)

headers:
	python make_headers.py $(SRC_DIR) $(HEADER_DIR)


.PHONY: headers build