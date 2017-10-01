#!Makefile

PROGRAM = chat_room_cli

vpath %.h ./Common/libb64/include/b64/

C_SOURCES = $(shell find . -name "*.c")
C_OBJECTS = $(patsubst %.c, %.o, $(C_SOURCES))

CC = gcc

C_FLAGS = -DDEBUG -c -Wall -Wextra -ggdb -ICommon/libb64/include

all: $(C_OBJECTS) 
	@echo link...
	$(CC) $(C_OBJECTS) -lm -lpthread -o $(PROGRAM)

.c.o:
	@echo compile $< ...
	$(CC) $(C_FLAGS) $< -o $@ 

.PHONY:clean
clean:
	$(RM) $(C_OBJECTS) chat_room_cli

