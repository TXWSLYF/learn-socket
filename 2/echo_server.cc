#include <iostream>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main()
{
    // 创建套接字
    auto serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

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
    int clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);

    char buffer[100];                             // 缓冲区
    int strLen = read(clnt_sock, buffer, 100); // 接收客户端发来的数据
    write(clnt_sock, buffer, strLen);           // 将数据原样返回

    // 关闭套接字
    close(clnt_sock);
    close(serv_sock);

    return 0;
}