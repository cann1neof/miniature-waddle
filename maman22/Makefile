# Matrix Calculator Makefile
# Compiler settings for C90 compliance
CC      := gcc
CFLAGS  := -Wall -pedantic -ansi
TARGET  := mainmat        # executable name
SRCS    := mainmat.c mymat.c commands.c command_queue.c      # source file(s)

.PHONY: all run clean

# Default target: build the program
all: $(TARGET)

# Linking/compiling rule
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^

# Run the program with the provided test file
run: $(TARGET) input.txt
	./$(TARGET) < input.txt > output.txt
	@echo "Program output captured in output.txt"

# Remove build artifacts
clean:
	$(RM) $(TARGET) output.txt
# -----------------------------------------------
