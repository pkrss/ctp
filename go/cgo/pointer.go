package cgo

// #include <stdlib.h>
import "C"

import (
	"container/list"
	"unsafe"
)

type AutoPointer struct {
	l *list.List
}

func (this *AutoPointer) AddCString(p string) *C.char {
	cp := C.CString(p)
	this.AddPointer(unsafe.Pointer(cp))
	return cp
}

func (this *AutoPointer) AddPointer(p unsafe.Pointer) {
	if this.l == nil {
		this.l = list.New()
	}
	this.l.PushBack(p)
}

func (this *AutoPointer) ReleaseAll() {
	if this.l == nil || this.l.Len() == 0 {
		return
	}

	l2 := this.l
	this.l = list.New()

	b := l2.Front()
	e := l2.Back()

	for {
		switch v := b.Value.(type) {
		case unsafe.Pointer:
			C.free(v)
		}
		if b == e {
			break
		}
		b = b.Next()
	}
}
