package cgo

/*
#cgo CXXFLAGS: -std=c++14
*/
import "C"
import (
	"sx98/ctp/conf"
)

//export ExtProfileGetString
func ExtProfileGetString(key *C.char) *C.char {
	s := conf.GetString(C.GoString(key))
	if s == "" {
		return nil
	}

	return C.CString(s)
}

//export ExtGbkToUtf8
func ExtGbkToUtf8(gbkStr *C.char) *C.char {
	s := gbk2utf8(C.GoString(gbkStr))
	return C.CString(s)
}
