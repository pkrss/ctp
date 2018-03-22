#ifndef _STRUCTS_H
#define _STRUCTS_H

#include <string>
#include "ThostFtdcUserApiDataType.h"

using namespace std;




// 策略结构
struct StrategyField
{
	int    BSQty;	// 每次买卖的合约数量

	double ModifyRange;		// 跟踪止损，调整的阀值
	double ModifyRatio;		// 跟踪止损，调整的幅度
	double Rongrendu;		// 开仓和平仓时的价差 容忍度
	string TradeBegTime;	// 每天策略开始执行的起始时间
	string ForceCloseTime;	// 每日强平时间，如14:58:00


};



///资金账户
struct AccountField
{

	/////投资者代码
	//string	InvestorID;
		
	///入金金额
	TThostFtdcMoneyType	Deposit;   // 初始资金

	///当前金额
	TThostFtdcMoneyType	NowCash;

	///冻结的保证金
	TThostFtdcMoneyType	FrozenMargin;
	///冻结的手续费
	TThostFtdcMoneyType	FrozenCommission;
		
	///当前保证金总额
	TThostFtdcMoneyType	CurrMargin;
	
	///手续费
	TThostFtdcMoneyType	Commission;

	///平仓盈亏
	TThostFtdcMoneyType	CloseProfit;
	///持仓盈亏
	TThostFtdcMoneyType	PositionProfit;
		
	///可用资金
	TThostFtdcMoneyType	Available;  
	
	///变动时间
	string	UpdateTime;

	// 入金+平仓盈亏+持仓盈亏-手续费 = 动态权益； 
	// 动态权益 -占用保证金 -冻结保证金 -冻结手续费 = 可用资金
	
};


/*
==========================================
  上次结算准备金：　      500,000,000.00
- 上次信用额度：　　                0.00
- 上次质押金额：　　                0.00
+ 质押金额：　　　　                0.00
- 今日出金：　　　　                0.00
+ 今日入金：　　　　                0.00
------------------------------------------
= 静态权益：　　　　      500,000,000.00
+ 平仓盈亏：　　　　                0.00
+ 持仓盈亏：　　　　                0.00
- 手续费：　　　　　                0.00
------------------------------------------
= 动态权益：　　　　      500,000,000.00
- 占用保证金：　　　                0.00
- 冻结保证金：　　　                0.00
- 冻结手续费：　　　                0.00
- 交割保证金：　　　                0.00
+ 信用金额：　　　　                0.00
------------------------------------------
= 可用资金：　　　　      500,000,000.00
==========================================

==========================================
  保底资金：　　　　                0.00
  可取资金：　　　　      500,000,000.00
==========================================
*/

///合约
struct InstrumentField
{
	///合约代码
	string	InstrumentID;
	///合约名称
	string	InstrumentName;
	///交易所代码
	string	ExchangeID;

	///合约数量乘数
	TThostFtdcVolumeMultipleType	VolumeMultiple;
	///最小变动价位
	TThostFtdcPriceType	PriceTick;
	
	// 保证金率-------------------------
	///多头保证金率
	TThostFtdcRatioType	LongMarginRatio;
	///空头保证金率
	TThostFtdcRatioType	ShortMarginRatio;

	// 手续费率---------------------------
	///开仓手续费
	TThostFtdcRatioType	OpenRatioByVolume;
	///平仓手续费
	TThostFtdcRatioType	CloseRatioByVolume;
	///平今手续费
	TThostFtdcRatioType	CloseTodayRatioByVolume;


	///开仓手续费率
	TThostFtdcRatioType	OpenRatioByMoney;
	///平仓手续费率
	TThostFtdcRatioType	CloseRatioByMoney;
	///平今手续费率
	TThostFtdcRatioType	CloseTodayRatioByMoney;

	///当前是否交易
	TThostFtdcBoolType	IsTrading;
};

///合约手续费率
struct CommissionRateField
{
	
	///合约代码
	TThostFtdcInstrumentIDType	InstrumentID;

	///开仓手续费
	TThostFtdcRatioType	OpenRatioByVolume;
	///平仓手续费
	TThostFtdcRatioType	CloseRatioByVolume;
	///平今手续费
	TThostFtdcRatioType	CloseTodayRatioByVolume;


	///开仓手续费率
	TThostFtdcRatioType	OpenRatioByMoney;
	///平仓手续费率
	TThostFtdcRatioType	CloseRatioByMoney;
	///平今手续费率
	TThostFtdcRatioType	CloseTodayRatioByMoney;
	
};


///深度行情
struct QuoteField
{

	//交易时间  yyyy-mm-dd hh:mm:ss.000
    string TradingTime;  

	///合约代码
	string	InstrumentID;
	///交易所代码
	string	ExchangeID;
	///合约在交易所的代码
	//TThostFtdcExchangeInstIDType	ExchangeInstID;
	
	///上次结算价
	TThostFtdcPriceType	PreSettlementPrice;
	///昨收盘
	TThostFtdcPriceType	PreClosePrice;
	///昨持仓量
	TThostFtdcLargeVolumeType	PreOpenInterest;
	///今开盘
	TThostFtdcPriceType	OpenPrice;
	///最高价
	TThostFtdcPriceType	HighestPrice;
	///最低价
	TThostFtdcPriceType	LowestPrice;
	///最新价
	TThostFtdcPriceType	LastPrice;

	///数量
	TThostFtdcVolumeType	Volume;
	///成交金额
	TThostFtdcMoneyType	Turnover;
	///持仓量
	TThostFtdcLargeVolumeType	OpenInterest;

	
	// 增仓
    double AddPosition;  
	// 开仓
    double OpenPosition;
	// 平仓
	double ClosePosition;
	// 成交类型
	string TradingType;  // 多开、空开、多平、空平、多换、空换、双开、双平、

