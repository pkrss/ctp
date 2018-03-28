package rpc

import (
	"net/http"

	"github.com/hprose/hprose-golang/rpc"
)

func StartRpcServer() {
	service := rpc.NewHTTPService()
	service.AddAllMethods(&RpcServer{})
	http.ListenAndServe(":8094", service)
}
