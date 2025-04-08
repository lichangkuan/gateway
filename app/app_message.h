#ifndef __APP_MESSAGE_H__
#define __APP_MESSAGE_H__

/**
 * @brief 将字符串转换为json格式
 * 
 * @param chars 字符数组消息
 * @param chars_len 字符数组长度
 * @return char* Json消息
 */
char *app_message_chars2Json(char *chars, int chars_len);

/**
 * @brief 将json格式转换为字符串
 * 
 * @param json json消息
 * @param chars_buf 字符数组消息的容器
 * @param buf_size 容器大小
 * @return int 实际写入的字符数组长度，-1表示失败
 */
int app_message_json2chars(char *json, char *chars_buf, int buf_size);

#endif /* __APP_MESSAGE_H__ */