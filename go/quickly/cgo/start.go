package cgo

/*
#cgo CXXFLAGS: -std=c++14

typedef void(*FnCallback)(const char*)
typedef void(*FnSetCallback)(FnCallback cb)

extern void GoNotifyStringCallback(const char *);
void callCtpCb(void* p,void* p2){
	FnSetCallback fn = (FnSetCallback)p;
	fn((FnCallback)p2);
}
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

//export GoNotifyStringCallback
func GoNotifyStringCallback(s *C.char) {

}

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

	funcName = "setRealtimeQuoteNotifyCallback"
	fn, err = handle.FindProc(funcName)
	if err != nil {
		log.Printf("FindProc %s not exist: %s\n", funcName, err.Error())
		return false
	}
	C.callCtpCb(unsafe.Pointer(fn), C.GoNotifyStringCallback)
	// _, _, _ = fn.Call(uintptr(GoNotifyStringCallback))

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
