package main

import (
	"log"

	"github.com/pkrss/ctp/go/quickly/cgo"
	"github.com/pkrss/go-utils/conf"

	"github.com/pkrss/go-utils/profile"
)

func start() {

	conf.InitByConfigFile("./conf/config.json")

	profile.SetMyGetString(conf.GetString)

	// cgo.GoCtpApiMdApiCreate()
	cgo.Start()

	// ctpRpc.OnFunRegQuoteStk = cgo.RegQuoteStk
	// ctpRpc.StartRpcServer()
}

func main() {
	log.Println("start begin...")

	// messages := make(chan string)

	start()

	log.Println("start end...")

	// <-messages

}
