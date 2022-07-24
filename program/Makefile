CC=gcc
CFLAGS=-c -Wall
LIBOBJ=server.o client.o common.o

all: msg_client msg_server

msg_client: msg_client.o $(LIBOBJ)
	$(CC) $^ -o $@ -pthread

msg_server: msg_server.o $(LIBOBJ)
	$(CC) $^ -o $@ -pthread

%.o: %.c
	$(CC) $(CFLAGS) $^ -o $@ -pthread

.PHONY: clean
clean:
	rm -rf *.o
	rm -rf msg_client msg_server
