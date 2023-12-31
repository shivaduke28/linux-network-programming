#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>  // for open
#include <unistd.h> // for close
#include <errno.h>
#include <arpa/inet.h>
#include <string.h>

int main()
{
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    // socketの生成
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        perror("socket");
        printf("%d\n", errno);
        exit(EXIT_FAILURE);
    }

    // socketの設定
    // ポート番号
    // どのアドレスからの接続も受け入れるように待ち受ける
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(12345);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // ソケットのオプションを指定
    int reuseaddr = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr));

    // bindする
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("bind");
        printf("%d\n", errno);
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // TCPクライアントからの接続要求を待てる状態にする
    if (listen(server_socket, 5) == -1)
    {
        perror("listen");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    char request_buffer[2048];
    char response_buffer[2048];

    memset(response_buffer, 0, sizeof(response_buffer));
    snprintf(response_buffer, sizeof(response_buffer),
             "HTTP/1.0 200 OK\r\n"
             "Content-Length: 20\r\n"
             "Content-Type: text/html\r\n"
             "\r\n"
             "HELLO WORLD");

    while (1)
    {
        // TCPクライアントからの接続要求を受け付ける
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_socket < 0)
        {
            perror("accept");
            printf("%d\n", errno);
            break;
        }

        printf("accepted connection from %s, port=%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        memset(request_buffer, 0, sizeof(request_buffer));
        ssize_t bytes_read = recv(client_socket, request_buffer, sizeof(request_buffer), 0);
        if (bytes_read <= 0)
        {
            perror("Receiving data failed.");
            close(client_socket);
            continue;
        }

        printf("%s", request_buffer);

        // Null-terminate the receiving data
        request_buffer[bytes_read] = '\0';

        // Parse the HTTP request
        char *method = strtok(request_buffer, " ");
        char *url = strtok(NULL, " ");
        char *version = strtok(NULL, "\r\n");

        printf("Method: %s\n", method);
        printf("URL: %s\n", url);
        printf("Version: %s\n", version);

        send(client_socket, response_buffer, (int)strlen(response_buffer), 0);

        // TCPセッションの終了
        close(client_socket);
    }

    // listenするソケットの終了
    close(server_socket);

    exit(EXIT_SUCCESS);
}