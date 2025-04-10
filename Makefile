CC := gcc
CFLAGS := -g -O0 -Wall

log :=  thirdparty/log/log.h thirdparty/log/log.c
log_test: test/log_test.c $(log)
	-$(CC) $^ -o $@ -I thirdparty
	-./$@
	-rm $@

json := thirdparty/cJSON/cJSON.h thirdparty/cJSON/cJSON.c
json_test: test/json_test.c $(json) $(log)
	-$(CC) $(CFLAGS) $^ -o $@ -I thirdparty
#	-./$@
#	-rm $@

# 变量定义
app_common := app/app_common.c app/app_common.h
app_common_test: test/app_common_test.c $(app_common) $(log)
	-$(CC) $^ -o $@ -I app -I thirdparty
	-./$@
	-rm $@

app_message := app/app_message.c app/app_message.h
app_message_test: test/app_message_test.c $(app_common) $(app_message) $(log) $(json)
	-$(CC) $^ -o $@ -I app -I thirdparty
	-./$@
	-rm $@

app_pool := app/app_pool.c app/app_pool.h
app_pool_test: test/app_pool_test.c $(app_pool) $(log)
	-$(CC) $^ -o $@ -I app -I thirdparty
	-./$@
	-rm $@

app_buffer := app/app_buffer.h app/app_buffer.c
app_buffer_test: test/app_buffer_test.c $(app_buffer) $(log)
	-$(CC) $^ -o $@ -I app -I thirdparty
	-./$@
	-rm $@

# $^ 依赖列表
# $@ 目标文件
# -I 给gcc配置包含的查看路径