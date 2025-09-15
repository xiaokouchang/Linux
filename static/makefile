dy-lib=libmymethod.so
static-lib=libmymath.a

# 同时生成动态库和静态库
.PHONY:all
all: $(dy-lib) $(static-lib)


# 生成静态库
$(static-lib):mymath.o
	ar -rc $@ $^
mymath.o:mymath.c
	gcc -c $^

# 生成动态库
$(dy-lib):mylog.o myprint.o
	gcc -shared -o $@ $^
mylog.o:mylog.c
	gcc -fPIC -c $^
myprint.o:myprint.c
	gcc -fPIC -c $^

# 清理静态库文件
.PHONY:clean
clean:
	rm -rf *.o *.a *.so mylib

# 打包
.PHONY:output
output:
	mkdir -p mylib/include
	mkdir -p mylib/lib
	cp *.h mylib/include
	cp *.a mylib/lib 
	cp *.so mylib/lib
