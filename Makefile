CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
TARGET = toyforth
SRCS = src/main.c src/mem.c src/ops.c src/parser.c src/stack.c src/dictionary.c src/engine.c src/file_utils.c src/list.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TARGET)
	@bash run_tests.sh

clean:
	rm -f $(OBJS) $(TARGET)
	rm -f tests/*.out

.PHONY: all clean test