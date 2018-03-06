package ctp

type CThostFtdcMarketDataField struct {
	///交易日
	TradingDay string `json:"tradingDay"`
	///合约代码
	InstrumentID string `json:"instrumentID"`
	///交易所代码
	ExchangeID string `json:"exchangeID"`
	///合约在交易所的代码
	ExchangeInstID string `json:"exchangeInstID"`
	///最新价
	LastPrice float64 `json:"lastPrice"`
	///上次结算价
	PreSettlementPrice float64 `json:"preSettlementPrice"`
	///昨收盘
	PreClosePrice float64 `json:"preClosePrice"`
	///昨持仓量
	PreOpenInterest float64 `json:"preOpenInterest"`
	///今开盘
	OpenPrice float64 `json:"openPrice"`
	///最高价
	HighestPrice float64 `json:"highestPrice"`
	///最低价
	LowestPrice float64 `json:"lowestPrice"`
	///数量
	Volume int `json:"volume"`
	///成交金额
	Turnover float64 `json:"turnover"`
	///持仓量
	OpenInterest float64 `json:"openInterest"`
	///今收盘
	ClosePrice float64 `json:"closePrice"`
	///本次结算价
	SettlementPrice float64 `json:"settlementPrice"`
	///涨停板价
	UpperLimitPrice float64 `json:"upperLimitPrice"`
	///跌停板价
	LowerLimitPrice float64 `json:"lowerLimitPrice"`
	///昨虚实度
	PreDelta float64 `json:"preDelta"`
	///今虚实度
	CurrDelta float64 `json:"currDelta"`
	///最后修改时间
	UpdateTime string `json:"updateTime"`
	///最后修改毫秒
	UpdateMillisec int `json:"updateMillisec"`
	///业务日期
	ActionDay string `json:"actionDay"`
}
