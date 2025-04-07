log :=  thirdparty/log/log.h thirdparty/log/log.c
log_test: test/log_test.c $(log)
	-gcc $^ -o $@ -I thirdparty
	-./$@
	-rm $@

# $^ 依赖列表
# $@ 目标文件
# -I 给gcc配置包含的查看路径