#include "my-socket.h"
#include <iostream>

#define BUF_SIZE 100

int main()
{
    int server_fd = get_server_fd(8080);
    char buffer[BUF_SIZE] = {0};

    while (1)
    {
        // 接受客户端请求
        int client_fd = accept(server_fd, NULL, NULL);
        std::cout << "接收到客户端请求：" << client_fd << std::endl;

        while (1)
        {
            /**
             * 读取客户端发来的数据
             * read 是一个阻塞的系统调用，返回值是本次读取到的数据的长度（单位字节）
             * 触发时机为：
             * 1. 客户端发送数据
             * 2. 客户端断开连接
             * 当客户端断开连接时，read_len 为 0，表示 end of file
             * 这也在一定程度上加深了对“tcp 流式传输”的理解
             * https://man7.org/linux/man-pages/man2/read.2.html
             */
            int read_len = read(client_fd, buffer, BUF_SIZE);
            std::cout << "读取客户端数据：" << read_len << std::endl;

            // 出现错误
            if (read_len < 0)
            {
                exit(read_len);
            }

            // 读到文件末尾
            if (read_len == 0)
            {
                close(client_fd);
                std::cout << "关闭客户端" << client_fd << std::endl;
                continue;
            }
            else
            {
                // 返回数据给客户端
                write(client_fd, buffer, read_len);
            }
        }
    }

    close(server_fd);

    return 0;
}