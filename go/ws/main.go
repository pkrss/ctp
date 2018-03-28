package main

import (
	"log"
	"sx98/ctp/cgo"
	"sx98/ctp/conf"

	"github.com/pkrss/go-utils/profile"
)

func Start() {

	conf.InitByConfigFile()

	profile.SetMyGetString(conf.GetString)

	// cgo.GoCtpApiMdApiCreate()
	cgo.GoCtpApiTraderApiCreate()

	// ctpRpc.OnFunRegQuoteStk = cgo.RegQuoteStk
	// ctpRpc.StartRpcServer()
}

func main() {
	log.Println("start begin...")

	messages := make(chan string)

	conf.Working = true

	Start()

	log.Println("start end...")

	<-messages

	conf.Working = false
}
