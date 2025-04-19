# -------- Makefile (maman11/task2) --------
CC      := gcc
CFLAGS  := -Wall -pedantic
TARGET  := count_zero_bits        # resulting executable
SRCS    := count_zero_bits.c      # add more .c files here if you have them

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^

run: $(TARGET) input.txt
	./$(TARGET) < input.txt > output.txt
	@echo "Output written to output.txt"

clean:
	$(RM) $(TARGET) output.txt
# ------------------------------------------
