#include "my-socket.h"
#include <iostream>
#include <sys/stat.h>
#include <fcntl.h>

#define BUF_SIZE 100

int main()
{
    // 以只读模式打开文件
    int file_fd = open("./demo-server.txt", O_RDONLY);
    std::cout << "打开文件：" << file_fd << std::endl;

    int server_fd = get_server_fd(8080);
    char buffer[BUF_SIZE] = {0};

    while (1)
    {
        // 接受客户端请求
        int client_fd = accept(server_fd, NULL, NULL);
        std::cout << "接收到客户端请求：" << client_fd << std::endl;

        while (1)
        {
            int read_len = read(file_fd, buffer, BUF_SIZE);
            std::cout << "读取文件数据：" << read_len << std::endl;

            // 出现错误
            if (read_len < 0)
            {
                exit(read_len);
            }

            // 读到文件末尾
            if (read_len == 0)
            {
                /**
                 * 文件 offset 重新定位到文件初始位置
                 * https: //man7.org/linux/man-pages/man2/lseek.2.html
                 */
                lseek(file_fd, 0, SEEK_SET);

                close(client_fd);
                std::cout << "关闭客户端连接" << client_fd << std::endl;
                break;
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