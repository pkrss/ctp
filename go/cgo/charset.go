package cgo

import (
	"github.com/axgle/mahonia"
)

var gbk mahonia.Decoder

func gbk2utf8(gbkString string) string {
	if gbkString == "" {
		return ""
	}
	if gbk == nil {
		gbk = mahonia.NewDecoder("gbk")
	}
	return gbk.ConvertString(gbkString)
}
