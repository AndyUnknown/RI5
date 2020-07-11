.SUFFIXES:.c .o

CC=gcc

SRCS=main.cpp
OBJS=$(SRCS:.c=.o)
EXEC=main

start: $(OBJS)
	$(CC) -o $(EXEC) $(OBJS)

.c.o:
	$(CC) -Wall -o $@ -c $<

clean:
	rm -rf $(EXEC) $(OBJS)