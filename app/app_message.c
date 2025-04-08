#include "cJSON/cJSON.h"
#include "log/log.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char *app_message_chars2Json(char *chars, int chars_len)
{
    // 生成json格式消息
    cJSON *root_obj = cJSON_CreateObject();
    cJSON *cJSON_AddNumberToObject
}




int app_message_json2chars(char *json, char *chars_buf, int buf_size)
{

}