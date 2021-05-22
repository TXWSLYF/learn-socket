#include "my-socket.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>

#define BUF_SIZE 100

int main()
{
    /**
     * O_CREAT: 文件不存在自动创建
     * O_TRUNC: 清空文件所有内容
     * O_WRONLY: 只写
     * S_IRWXU: 文件权限
     */
    int file_fd = open("/Users/alex/code/github/learn-socket/file-server/demo-client.txt", O_CREAT | O_TRUNC | O_WRONLY, S_IRWXU);
    std::cout << "打开本地文件 demo-client.txt：" << file_fd << std::endl;

    // 建立缓冲区
    char buffer[BUF_SIZE] = {0};

    // 创建套接字
    int client_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // 向服务器（特定 IP + port）发送连接请求
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8080);
    connect(client_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    while (1)
    {
        int read_len = read(client_fd, buffer, BUF_SIZE);
        std::cout << "读取服务端数据：" << read_len << std::endl;

        if (read_len < 0)
        {
            exit(read_len);
        }

        if (read_len == 0)
        {
            std::cout << "读取完毕！" << read_len << std::endl;
            break;
        }
        else
        {
            write(file_fd, buffer, read_len);
        }
    }

    close(client_fd);
    close(file_fd);

    return 0;
}