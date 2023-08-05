#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <fcntl.h>  // for open
#include <unistd.h> // for close
#include <string.h>
#include <arpa/inet.h>

int main()
{
    struct sockaddr_in server;
    int sock;
    char buf[32];
    int n;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        perror("socket");
        printf("%d\n", errno);
        exit(EXIT_FAILURE);
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(12345);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        perror("connect");
        printf("%d\n", errno);
        exit(EXIT_FAILURE);
    }

    memset(buf, 0, sizeof(buf));
    n = read(sock, buf, sizeof(buf));
    printf("%d, %s\n", n, buf);

    close(sock);
    exit(EXIT_SUCCESS);
}