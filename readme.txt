
yum install -y gcc-c++ gcc

# 编译

export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH
gcc -o test test.c -Iinclude/ -Llib/v6.3.20160606_tradeapi64_windows/ -lthostmduserapi -lctpdll


./test




cgo: https://golang.org/cmd/cgo/
MSVC and MinGW DLLs: http://www.mingw.org/wiki/MSVC_and_MinGW_DLLs
