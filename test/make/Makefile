# 目标名：依赖文件
#	命令1
#	命令2

hello.0: hello.c
	gcc -c hello.c -o hello.o

main: main.c hello.c hello.h
	gcc main.c hello.c -o main
	./main
	rm main