	// 方向
	string BuySell;  // 买卖： B/S

	///涨停板价
	//TThostFtdcPriceType	UpperLimitPrice;
	///跌停板价
	//TThostFtdcPriceType	LowerLimitPrice;

	///最后修改时间
	//TThostFtdcTimeType	UpdateTime;
	/////最后修改毫秒
	//TThostFtdcMillisecType	UpdateMillisec;

	///申买价一
	TThostFtdcPriceType	BidPrice1;
	///申买量一
	TThostFtdcVolumeType	BidVolume1;
	///申卖价一
	TThostFtdcPriceType	AskPrice1;
	///申卖量一
	TThostFtdcVolumeType	AskVolume1;

	///今收盘
	TThostFtdcPriceType	ClosePrice;
	///本次结算价
	TThostFtdcPriceType	SettlementPrice;


};

///深度行情
struct DailyQuoteField
{

	//交易日  yyyy-mm-dd
    string TradingDate;  

	///合约代码
	string	InstrumentID;
	///交易所代码
	string	ExchangeID;
	
	///今开盘
	TThostFtdcPriceType	OpenPrice;
	///最高价
	TThostFtdcPriceType	HighestPrice;
	///最低价
	TThostFtdcPriceType	LowestPrice;
	///最新价
	TThostFtdcPriceType	LastPrice;

	///数量
	TThostFtdcVolumeType	Volume;
	///成交金额
	TThostFtdcMoneyType	Turnover;
	///持仓量
	TThostFtdcLargeVolumeType	OpenInterest;
	
};


// 指标结构
struct IndicatorField
{
	//交易时间  yyyy-mm-dd hh:mm:ss.000
    string TradingTime;  

	///合约代码
	string	InstrumentID;

	///最新价
	TThostFtdcPriceType	LastPrice;

	///申买价一
	TThostFtdcPriceType	BidPrice1;
	///申买量一
	TThostFtdcVolumeType	BidVolume1;
	///申卖价一
	TThostFtdcPriceType	AskPrice1;
	///申卖量一
	TThostFtdcVolumeType	AskVolume1;

	double Avg10;
	double Avg120;
	double Avg240;
	double AvgDay;

	double MaxPrice; // 今日开盘以来的最高价
	double Max10;
	double Max120;

	double MinPrice; // 今日开盘以来的最低价
	double Min10;
	double Min120;


	string BuySell;

};



///报单
struct OrderField
{

	///投资者代码
	string	InvestorID;

	///策略ID
	string StrategyID;

	///合约代码
	string	InstrumentID;

	///委托时间
	string	InsertTime;  // yyyy-mm-dd hh:mm:ss.000

	///报单引用
	long	OrderRef;

	///平仓ID
	long OpenOrderRef;  // = TradeID

	///组合开平标志
	string	CombOffsetFlag;

	///买卖方向
	string	Direction;
	
	///报单价格
	double	OrderPrice;
	///报单数量
	int	OrderVolume;
	///报单金额
	double OrderAmount;

	///冻结的保证金
	TThostFtdcMoneyType	FrozenMargin;
	///冻结的手续费
	TThostFtdcMoneyType	FrozenCommission;

	///止损价
	double	StopPrice;
	
	///报单状态
	int	OrderStatus;  // 1-未成交，0-已成交，-1-已撤单
	
	///状态信息
	string	StatusMsg;

	///撤销时间
	string	CancelTime;

	///成交时间
	string	UpdateTime;
	
	///成交价格
	double	Price;
	///成交数量
	int	Volume;
	///成交金额
	double Amount;

	///保证金
	TThostFtdcMoneyType	Margin;
	///手续费
	TThostFtdcMoneyType	Commission;

	// 平仓盈亏(只有当平仓之后才赋予值)
	double CloseProfit;
	
};



///成交
struct TradeField
{
	///投资者代码
	string	InvestorID;

	///策略ID
	string StrategyID;

	///合约代码
	string	InstrumentID;

	///成交时间
	string	TradeTime;

	///成交编号
	long	TradeID;  //= OrderRef

	///买卖方向
	string	Direction;
	
	///开平标志
	string	OffsetFlag;

	///价格
	TThostFtdcPriceType	Price;
	///数量
	TThostFtdcVolumeType	Volume;
	///金额
	double Amount;
	///保证金
	TThostFtdcMoneyType	Margin;
	///手续费
	TThostFtdcMoneyType	Commission;

	///止损价
	double	StopPrice;

	
};


///投资者持仓明细
struct PositionDetailField
{
	///投资者代码
	string	InvestorID;

	///策略ID
	string StrategyID;

	// 成交时间
	string TradingTime;

	///合约代码
	string	InstrumentID;
	
	///成交编号
	long	OrderRef;

	///买卖
	string	Direction;
	///开仓日期
	string	OpenDate;
		
	///数量
	TThostFtdcVolumeType Volume;

	///成交金额
	TThostFtdcMoneyType	Amount;
	
	///开仓价
	TThostFtdcPriceType	OpenPrice;

	///投资者保证金
	TThostFtdcMoneyType	Margin;

	///止损价
	double	StopPrice;  // 跟踪止损价格

	/// 平仓时间
	string CloseTime; 
	///平仓量
	TThostFtdcVolumeType CloseVolume;
	///平仓金额
	TThostFtdcMoneyType	CloseAmount;

	///平仓盈亏
	double CloseProfit;

	///可平仓量
	int	LeftVolume;

};


#endif