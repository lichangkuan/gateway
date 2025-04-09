
#include <mqueue.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

static char *mq_name = "/my_mq";    
static mqd_t mq_fd; // 消息队列描述符

// first线程函数
void *first_thread_fun(void *arg) // 第一个线程函数
{
    printf("first线程开始执行, 发送消息\n");
    mq_send(mq_fd, "hello", 6, 0);  // 发送消息

    return NULL;
}

// second线程函数
void *second_thread_fun(void *arg) // 第二个线程函数
{
    printf("second线程开始执行, 接收消息\n");

    mq_receive(mq_fd, NULL, 0, NULL);   // 接收消息
    return NULL;
}

int main(int argc, char const *argv[])
{
    printf("主线程开始执行\n");

    // 创建队列
    struct mq_attr attr; // 消息队列属性
    attr.mq_maxmsg = 10; // 最大消息数
    attr.mq_msgsize = 100; // 每条消息的最大字节数

    // 创建消息队列
    // O_CREAT: 如果不存在则创建。O_RDWR: 可读可写
    // 0666: 权限，创建文件的权限组合。NULL: 消息队列属性
    mq_open(mq_name, O_CREAT | O_RDWR, 0666, NULL);

    // 创建2个分线程
    pthread_t pt1, pt2;
    pthread_create(&pt1, NULL, first_thread_fun, NULL);
    pthread_create(&pt2, NULL, second_thread_fun, NULL);

    // 等待2个分线程执行完毕
    pthread_join(pt1, NULL);
    pthread_join(pt2, NULL);
    printf("主线程结束执行");
    return 0;
}
