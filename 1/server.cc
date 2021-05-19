#include <iostream>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUF_SIZE 100

using namespace std;

int main()
{
    // 创建套接字
    int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // 解决 socket 关闭之后，端口占用问题
    // https://stackoverflow.com/questions/24194961/how-do-i-use-setsockoptso-reuseaddr/25193462
    int enable = 1;
    setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));

    /**
     * 将套接字和 IP、端口绑定
     */
    struct sockaddr_in serv_addr;             // 创建 socket 网络格式的地址
    memset(&serv_addr, 0, sizeof(serv_addr)); // 每个字节都填充为 0
    serv_addr.sin_family = AF_INET;           // 设置协议簇
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8080);
    bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    // 进入监听状态，等待用户发起请求
    listen(serv_sock, 20);

    // 接收客户端请求
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = sizeof(clnt_addr);
    char buffer[BUF_SIZE] = {0};

    cout << "before accept" << endl;

    while (1)
    {
        // 接受来自客户端的连接，这一步会导致进程阻塞，直到有来自客户端的链接，进程才会被唤醒
        int clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
        cout << "accepted, clnt_sock:" << clnt_sock << endl;

        // 接收客户端发来的数据
        int strLen = recv(clnt_sock, buffer, BUF_SIZE, 0);
        cout << "recved, clnt_sock:" << clnt_sock << endl;

        send(clnt_sock, buffer, strLen, 0); //将数据原样返回
        cout << "sended, clnt_sock:" << clnt_sock << endl;

        close(clnt_sock);            // 关闭套接字
        memset(buffer, 0, BUF_SIZE); // 重置缓冲区
    }

    close(serv_sock);

    return 0;
}