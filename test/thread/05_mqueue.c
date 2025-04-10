
#include <mqueue.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

static char *mq_name = "/my_mq"; // '/':表示绝对路径:/dev/mqueue, 'my_mq':消息队列目录名
static mqd_t mq_fd;              // 消息队列描述符   //description描述符

// first线程函数
void *first_thread_fun(void *arg) // 第一个线程函数
{
    sleep(1);
    printf("first线程开始执行, 发送消息\n");
    mq_send(mq_fd, "hello", 6, 0); // 发送消息

    return NULL;
}

// second线程函数
void *second_thread_fun(void *arg) // 第二个线程函数
{
    
    printf("second线程开始执行, 接收消息\n");

    // 创建容器以接收消息
    char buf[100];
    ssize_t len = mq_receive(mq_fd, buf, 100, NULL); // 接收消息
    printf("接收到的消息为:%s, 长度为:%ld\n", buf, len);
    return NULL;
}

int main(int argc, char const *argv[])
{
    printf("主线程开始执行\n");

    // 创建队列
    struct mq_attr attr;   // 消息队列属性
    attr.mq_maxmsg = 10;   // 最大消息数
    attr.mq_msgsize = 100; // 每个消息的最大字节数

    // 创建消息队列
    // O_CREAT: 如果不存在则创建。O_RDWR: 可读可写
    // 0666: 权限，创建文件的权限组合。NULL: 消息队列属性
    mq_fd = mq_open(mq_name, O_CREAT | O_RDWR, 0x644, &attr);
    if (mq_fd == -1)
    {
        printf("创建消息队列失败\n");
        return -1;
    }

    // 创建2个分线程
    pthread_t pt1, pt2;
    pthread_create(&pt1, NULL, first_thread_fun, NULL);
    pthread_create(&pt2, NULL, second_thread_fun, NULL);

    // 等待2个分线程执行完毕
    pthread_join(pt1, NULL);
    pthread_join(pt2, NULL);
    printf("主线程结束执行\n");

    // 关闭消息队列
    mq_close(mq_fd);
    mq_unlink(mq_name);

    return 0;
}
