
# see http://zacg.github.io/blog/2013/06/06/calling-c-plus-plus-code-from-go-with-swig/

swig -go -intgosize 64 -c++ ctp.swig

go tool 6c -I $GOPATH/pkg/linux_amd64/ -D _64BIT ctp_gc.c
go tool 6g ctp.go
go tool pack grc ctp.a ctp.6 ctp_gc.6

# The last step is installation: first install your compiled C++ shared library, then run go install on the package created in the last step.
cd $GOPATH/src/simplelib/
go install