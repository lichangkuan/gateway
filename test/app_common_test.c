#include "app_common.h"
#include "log/log.h"
int main(int argc, char const *argv[])
{
    long time = app_common_getCurrentTime();
    log_debug("当前时间戳: %ld", time);
    return 0;
}
