#!Makefile

PROGRAM = chat_room_srv

C_SOURCES = $(shell find . -name "*.c")
C_OBJECTS = $(patsubst %.c, %.o, $(C_SOURCES))

CC = gcc

C_FLAGS = -DDEBUG -c -Wall -Wextra -ggdb3 -I.

all: $(C_OBJECTS) 
	@echo link...
	$(CC) $(C_OBJECTS) -lm -lpthread -L/usr/lib64/mysql -lmysqlclient -o $(PROGRAM)

.c.o:
	@echo compile $< ...
	$(CC) $(C_FLAGS) $< -o $@ 

.PHONY:clean
clean:
	$(RM) $(C_OBJECTS) chat_room_srv

