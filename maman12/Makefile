# -------- Makefile (put in maman11/task1) --------
CC      := gcc
CFLAGS  := -Wall -pedantic -ansi
TARGET  := magic        # executable name
SRCS    := magic.c      # source file(s)

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

# Remove build artefacts
clean:
	$(RM) $(TARGET) output.txt
# -----------------------------------------------
