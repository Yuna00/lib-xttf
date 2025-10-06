#################################
#  Author    : Mads Bay Jensen  #
#  Date      : 30-8-2025        #
#  License   : GPLv2            #
#################################

CC = gcc
CFLAGS = -O0 -pipe -g3 -Wall #-fsanitize=address
LDFLAGS =
LIBS = -lxcb -lxcb-util
TARGET = prog

SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, build/%.o, $(SRC))

all: $(OBJ)
	$(CC) $(LDFLAGS) $(CFLAGS) -I inc $^ -o $(TARGET) $(LIBS)

build/%.o: src/%.c | build
	$(CC) $(CFLAGS) -I inc -c $< -o $@

build:
	mkdir build

clean:
	rm -rf build $(TARGET)

run: $(TARGET)
	./$<

.PHONY: clean run
