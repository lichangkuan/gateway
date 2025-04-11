#ifndef __APP_BT_H__
#define __APP_BT_H__

#include "app_device.h"

/**
 * @brief 蓝牙模块初始化
 * 1.给设备指定prewrite和postread两个函数
 * 2.蓝牙连接初始化
 * 
 * @param device 
 * @return int 
 */
int app_bt_init(Device *device);

int app_bt_preWrite(char *data,int data_len);

int app_bt_postRead(char *data,int data_len);



#endif /* __APP_BT_H__ */