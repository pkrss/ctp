package cgo

/*
#cgo CXXFLAGS: -std=c++14
*/
import "C"
import (
	"log"
	"syscall"
	"unsafe"

	"github.com/pkrss/go-utils/conf"
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

var apiMd unsafe.Pointer

func Start() bool {
	// pszFlowPath := conf.GetString("data.path")

	p := AutoPointer{}

	handle, err := syscall.LoadDLL("ctphq.dll")
	if err != nil {
		log.Printf("load thostmduserapi.dll: %s\n", err.Error())
		return false
	}

	funcName := "my_ctp_init"
	fn, err := handle.FindProc(funcName)
	if err != nil {
		log.Printf("FindProc %s not exist: %s\n", funcName, err.Error())
		return false
	}
	_, _, _ = fn.Call()
	// if err != nil {
	// 	log.Printf("CallProc %s error: %s\n", funcName, err.Error())
	// 	return false
	// }

	funcName = "my_ctp_loop"
	fn, err = handle.FindProc(funcName)
	if err != nil {
		log.Printf("FindProc %s not exist: %s\n", funcName, err.Error())
		return false
	}
	_, _, _ = fn.Call()

	funcName = "my_ctp_uninit"
	fn, err = handle.FindProc(funcName)
	if err != nil {
		log.Printf("FindProc %s not exist: %s\n", funcName, err.Error())
		return false
	}
	_, _, _ = fn.Call()

	p.ReleaseAll()

	return true
}
