NAME=gauge
OBJS=gauge.o Gaugette.o IfStats.o
CC=g++
CFLAGS=-Wall -Wno-write-strings -g

all:	$(NAME)

$(NAME):	$(OBJS)
	$(CC) $(OBJS) -o$(NAME)

gauge.o:	gauge.cpp

Gaugette.o: Gaugette.cpp Gaugette.h

clean:
	rm -f $(OBJS) $(NAME)
