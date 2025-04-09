#define _GNU_SOURCE
#include "app_pool.h"
#include "log/log.h"
#include <unistd.h>

int task_func(void *arg)
{
    log_debug("任务函数(%d)执行，参数为：%s", gettid(), (char *)arg);
    return 0;
}
int main(int argc, char const *argv[])
{
    app_pool_init(5); // 初始化线程池，创建5个线程

    for (int i = 0; i < 10; i++)
    {
        app_pool_addTask(task_func, "abcd"); // 添加任务到线程池
    }
    sleep(1); // 等待任务执行完毕
    app_pool_destroy(); // 销毁线程池
    
    return 0;
}
