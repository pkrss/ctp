#ifndef _ATS_BACKTESTING_H
#define _ATS_BACKTESTING_H


#include "structs.h"
#include "FStrategy.h"
#include "FAccount.h"
#include "FQuote.h"


/*-----------------------------------------------------------//
 * 读取行情，存入 newQuote
 * 放入 mapQuotes，Key为合约代码，Value为行情vector，每日一清空
 * 指标：AvgDay、Avg10、Avg120、Avg240
 * 账户：Account：初始资金、可用资金、冻结保证金、冻结手续费等
 * 报单：newOrder、vecOrders
 * 成交：newTrade、vecTrades
 * 持仓：vecPositionDetails
 * 权益：组合市值
//-----------------------------------------------------------*/

// ----自定义 类型------------------------------------------------------//
//typedef map<TThostFtdcInstrumentIDType, vector<QuoteField>> MapQuotesType;
typedef map<string, InstrumentField> MapInstrumentType;
typedef map<string,vector<CommissionRateField>> MapCommissionRateType;
typedef map<string, vector<QuoteField>> MapQuotesType;
typedef map<string, vector<IndicatorField>> MapIndicatorsType;


// ---- 自定义变量 -----------------------------------------------------//
string vInvestorID = "Kevin2002";	// 用户ID
string vStrategyID = "R-Breaker-M1";	// 策略ID
double vInitMoney  = 100000;			// 初始化资金

FAccount myAccount(vInvestorID,vInitMoney);
FStrategy myStrategy(vStrategyID);
FQuote myQuotes;


// 交易日
string TradingDate; // yyyy-mm-dd

//// 下一个报单引用编码
//long NextOrderRef ;  

//// 账户信息
//AccountField myAccount;
//
//// 策略信息
//StrategyField_R_Breaker myStrategy;			

QuoteField newQuote;			// 最新行情


//QuoteField newQuote;			// 最新行情
//IndicatorField newIndicator;	// 最新指标
//OrderField newOrder;				// 最新报单

//MapQuotesType mapQuotes;			// 行情map： Key：Symbol，Value：vecQuotes
//MapIndicatorsType mapIndicators;	// 指标map： Key：Symbol, Value: vecIndicators



// ---- 自定义 函数 ---------------------------------------------------//
// 初始化 newQuote 结构向量
void InitNewQuote(QuoteField& newQuote);

// 根据行情vector，对应指标，存入行情向量vecQuotes
void Convert2NewQuote(const vector<string>& dest, QuoteField& newQuote);


// 初始化策略
//void InitStrategy();


 

// 初始化 新的一天 的行情 、指标
//void InitNewDay();


// 提交新报单后，更新账户资金
//void UpdateAccount(const OrderField& insertOrder);



// ---- 自定义 基础操作函数 ------------------------------------------------//

// 将每一行行情数据拆分成不同的变量，存入vector
void split(const string& strSrc, const string& strFlag, vector<string>& vecDest) ;

#endif