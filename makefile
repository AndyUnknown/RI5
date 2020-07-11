.SUFFIXES:.c .o

CC=g++

SRCS=main.cpp
OBJS=$(SRCS:.cpp=.o)
EXEC=main

start: $(OBJS)
	$(CC) -o $(EXEC) $(OBJS)

.c.o:
	$(CC) -Wall -o $@ -c $<

clean:
	rm -rf $(EXEC) $(OBJS)