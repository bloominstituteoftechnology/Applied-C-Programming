CC=gcc -Wall -Wextra

OBJS=server.o lastmessage.o

all: server

server.o: server.c lastmessage.h
	$(CC) -c $<

lastmessage.o: lastmessage.c lastmessage.h
	$(CC) -c $<

server: server.o lastmessage.o
	$(CC) $^ -o server

clean:
	rm -f server $(OBJS)
