# Matrix Calculator Makefile
# Compiler settings for C90 compliance
CC      := gcc
CFLAGS  := -Wall -pedantic -ansi
TARGET  := numbers        # executable name
SRCS    := numbers.c     # source file(s)

# Optional arguments (can be overridden from command line)
ARGS    :=

.PHONY: all run clean

# Default target: build the program
all: $(TARGET)

# Linking/compiling rule
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^

# Run the program - supports optional arguments
# Usage: make run                    (uses stdio redirection)
# Usage: make run ARGS="input.txt output.txt"  (uses command line args)
run: $(TARGET)
ifeq ($(ARGS),)
	./$(TARGET)
else
	./$(TARGET) $(ARGS)
endif

# Remove build artifacts
clean:
	$(RM) $(TARGET) output.txt
# -----------------------------------------------
