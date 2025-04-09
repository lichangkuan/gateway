#include "app_pool.h"
#include "log/log.h"
#include <pthread.h>
#include <stdlib.h>

// 线程数量
static int thread_num; // 线程数量

static pthread_t *thread_pool; // 线程池中所有线程标识的容器

static mqd_t mq_fd; // 消息队列标识符

static char *mq_name = "/app_pool"; // 消息队列名称

// 线程函数
void *thread_fun(void *arg)
{
    Task task;
    while (1)
    {
        // 从消息队列中获取任务
        int len = mq_receive(mq_fd, (char *)&task, sizeof(Task), NULL);
        // 执行任务：任务函数
        if (len == sizeof(Task))
        {
            // 执行任务函数
            task.task_fun(task.arg);
        }
        else
        {
            // 任务获取失败
            log_error("获取任务失败");
        }
    }
    
}

int app_pool_init(int size)
{
    // 初始化线程池
    thread_num = size;
    
    thread_pool = malloc(sizeof(pthread_t) * size);

    // 创建线程
    for (int i = 0; i < size; i++)
    {
        pthread_create(&thread_pool[i], NULL, thread_fun, NULL);
    }
    
    // 初始化任务队列
    struct mq_attr mq_attr;
    mq_attr.mq_maxmsg = 10; // 最大消息数
    mq_attr.mq_msgsize = sizeof(Task); // 消息大小
    mq_attr.mq_curmsgs = 0; // 当前消息数
    mq_fd = mq_open(mq_name, O_CREAT | O_RDWR, 0666, &mq_attr);
    

}

void app_pool_destroy(void)
{
    // 关闭并删除消息队列
    mq_close(mq_fd);
    mq_unlink(mq_name); 

    // 销毁线程池
    for (int i = 0; i < thread_num; i++)
    {
        pthread_cancel(thread_pool[i]);
        pthread_join(thread_pool[i], NULL);
    }
    free(thread_pool);
}

int app_pool_addTask(int (*task_fun)(void *arg), void *arg)
{
    // 创建任务
    Task task = {
        .task_fun = task_fun,
        .arg = arg,
    };
    // 添加任务到消息队列
    return mq_send(mq_fd, (char *)&task, sizeof(Task), 1);
}