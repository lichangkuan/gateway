#include "app_bt.h"
#include <string.h>
#include "log/log.h"

int app_bt_init(Device *device)
{
    
}

int app_bt_preWrite(char *data,int data_len)
{
    // 判断data数据的合法性
    if(data_len < 6)
    {
        log_error("要处理的消息长度不能小于6");
        return -1;
    }

    // 计算蓝牙数据的长度
    int blue_len = 8 + 2 + data[2] + 2;

    // 创建蓝牙数组容器
    char blue_data[blue_len];

    // 根据data中的数据组装蓝牙数据
    memcpy(blue_data,"AT+MESH",8);
    memcpy(blue_data+8,data+3,2);
    memcpy(blue_data+10,data+5,data[2]);
    memcpy(blue_data+10+data[2],"\r\n",2);

    // 清空data中的数据
    memset(data,0,data_len);
    // 将蓝牙数据拷贝到data中
    memcpy(data,blue_data,blue_len);

    // 返回蓝牙数据的长度
    return blue_len;
}

int app_bt_postRead(char *data,int data_len)
{
    
}