CC = gcc

all: server client

server: src/server.c
	$(CC) -o ./bin/server -c $<
client: src/client.c
	$(CC) -o ./bin/client -c $<
