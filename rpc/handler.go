package rpc

type FunRegQuoteStk func(stks string)

var OnFunRegQuoteStk FunRegQuoteStk

type RpcServer struct{}

func (this *RpcServer) NotifyStkChange(stksString string) {
	if OnFunRegQuoteStk == nil {
		return
	}
	OnFunRegQuoteStk(stksString)
}
