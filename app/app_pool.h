#ifndef __APP_POOL_H__
#define __APP_POOL_H__

// 任务结构体
typedef struct
{
    int (*task_fun)(void *arg); // 任务函数
    void *arg;                  // 任务参数
} Task;

/**
 * @brief 创建线程池
 *
 * @param size 线程个数
 * @return int
 */
int app_pool_init(int size);

// 销毁线程池
void app_pool_destroy(void);

/**
 * @brief 添加任务到线程池
 *
 * @param task_fun
 * @param arg
 * @return int  0成功，-1失败
 */
int app_pool_addTask(int (*task_fun)(void *arg), void *arg);

#endif /* __APP_POOL_H__ */