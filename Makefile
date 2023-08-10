CC = gcc

all: server client

server: src/server.c
	$(CC) -o ./bin/server $<
client: src/client.c
	$(CC) -o ./bin/client $<
