#include <stdio.h>  // perror()
#include <fcntl.h>  // open()
#include <stdlib.h> // exit()
#include <unistd.h> // read()/write()/close()

int main(int argc, char const *argv[])
{
    // 打开文件流: 读写
    int read_fd = open("elden.txt", O_RDONLY);
    if (read_fd == -1)
    {
        printf("打开读文件失败 \n");
        return -1;
    }
    int write_fd = open("elden2.txt", O_CREAT | O_WRONLY | O_APPEND, 0644);
    if (write_fd == -1)
    {
        printf("打开写文件失败 \n");
        close(read_fd);
        return -1;
    }

    // 不断边读边写，并输出到控制台
    char buffer[1024]; // 读取缓冲区
    ssize_t read_len;  // 读取到的数据长度
    while ((read_len = read(read_fd, buffer, sizeof(buffer))) > 0)
    {
        write(write_fd, buffer, read_len);      // 往新文件写入
        write(STDOUT_FILENO, buffer, read_len); // 输出到控制台
    }

    // 关闭文件流
    close(read_fd);
    close(write_fd);
    return 0;
}
