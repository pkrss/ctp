#pragma once
#include "structs.h"
#include "FQuote.h"
#include "FAccount.h"


class FStrategy
{
public:
	
	FStrategy(string stragetyId);
	~FStrategy(void);

	//// 更新FQuote类的newInstrument
	//void UpdateNewInstrument(const InstrumentField& newInstrument);


	// 根据最新指标，执行交易策略
	void ExecStrategy(const IndicatorField& newIndicator);
	
	 // 策略：R-Breaker Strategy
	void Strategy_R_Breaker(const IndicatorField& newIndicator);

	// 策略：双均线策略
	void Strategy_Two_MA(const IndicatorField& newIndicator);

	// 策略：OpenPrice Breaker
	void Strategy_OpenPriceBreaker(const IndicatorField& newIndicator);

	// 策略：相同止盈止损幅度，由趋势方向决定胜率
	void Strategy_StopLossProfit(const IndicatorField& newIndicator);


	 // 生成开仓报单
	void CreateOpenOrder(const IndicatorField& newIndicator, string buySell,OrderField& order); 

	// 生成平仓报单
	void CreateCloseOrder(const IndicatorField& newIndicator,const PositionDetailField& positionDetail, OrderField& order); 

	// 生成强平报单
	void CreateForceCloseOrder(const PositionDetailField& positionDetail,OrderField& order) ;
		
	// 将报单插入vector,包括开仓、平仓，不包括撤单
	void InsertVecOrders(const OrderField& order);

	// 报单操作,处理撤单等，不包括开仓、平仓
	void OrderAction(OrderField order,QuoteField& quote);

	// 撮合成交，判断订单是否可以成交
	void MatchOrders(const IndicatorField& newIndicator,const string priceType); // priceType: "NEW"，"CONTERPARTY"
	
	// 撮合成交
	//void MatchOrders(const IndicatorField& newIndicator,OrderField& order,const string buySell,const double price); // priceType: "NEW"，"CONTERPARTY"

	// 撮合成交：开仓单
	void MatchOpenOrders(const IndicatorField& newIndicator,OrderField& order,const double price); // priceType: "NEW"，"CONTERPARTY"
	// 撮合成交：平仓单
	void MatchCloseOrders(const IndicatorField& newIndicator,OrderField& order,const double price); // priceType: "NEW"，"CONTERPARTY"


	// 撤单操作，撮合之后，若订单不成交则马上撤单
	void CancelOrders(const IndicatorField& newIndicator); 
	
	 // 限定时间达到时，强制平仓当天所有持仓，
	void ForceClose();


	// 发出平仓委托之后，更新持仓明细
	void UpdatePositionDetailByOrder(const OrderField& order);

	// 开仓成交之后，生成持仓明细
	void InsertPositionDetail(const OrderField& trade);
	// 平仓成交之后，更新持仓明细
	void UpdatePositionDetailByTrade(const OrderField& trade);

	// 根据最新行情，调整持仓合约的止盈止损价格
	void UpdateStopPrice(const IndicatorField& newIndicator);
	// 根据最新行情，调整持仓合约的止盈止损价格,仅调整一次，目的控制交易损失
	void FStrategy::UpdateStopPriceFirst(const IndicatorField& newIndicator);


	// 根据成交之后的委托单，生成持仓明细
	void CreatePositionDetail(const OrderField& trade, PositionDetailField& positionDetail);
	// 将持仓明细插入vector
	void InsertVecPositionDetails(const PositionDetailField& positionDetail);

	// 获取 某合约 尚未成交的全部委托单数量（含开仓、平仓）
	int GetInstOrderNum(const string instrumentId);
	// 获取 某合约 尚未成交的“开仓”委托单数量
	int GetInstOpenOrderNum(const string instrumentId);
	// 获取 某合约 尚未成交的“平仓”委托单数量
	int GetInstCloseOrderNum(const string instrumentId);


	// 获取 全部合约 尚未成交的“开仓”委托单数量
	int GetOpenOrderNum();

	// 获取 某合约的持仓数量,volumeType: VOLUME、LEFTVOLUME
	int GetInstPositionNum(const string instrumentId,const string volumeType);

	// 获取 某合约的多头持仓数量,volumeType: VOLUME、LEFTVOLUME
	int GetInstLongPositionNum(const string instrumentId,const string volumeType);

	// 获取 某合约的空头持仓数量,volumeType: VOLUME、LEFTVOLUME
	int GetInstShortPositionNum(const string instrumentId,const string volumeType);

	// 获取 全部合约的持仓数量
	int GetPositionNum();

	// 获取 某合约的可平仓数量
	int GetInstLeftPositionNum(const string instrumentId);
	//// 获取 全部合约的可平仓数量
	//int FStrategy::GetLeftPositionNum();

	// 获取某合约的持仓盈亏
	double GetInstPositionProfit(const string instrumentId);
	// 获取全部合约的持仓盈亏
	double GetPositionProfit();

	// 获取 全部合约 当天的 总盈亏 （持仓盈亏+平仓盈亏）
	double GetProfit();

	// 计算平仓盈亏
	double CalcCloseProfit( const OrderField& order);


	// 初始化 合约map
	void InitMapInstruments();

	// 根据最新行情的合约代码，获取最新的合约信息
	void GetNewInstrument(const string instrumentId, InstrumentField& newInstrument);

	// 获取 合约乘数
	int GetInstVolumeMultiple(const string instrumentId);

	// 设置FQuote 类对象
	void SetFQuote(FQuote *pQuote);

	// 设置FAccount 类对象
	void SetFAccount(FAccount *pAccount);

	// 将vecOrders输出至log文件
	void PrintVecOrders();
	// 将vecPositionDetails输出至log文件
	void PrintVecPositionDetails();
	
	// 每日交易及盈亏分析
	void DailyStat(const string TradingDate);


	// 获取系统当前时间的毫秒数
	string GetNowMillisecond(void);

public:

	string StrategyID;
		
	StrategyField Strategy;			// 策略信息

	string DataFile;	// 数据文件来源
	long NextOrderRef;	// 下一个报单引用

	InstrumentField newInstrument;		// 最新行情对应的合约信息
	OrderField newOrder; // 最新报单

	//报单向量，保存当前所有合约的报单
	vector<OrderField> vecOrders;

	//持仓明细向量，保存当前所有合约的持仓明细
	vector<PositionDetailField> vecPositionDetails;

	map<string, InstrumentField> mapInstruments;			// 合约map
	//MapCommissionRateType mapCommissionRates;	// 手续费率map

	string preDirection;   // UP,DOWN
	string newDirection;   // UP,DOWN
	int dayTradingNum;     // 日交易笔数
	int dayTradesNum;

private:

	FQuote*   pFQuotes;   // 指向类的指针
	FAccount* pFAccount; // 指向类的指针

	
};

