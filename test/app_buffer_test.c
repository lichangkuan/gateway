#include "app_buffer.h"
#include "log/log.h"

int main(int argc, char const *argv[])
{
    // 初始化缓冲区
    Buffer *buffer = app_buffer_init(13);

    // 写数据
    app_buffer_write(buffer, "abcde", 5);
    app_buffer_write(buffer, "fghijk", 6);

    // 读数据1
    char data_buf[10];
    int data_len = app_buffer_read(buffer, data_buf, 10);
    log_debug("读取数据1: %.*s, 长度: %d", data_len, data_buf, data_len);

    // 读数据2
    char data_buf2[10];
    int data_len2 = app_buffer_read(buffer, data_buf2, 10);
    log_debug("读取数据2: %.*s, 长度: %d", data_len2, data_buf2, data_len2);

    // 读数据3
    char data_buf3[10];
    int data_len3 = app_buffer_read(buffer, data_buf3, 10);
    log_debug("读取数据3: %.*s, 长度: %d", data_len3, data_buf3, data_len3);

    // 释放缓冲区
    app_buffer_free(buffer);

    return 0;
}
