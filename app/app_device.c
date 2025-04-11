#include "app_device.h"
#include "log/log.h"
#include "app_message.h"
#include "app_common.h"
#include "app_mqtt.h"
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "app_pool.h"

#define BUFFER_SIZE 1024

static Device *device; // 整个应用中只有一个Device对象

Device *app_device_init(char *filename)
{
    if (device)
    {
        return device;
    }

    // 申请device内存
    device = (Device *)malloc(sizeof(Device));

    // 初始化device中的各个成员
    device->filename = filename;
    device->fd = open(filename, O_RDONLY);
    device->up_buffer = app_buffer_init(BUFFER_SIZE);
    device->down_buffer = app_buffer_init(BUFFER_SIZE);
    device->is_running = 0;
    device->post_read = NULL;
    device->pre_write = NULL;

    // 返回device指针
    return device;
}

// 不断从串口文件中读取数据的线程函数
static void *read_thread_fun(void *arg)
{
    while (device->is_running)
    {
        /* 从串口中读数据: 蓝牙数据 */
        char data_buf[128];
        ssize_t data_len = read(device->fd, data_buf, sizeof(data_buf));

        // 将蓝牙数据转换为字符数组Message
        if (data_len > 0 && device->post_read)
        {
            data_len = device->post_read(data_buf, data_len);
        }
        if (data_len > 0)
        {
            // 将Message写入到上行缓冲区
            app_buffer_write(device->up_buffer, data_buf, data_len);

            // 将发送消息的任务交给线程池模块完成，注册一个发送消息到远程的任务
            
        }
    }
}

int app_device_start()
{
    if (device->is_running)
    {
        log_debug("读线程已经在运行中");
        return 0;
    }
    device->is_running = 1;
    // 启动上行流程：启动读线程
    pthread_create(&device->read_thread, NULL, read_thread_fun, NULL);

    // 启动下行流程：给MQTT模块注册一个接收远程消息的回调函数
    app_pool_addTask();
}

void app_device_close()
{
}