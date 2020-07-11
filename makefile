.SUFFIXES:.cpp .o

CC=g++ -fPIC

SRCS=main.cpp
OBJS=$(SRCS:.cpp=.o)
EXEC=main
.cpp.o:
	$(CC) -Wall -o $@ -cpp $<

clean:
	rm -rf $(EXEC) $(OBJS)
