#!Makefile

PROGRAM = char_room_srv

C_SOURCES = $(shell find . -name "*.c")
C_OBJECTS = $(patsubst %.c, %.o, $(C_SOURCES))

CC = gcc

C_FLAGS = -DDEBUG -c -Wall -Wextra -ggdb -I.

all: $(C_OBJECTS) 
	@echo compile...
	$(CC) $(C_OBJECTS) -lm -lmysqlclient -o $(PROGRAM)

.c.o:
	@echo link$< ...
	$(CC) $(C_FLAGS) $< -o $@ 

.PHONY:clean
clean:
	$(RM) $(C_OBJECTS) char_room_srv

