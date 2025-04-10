#ifndef __APP_BUFFER_H__
#define __APP_BUFFER_H__
#include <pthread.h>
// 内部的子缓冲区
typedef struct
{
    char *ptr;      // 指向缓冲区的指针
    int total_size; // 缓冲区的总大小
    int len;        // 已存数据的长度

} SubBuffer;

// 缓冲区
typedef struct
{
    SubBuffer *sub_buffers[2];  // 2个子缓冲区
    int read_index;             // 读索引
    int write_index;            // 写索引
    pthread_mutex_t read_lock;  // 读锁
    pthread_mutex_t write_lock; // 写锁

} Buffer;

/**
 * @brief 初始化缓冲区
 *
 * @param size 缓冲区的大小
 * @return Buffer* 缓冲区的指针
 */
Buffer *app_buffer_init(int size);

/**
 * @brief 释放缓冲区
 *
 * @param buffer 要释放的缓冲区
 */
void app_buffer_free(Buffer *buffer);

/**
 * @brief 向缓冲区写入数据
 *
 * @param buffer 向哪个缓冲区写入数据
 * @param data 写入数据的内容
 * @param data_len 写入数据的长度
 * @return int
 */
int app_buffer_write(Buffer *buffer, char *data, int data_len);

/**
 * @brief 从缓冲区读取数据
 *
 * @param buffer 向哪个缓冲区读取数据
 * @param data_buf 接收读取数据的容器
 * @param buf_size 接收数据容器的大小，必须大于等于要读取的数据长度
 * @return int
 */
int app_buffer_read(Buffer *buffer, char *data_buf, int buf_size);

#endif /* __APP_BUFFER_H__ */
