#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>  // for open
#include <unistd.h> // for close
#include <errno.h>

int main()
{
    int sock0;
    struct sockaddr_in addr;
    struct sockaddr_in client;

    int len;
    int sock;
    int bin;

    // socketの生成
    sock0 = socket(AF_INET, SOCK_STREAM, 0);
    if (sock0 == -1)
    {
        perror("socket");
        printf("%d\n", errno);
        exit(EXIT_FAILURE);
    }

    // socketの設定
    // ポート番号
    // どのアドレスからの接続も受け入れるように待ち受ける
    addr.sin_family = AF_INET;
    addr.sin_port = htons(12345);
    addr.sin_addr.s_addr = INADDR_ANY;

    // bindする
    if (bind(sock0, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        perror("bind");
        printf("%d\n", errno);
        close(sock0);
        exit(EXIT_FAILURE);
    }

    // TCPクライアントからの接続要求を待てる状態にする
    if (listen(sock0, 5) == -1)
    {
        perror("listen");
        close(sock0);
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        // TCPクライアントからの接続要求を受け付ける
        len = sizeof(client);
        sock = accept(sock0, (struct sockaddr *)&client, &len);
        if (sock < 0)
        {
            perror("accept");
            printf("%d\n", errno);
            break;
        }

        // 5文字送信
        if (write(sock, "HELLO", 5) == -1)
        {
            perror("write");
            break;
        }

        // TCPセッションの終了
        close(sock);
    }

    // listenするソケットの終了
    close(sock0);

    exit(EXIT_SUCCESS);
}