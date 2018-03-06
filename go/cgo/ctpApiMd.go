package cgo

/*
#include "impl/ctpApiMd.h"
*/
import "C"

import (
	"log"
	"runtime"
	"sx98/ctp/conf"
	"syscall"
	"unsafe"
)

var apiMd unsafe.Pointer

func GoCtpApiMdApiCreate() {
	pszFlowPath := conf.Config.Data.Path

	p := AutoPointer{}

	handle, err := syscall.LoadDLL("thostmduserapi.dll")
	if err != nil {
		log.Printf("load thostmduserapi.dll: %s\n", err.Error())
		return
	}

	var funcName string
	switch runtime.GOOS {
	case "windows":
		funcName = "?CreateFtdcMdApi@CThostFtdcMdApi@@SAPEAV1@PEBD_N1@Z"
	case "linux":
		funcName = "_ZN15CThostFtdcMdApi15CreateFtdcMdApiEPKcbb"
	default:
		funcName = "CThostFtdcMdApi::CreateFtdcMdApi"
	}
	funCThostFtdcMdApi_CreateFtdcMdApi, err := handle.FindProc(funcName)
	if err != nil {
		log.Printf("FindProc %s not exist: %s\n", funcName, err.Error())
		return
	}
	mdApi, _, err := funCThostFtdcMdApi_CreateFtdcMdApi.Call(uintptr(unsafe.Pointer(p.AddCString(pszFlowPath))), 0, 0)

	apiMd = C.ctpMdInit(unsafe.Pointer(mdApi))

	p.ReleaseAll()
}

func GoCtpApiMdApiRegQuoteStk(stks string) {

	if stks == "" {
		return
	}

	p := AutoPointer{}

	C.ctpMdRegQuoteStk(apiMd, p.AddCString(stks))

	p.ReleaseAll()
}
