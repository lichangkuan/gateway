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

# $^ 依赖列表
# $@ 目标文件
# -I 给gcc配置包含的查看路径