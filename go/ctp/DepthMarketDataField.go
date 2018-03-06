package ctp

type CThostFtdcDepthMarketDataField struct {

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
	///申买价一
	BidPrice1 float64 `json:"bidPrice1"`
	///申买量一
	BidVolume1 int `json:"bidVolume1"`
	///申卖价一
	AskPrice1 float64 `json:"askPrice1"`
	///申卖量一
	AskVolume1 int `json:"askVolume1"`
	///申买价二
	BidPrice2 float64 `json:"bidPrice2"`
	///申买量二
	BidVolume2 int `json:"bidVolume2"`
	///申卖价二
	AskPrice2 float64 `json:"askPrice2"`
	///申卖量二
	AskVolume2 int `json:"askVolume2"`
	///申买价三
	BidPrice3 float64 `json:"bidPrice3"`
	///申买量三
	BidVolume3 int `json:"bidVolume3"`
	///申卖价三
	AskPrice3 float64 `json:"askPrice3"`
	///申卖量三
	AskVolume3 int `json:"askVolume3"`
	///申买价四
	BidPrice4 float64 `json:"bidPrice4"`
	///申买量四
	BidVolume4 int `json:"bidVolume4"`
	///申卖价四
	AskPrice4 float64 `json:"askPrice4"`
	///申卖量四
	AskVolume4 int `json:"askVolume4"`
	///申买价五
	BidPrice5 float64 `json:"bidPrice5"`
	///申买量五
	BidVolume5 int `json:"bidVolume5"`
	///申卖价五
	AskPrice5 float64 `json:"askPrice5"`
	///申卖量五
	AskVolume5 int `json:"askVolume5"`
	///当日均价
	AveragePrice float64 `json:"averagePrice"`
	///业务日期
	ActionDay string `json:"actionDay"`
}
