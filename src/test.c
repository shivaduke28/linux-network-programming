#include <stdio.h>
#include <sys/socket.h>

#include <errno.h>

int main()
{
    printf("Hello World\n");
    int sock;
    // sock = socket(AF_INET, SOCK_STREAM, 0);
    sock = socket(3000, 4000, 5000);
    if (sock < 0)
    {
        perror("socket");
        printf("%d\n", errno);
        return 1;
    }
    return 0;
}