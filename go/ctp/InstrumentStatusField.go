package ctp

// eg: {CFFEX TF 00000001 TF 54 99 15:15:00 49}
type CThostFtdcInstrumentStatusField struct {
	///交易所代码
	ExchangeID string `json:"exchangeID"`
	///合约在交易所的代码
	ExchangeInstID string `json:"exchangeInstID"`
	///结算组代码
	SettlementGroupID string `json:"settlementGroupID"`
	///合约代码
	InstrumentID string `json:"instrumentID"`
	///合约交易状态
	InstrumentStatus byte `json:"instrumentStatus"`
	///交易阶段编号
	TradingSegmentSN int `json:"tradingSegmentSN"`
	///进入本状态时间
	EnterTime string `json:"enterTime"`
	///进入本状态原因
	EnterReason byte `json:"enterReason"`
}
