package ctp

type CThostFtdcInstrumentField struct {
	///合约代码
	InstrumentID string `json:"instrumentID"`
	///交易所代码
	ExchangeID string `json:"exchangeID"`
	///合约名称
	InstrumentName string `json:"instrumentName"`
	///合约在交易所的代码
	ExchangeInstID string `json:"exchangeInstID"`
	///产品代码
	ProductID string `json:"productID"`
	///产品类型
	ProductClass byte `json:"productClass"`
	///交割年份
	DeliveryYear int `json:"deliveryYear"`
	///交割月
	DeliveryMonth int `json:"deliveryMonth"`
	///市价单最大下单量
	MaxMarketOrderVolume int `json:"maxMarketOrderVolume"`
	///市价单最小下单量
	MinMarketOrderVolume int `json:"minMarketOrderVolume"`
	///限价单最大下单量
	MaxLimitOrderVolume int `json:"maxLimitOrderVolume"`
	///限价单最小下单量
	MinLimitOrderVolume int `json:"minLimitOrderVolume"`
	///合约数量乘数 int
	VolumeMultiple int `json:"volumeMultiple"`
	///最小变动价位
	PriceTick float64 `json:"priceTick"`
	///创建日
	CreateDate string `json:"createDate"`
	///上市日
	OpenDate string `json:"openDate"`
	///到期日
	ExpireDate string `json:"expireDate"`
	///开始交割日
	StartDelivDate string `json:"startDelivDate"`
	///结束交割日
	EndDelivDate string `json:"endDelivDate"`
	///合约生命周期状态
	InstLifePhase byte `json:"instLifePhase"`
	///当前是否交易
	IsTrading int `json:"isTrading"`
	///持仓类型
	PositionType byte `json:"positionType"`
	///持仓日期类型
	PositionDateType byte `json:"positionDateType"`
	///多头保证金率
	LongMarginRatio float64 `json:"longMarginRatio"`
	///空头保证金率
	ShortMarginRatio float64 `json:"shortMarginRatio"`
	///是否使用大额单边保证金算法
	MaxMarginSideAlgorithm byte `json:"maxMarginSideAlgorithm"`
	///基础商品代码
	UnderlyingInstrID string `json:"underlyingInstrID"`
	///执行价
	StrikePrice float64 `json:"strikePrice"`
	///期权类型
	OptionsType byte `json:"optionsType"`
	///合约基础商品乘数
	UnderlyingMultiple float64 `json:"underlyingMultiple"`
	///组合类型
	CombinationType byte `json:"combinationType"`
}
