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


# Rule to link the executable
test:
	$(CC) $(CFLAGS) -I$(HEADER_DIR) $(SRCS) -o $(EXEC)