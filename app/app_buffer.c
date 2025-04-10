#include "app_buffer.h"
#include "log/log.h"
#include <stdlib.h>
#include <string.h>

// 初始化子缓冲区
static SubBuffer *init_sub_buffer(int size)
{
    SubBuffer *sub_buffer = (SubBuffer *)malloc(sizeof(SubBuffer));
    sub_buffer->ptr = (char *)malloc(size);
    sub_buffer->total_size = size;
    sub_buffer->len = 0;

    return sub_buffer;
}

// 交换读写缓冲区
static void swap_sub_buffer(Buffer *buffer)
{
    // 如果正在写，不能切换：切换操作要加写锁
    // 加写锁
    log_debug("准备切换缓冲区前加写锁");
    pthread_mutex_lock(&buffer->write_lock);

    // 交换读写索引
    int temp = buffer->read_index;
    buffer->read_index = buffer->write_index;
    buffer->write_index = temp;

    // 解写锁
    log_debug("切换缓冲区后解写锁");
    pthread_mutex_unlock(&buffer->write_lock);
}

/**
 * @brief 初始化缓冲区
 *
 * @param size 缓冲区的大小
 * @return Buffer* 缓冲区的指针
 */
Buffer *app_buffer_init(int size)
{
    // 申请Buffer内存
    Buffer *buffer = (Buffer *)malloc(sizeof(Buffer));

    // 初始化Buffer的属性:结构体
    buffer->sub_buffers[0] = init_sub_buffer(size);
    buffer->sub_buffers[1] = init_sub_buffer(size);
    buffer->read_index = 0;
    buffer->write_index = 1;

    // 初始化读写锁
    pthread_mutex_init(&buffer->read_lock, NULL);
    pthread_mutex_init(&buffer->write_lock, NULL);

    return buffer;
}

/**
 * @brief 释放缓冲区
 *
 * @param buffer 要释放的缓冲区
 */
void app_buffer_free(Buffer *buffer)
{
    free(buffer->sub_buffers[0]->ptr);
    free(buffer->sub_buffers[1]->ptr);
    free(buffer->sub_buffers[0]);
    free(buffer->sub_buffers[1]);
    free(buffer);
}

/**
 * @brief 向缓冲区写入数据
 *
 * @param buffer 向哪个缓冲区写入数据
 * @param data 写入数据的内容
 * @param data_len 写入数据的长度
 * @return int
 */
int app_buffer_write(Buffer *buffer, char *data, int data_len)
{

    // 限制数据长度不要超过一字节
    if (data_len > 255)
    {
        log_error("数据长度不能超过255");
        return -1;
    }

    // 加写锁
    log_debug("写数据前加写锁");
    pthread_mutex_lock(&buffer->write_lock);

    // 得到写缓冲区
    SubBuffer *w_buffer = buffer->sub_buffers[buffer->write_index];

    // 判断写缓冲剩余空间是否足够
    if (w_buffer->total_size - w_buffer->len < data_len + 1)
    {
        log_error("写缓冲区剩余空间不足");
        pthread_mutex_unlock(&buffer->write_lock); // 解写锁
        return -1;
    }

    // 写入数据: 先写长度， 再写数据
    w_buffer->ptr[w_buffer->len] = data_len;
    memcpy(w_buffer->ptr + w_buffer->len + 1, data, data_len);

    // 更新写缓冲区的长度
    w_buffer->len += data_len + 1;

    // 解写锁
    log_debug("写数据后解写锁");
    pthread_mutex_unlock(&buffer->write_lock);

    return 0;
}

/**
 * @brief 从缓冲区读取数据
 *
 * @param buffer 向哪个缓冲区读取数据
 * @param data_buf 接收读取数据的容器
 * @param buf_size 接收数据容器的大小，必须大于等于要读取的数据长度
 * @return int
 */
int app_buffer_read(Buffer *buffer, char *data_buf, int buf_size)
{
    // 加读锁
    log_debug("读数据前加读锁");
    pthread_mutex_lock(&buffer->read_lock);

    // 得到读缓冲区
    SubBuffer *r_buffer = buffer->sub_buffers[buffer->read_index];

    // 如果读缓冲区为空，则切换到读写缓冲区
    if (r_buffer->len == 0)
    {
        swap_sub_buffer(buffer);
        // 得到读缓冲区
        r_buffer = buffer->sub_buffers[buffer->read_index];
        // 如果读缓冲区还是空，则返回-1
        if (r_buffer->len == 0)
        {
            log_error("读缓冲区为空");
            pthread_mutex_unlock(&buffer->read_lock); // 解读锁
            return -1;
        }
        return -1;
    }

    // 读取一份数据保存到data_buf中: 先读长度， 再读数据
    int data_len = r_buffer->ptr[0];

    // 判断读取的数据长度是否超过了缓冲区的大小
    // 这里的 buf_size 是我们要读取数据的容器的大小，必须大于等于要读取的数据长度
    // 这里的 data_len 是我们要读取的数据长度
    if (buf_size < data_len)
    {
        log_error("接收数据容器大小不足");
        pthread_mutex_unlock(&buffer->read_lock); // 解读锁
        return -1;
    }
    // memcpy 函数的作用是将内存中的数据从一个位置复制到另一个位置
    // 这里的意思是将读缓冲区中从 1 开始的数据复制到 data_buf 中
    // 第一个参数是目标地址，第二个参数是源地址，第三个参数是复制的字节数
    // 这里的 r_buffer->ptr + 1 是因为我们已经读取了一个字节的数据
    // 所以需要加上1
    // 这里的 data_len 是我们要读取的数据长度
    // 这里的 buf_size 是我们要读取数据的容器的大小，必须大于等于要读取的数据长度
    memcpy(data_buf, r_buffer->ptr + 1, data_len);

    // 将所有未读取的数据移动到读缓冲区头部
    // memmove 函数的作用是将内存中的数据从一个位置移动到另一个位置
    // 这里的意思是将读缓冲区中从 data_len + 1 开始的数据移动到缓冲区的头部
    // 也就是将读缓冲区中已经读取的数据删除
    // 这里的 r_buffer->ptr + data_len + 1 是因为我们已经读取了一个字节的数据
    // 所以需要加上1
    // 这里的 r_buffer->len - data_len - 1 是因为我们已经读取了一个字节的数据
    // 所以需要减去1
    // 第一个参数是目标地址，第二个参数是源地址，第三个参数是移动的字节数
    memmove(r_buffer->ptr, r_buffer->ptr + data_len + 1, r_buffer->len - data_len - 1);

    // 更新读缓冲区的长度
    r_buffer->len -= 1 + data_len;

    // 解读锁
    log_debug("读数据后解读锁");
    pthread_mutex_unlock(&buffer->read_lock);

    // 返回读取的数据长度
    return data_len;
}