#ifndef __APP_DEVICE_H__
#define __APP_DEVICE_H__

#include "app_buffer.h"
#include "pthread.h"
// 设备的结构体
typedef struct
{
    char *filename;        /* 串口文件名 */
    int fd;                /* 串口文件描述符 */
    Buffer *up_buffer;      // 上行缓冲区
    Buffer *down_buffer;    // 下行缓冲区
    pthread_t read_thread; // 读取串口文件中数据的线程
    int is_running;         // 是否正在运行
    int (*post_read)(char *data, int len); // 读取到串口数据后的后置处理函数
    int (*pre_write)(char *data, int len); // 
} Device;

Device *app_device_init(char *filename);

int app_device_start();

void app_device_close();

#endif /* __APP_DEVICE_H__ */