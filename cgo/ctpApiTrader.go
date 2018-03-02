package cgo

/*
#include "impl/ctpApiTrader.h"
*/
import "C"

import (
	"log"
	"runtime"
	"sx98/ctp/conf"
	"syscall"
	"unsafe"
)

var apiTrader unsafe.Pointer

func GoCtpApiTraderApiCreate() {
	pszFlowPath := conf.Config.Data.Path

	p := AutoPointer{}

	handle, err := syscall.LoadDLL("thosttraderapi.dll")
	if err != nil {
		log.Printf("load thosttraderapi.dll: %s\n", err.Error())
		return
	}

	var funcName string
	switch runtime.GOOS {
	case "windows":
		funcName = "?CreateFtdcTraderApi@CThostFtdcTraderApi@@SAPEAV1@PEBD@Z"
	case "linux":
		funcName = "_ZN19CThostFtdcTraderApi19CreateFtdcTraderApiEPKc"
	default:
		funcName = "CThostFtdcTraderApi::CreateFtdcTraderApi"
	}
	createApi, err := handle.FindProc(funcName)
	if err != nil {
		log.Printf("FindProc %s not exist: %s\n", funcName, err.Error())
		return
	}
	api, _, err := createApi.Call(uintptr(unsafe.Pointer(p.AddCString(pszFlowPath))), 0, 0)

	apiTrader = C.ctpTraderInit(unsafe.Pointer(api))

	p.ReleaseAll()
}

func TraderApiRegQuoteStk(stks string) {

	if stks == "" {
		return
	}

	p := AutoPointer{}

	// C.ctpTraderRegQuoteStk(apiTrader, p.AddCString(stks))

	p.ReleaseAll()
}
