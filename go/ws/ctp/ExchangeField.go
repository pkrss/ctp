package ctp

type CThostFtdcExchangeField struct {
	///交易所代码
	ExchangeID string `json:"exchangeID"`
	///交易所名称
	ExchangeName string `json:"exchangeName"`
	///交易所属性
	ExchangeProperty byte `json:"exchangeProperty"`
}
