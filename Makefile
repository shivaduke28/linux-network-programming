CC = gcc

all: server client

server: src/server.c
	$(CC) -o ./build/server -c $<
client: src/client.c
	$(CC) -o ./build/client -c $<
