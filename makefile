.SUFFIXES:.cpp .o

CC=g++

SRCS=main.cpp
OBJS=$(SRCS:.cpp=.o)
EXEC=main

start: $(OBJS)
	$(CC) -o $(EXEC) $(OBJS)

.cpp.o:
	$(CC) -Wall -o $@ -cpp $<

clean:
	rm -rf $(EXEC) $(OBJS)
