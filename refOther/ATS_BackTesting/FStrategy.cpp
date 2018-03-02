#include "FStrategy.h"
#include <Windows.h>
#include <iomanip>

FStrategy::FStrategy(string stragetyId)
{
	StrategyID = stragetyId;

	// 初始化 策略参数
	Strategy.BSQty = 1;						// 每次买卖的合约数量
	Strategy.ModifyRange = 10;				// 跟踪止损，调整的阀值
	Strategy.ModifyRatio = 0.1;				// 跟踪止损，调整的幅度
	Strategy.Rongrendu = 2.5;				// 开仓和平仓时的价差 容忍度
	Strategy.TradeBegTime = "9:00:02";		// 每天策略开始执行的起始时间
	Strategy.ForceCloseTime = "14:59:00";	// 每日强平时间，如14:58:00

	NextOrderRef = 101 ;					// 下一个报单引用
	DataFile = "SCrb0001_201401.csv";

	preDirection = "";   // UP,DOWN
	newDirection = "";   // UP,DOWN

	dayTradesNum = 50;  // 每日交易笔数上限

	// 设置大容量，避免频繁扩容
	vecOrders.reserve(1000); 

}


FStrategy::~FStrategy(void)
{
}

// 初始化 合约map
void FStrategy::InitMapInstruments()
{
	string instrumentId;
	InstrumentField instrumentInfo;


	// ---- 合约1 ----------------------------------------------------------//
	//合约代码 ------------------------------------
	instrumentId = "rb1405";
	
	// 合约信息-----------------------------------
	///合约代码
	instrumentInfo.InstrumentID = "rb1405";
	///合约名称
	instrumentInfo.InstrumentName = "rb1405";
	///交易所代码
	instrumentInfo.ExchangeID = "SHFE";
	///合约数量乘数
	instrumentInfo.VolumeMultiple = 10;
	///最小变动价位
	instrumentInfo.PriceTick = 1;
	
	// 保证金率---------------------
	///多头保证金率
	instrumentInfo.LongMarginRatio = 0.09;
	///空头保证金率
	instrumentInfo.ShortMarginRatio = 0.09;

	// 手续费率---------------------
	///开仓手续费
	instrumentInfo.OpenRatioByVolume = 0;
	///平仓手续费
	instrumentInfo.CloseRatioByVolume = 0;
	///平今手续费
	instrumentInfo.CloseTodayRatioByVolume = 0;

	///开仓手续费率
	instrumentInfo.OpenRatioByMoney = 0.000045 ; //0.000054;
	///平仓手续费率
	instrumentInfo.CloseRatioByMoney = 0.000045 ; //0.000054;
	///平今手续费率
	instrumentInfo.CloseTodayRatioByMoney = 0;

	///当前是否交易
	instrumentInfo.IsTrading = 1;

	
	// 合约信息插入map
	mapInstruments.insert(map<string, InstrumentField>::value_type(instrumentId, instrumentInfo));


	// ---- 合约2 ----------------------------------------------------------//
	//合约代码 ------------------------------------
	instrumentId = "rb1410";
	
	// 合约信息-----------------------------------
	///合约代码
	instrumentInfo.InstrumentID = "rb1410";
	///合约名称
	instrumentInfo.InstrumentName = "rb1410";
	///交易所代码
	instrumentInfo.ExchangeID = "SHFE";
	///合约数量乘数
	instrumentInfo.VolumeMultiple = 10;
	///最小变动价位
	instrumentInfo.PriceTick = 1;
	
	// 保证金率---------------------
	///多头保证金率
	instrumentInfo.LongMarginRatio = 0.09;
	///空头保证金率
	instrumentInfo.ShortMarginRatio = 0.09;

	// 手续费率---------------------
	///开仓手续费
	instrumentInfo.OpenRatioByVolume = 0;
	///平仓手续费
	instrumentInfo.CloseRatioByVolume = 0;
	///平今手续费
	instrumentInfo.CloseTodayRatioByVolume = 0;

	///开仓手续费率
	instrumentInfo.OpenRatioByMoney = 0.000045 ; //0.000054;
	///平仓手续费率
	instrumentInfo.CloseRatioByMoney = 0.000045 ; //0.000054;
	///平今手续费率
	instrumentInfo.CloseTodayRatioByMoney = 0;

	///当前是否交易
	instrumentInfo.IsTrading = 1;



	// 合约信息插入map
	mapInstruments.insert(map<string, InstrumentField>::value_type(instrumentId, instrumentInfo));


	// ---- 合约3 -------------------------------------------------------------------//
	//合约代码 ------------------------------------
	instrumentId = "ru1405";
	
	// 合约信息-----------------------------------
	///合约代码
	instrumentInfo.InstrumentID = "ru1405";
	///合约名称
	instrumentInfo.InstrumentName = "ru1405";
	///交易所代码
	instrumentInfo.ExchangeID = "SHFE";
	///合约数量乘数
	instrumentInfo.VolumeMultiple = 10;
	///最小变动价位
	instrumentInfo.PriceTick = 5;
	
	// 保证金率---------------------
	///多头保证金率
	instrumentInfo.LongMarginRatio = 0.12;
	///空头保证金率
	instrumentInfo.ShortMarginRatio = 0.12;

	// 手续费率---------------------
	///开仓手续费
	instrumentInfo.OpenRatioByVolume = 0;
	///平仓手续费
	instrumentInfo.CloseRatioByVolume = 0;
	///平今手续费
	instrumentInfo.CloseTodayRatioByVolume = 0;

	///开仓手续费率
	instrumentInfo.OpenRatioByMoney  = 0.00005; 
	///平仓手续费率
	instrumentInfo.CloseRatioByMoney = 0.00005;
	///平今手续费率
	instrumentInfo.CloseTodayRatioByMoney = 0;

	///当前是否交易
	instrumentInfo.IsTrading = 1;



	// 合约信息插入map
	mapInstruments.insert(map<string, InstrumentField>::value_type(instrumentId, instrumentInfo));

	// ---- 合约4 -------------------------------------------------------------------//
	//合约代码 ------------------------------------
	instrumentId = "ru1409";
	
	// 合约信息-----------------------------------
	///合约代码
	instrumentInfo.InstrumentID = "ru1409";
	///合约名称
	instrumentInfo.InstrumentName = "ru1409";
	///交易所代码
	instrumentInfo.ExchangeID = "SHFE";
	///合约数量乘数
	instrumentInfo.VolumeMultiple = 10;
	///最小变动价位
	instrumentInfo.PriceTick = 5;
	
	// 保证金率---------------------
	///多头保证金率
	instrumentInfo.LongMarginRatio = 0.12;
	///空头保证金率
	instrumentInfo.ShortMarginRatio = 0.12;

	// 手续费率---------------------
	///开仓手续费
	instrumentInfo.OpenRatioByVolume = 0;
	///平仓手续费
	instrumentInfo.CloseRatioByVolume = 0;
	///平今手续费
	instrumentInfo.CloseTodayRatioByVolume = 0;

	///开仓手续费率
	instrumentInfo.OpenRatioByMoney  = 0.00005; 
	///平仓手续费率
	instrumentInfo.CloseRatioByMoney = 0.00005;
	///平今手续费率
	instrumentInfo.CloseTodayRatioByMoney = 0;

	///当前是否交易
	instrumentInfo.IsTrading = 1;



	// 合约信息插入map
	mapInstruments.insert(map<string, InstrumentField>::value_type(instrumentId, instrumentInfo));


	return;
}



// 设置FQuote 类对象
void FStrategy::SetFQuote(FQuote *pQuote)
{
	pFQuotes = pQuote;
}

// 设置FAccount 类对象
void FStrategy::SetFAccount(FAccount *pAccount)
{
	pFAccount = pAccount;
}

//// 更新FQuote类的newInstrument
//void FStrategy::UpdateNewInstrument(const InstrumentField& newInstrument1)
//{
//	//更新当前最新行情对应的合约信息
//	memset(&newInstrument,0,sizeof(InstrumentField));
//	memcpy(&newInstrument, &newInstrument1,sizeof(InstrumentField));
//
//}


// 根据最新行情，执行交易策略
void FStrategy::ExecStrategy(const IndicatorField& newIndicator)
{

	// R-Breaker Strategy
	//Strategy_R_Breaker(newIndicator);

	//Strategy_Two_MA(newIndicator);

	Strategy_StopLossProfit(newIndicator);


}




// 策略：R-Breaker Strategy
 void FStrategy::Strategy_R_Breaker(const IndicatorField& newIndicator)
 {

	 static OrderField order;
	 
	 // 初始化最新报单
	 //memset(&newOrder,0,sizeof(newOrder));

	 //memset(&pairOrders, 0, sizeof(pairOrders));	


	 // ==== 平仓判断：=====================================================//
	 //---- 多头平仓 ----------
	 if ( (newIndicator.Avg10 <= newIndicator.AvgDay - Strategy.Rongrendu * newInstrument.PriceTick) && newIndicator.AvgDay > 0
		 && ( GetInstLongPositionNum(newIndicator.InstrumentID,"LEFTVOLUME") >= 1 ) // 存在可平仓多头时
		 )   
	 {
		 vector<PositionDetailField>::iterator iter;
		 for(iter = vecPositionDetails.begin(); iter != vecPositionDetails.end(); iter++)
		 {
			 if(iter->InstrumentID == newIndicator.InstrumentID && iter->Direction == "BUY" && iter->LeftVolume >= 1 )
			 {

				 // ----多头：卖出 平仓 ----------
					 // 清空报单
					  memset(&order,0,sizeof(order));
					 // 生成报单
					  CreateCloseOrder(newIndicator,*iter,order ); 
					// 提交报单,包括开仓、平仓
					 InsertVecOrders(order);
				
			 }  // InstrumentID ==

		 } // for



	 }  // 开仓判断：多头
	 
	else   // Avg10 >= AvgDay + Rongrendu
		  // ---- 空头平仓 -----
		 if ( (newIndicator.Avg10 >= newIndicator.AvgDay + Strategy.Rongrendu * newInstrument.PriceTick) && newIndicator.Avg10 > 0
			 && ( GetInstShortPositionNum(newIndicator.InstrumentID,"LEFTVOLUME") >= 1 ) // 存在可平仓空头时
			 )
		 {
			 vector<PositionDetailField>::iterator iter;
			 for(iter = vecPositionDetails.begin(); iter != vecPositionDetails.end(); iter++)
			 {
				 if(iter->InstrumentID == newIndicator.InstrumentID && iter->Direction == "SELL" && iter->LeftVolume >= 1 )
				 {

					 // ----空头：买入 平仓 ----------
						 // 清空报单
						  memset(&order,0,sizeof(order));
						 // 生成报单
						  CreateCloseOrder(newIndicator,*iter,order); 
						// 提交报单,包括开仓、平仓
						 InsertVecOrders(order);
				
				 }  // InstrumentID ==

			 } // for

		 }  // 开仓判断：空头


	 // ==== 成交判断：=====================================================//
	 if ( GetInstOrderNum(newIndicator.InstrumentID) >= 1 )
   	 {
		 // 撮合成交，判断订单是否可以成交
		 MatchOrders(newIndicator,"CONTERPARTY"); // priceType:最新价 "NEW"，对手价"CONTERPARTY"

	 }

	 // ==== 撤单判断：=====================================================//
	 if ( GetInstOrderNum(newIndicator.InstrumentID) >= 1 )
   	 {
		 // 撤单操作，撮合之后，若订单不成交则马上撤单
		 CancelOrders(newIndicator); 

	 }


	 // ==== 止损判断：=====================================================//
	 if( GetInstPositionNum(newIndicator.InstrumentID,"LEFTVOLUME") >= 1 )
	 {
		 vector<PositionDetailField>::iterator iter;
		 for(iter = vecPositionDetails.begin(); iter != vecPositionDetails.end(); iter++)
		 {
			 if(iter->InstrumentID == newIndicator.InstrumentID && iter->LeftVolume >= 1)
			 {
				 if(iter->Direction == "BUY")
				 {
					 if(newIndicator.LastPrice <= (iter->StopPrice - Strategy.Rongrendu * newInstrument.PriceTick) )
					 {
						 // ----多头：卖出 平仓 ----------

						 // 清空报单
						  memset(&order,0,sizeof(order));
						 // 生成报单
						  CreateCloseOrder(newIndicator,*iter,order); 
						// 提交报单,包括开仓、平仓
						 InsertVecOrders(order);

					 } // price
				 }    // BUY
				 else // "SELL"
				 {

					 if(newIndicator.LastPrice >= (iter->StopPrice + Strategy.Rongrendu * newInstrument.PriceTick) )
					 {
						 // ----空头：买入 平仓 ----------

						 // 清空报单
						  memset(&order,0,sizeof(order));
						 // 生成报单
						 CreateCloseOrder(newIndicator,*iter,order); 
						// 提交报单,包括开仓、平仓
						 InsertVecOrders(order);

					 } // price

				 }  // "SELL"

			 }  // InstrumentID ==

		 } // for

	 }  // 止损
	 

	 // ==== 动态止损调整 =================================================//
	 if( GetInstPositionNum(newIndicator.InstrumentID,"VOLUME") >= 1 )
	 {
		 UpdateStopPrice(newIndicator);

	 }

	 // ==== 强制平仓 =======================================================//
	 if(newIndicator.TradingTime.substr(11,8) > Strategy.ForceCloseTime)
	 {
		 // 限定时间达到时，强制平仓当天所有持仓，
		 ForceClose();

	 }

	 string strX = newIndicator.TradingTime.substr(11,8);

	 // ==== 开仓判断：=====================================================//
	 //---- 多头 ----------
	 if (newIndicator.Avg10 >= (newIndicator.AvgDay + Strategy.Rongrendu * newInstrument.PriceTick) && newIndicator.AvgDay > 0 )   
	 {
		 // 策略方向
		 newDirection = "UP";

		 if( ( GetInstOpenOrderNum(newIndicator.InstrumentID) + GetInstPositionNum(newIndicator.InstrumentID,"VOLUME") < 1 ) // 委托单、持仓量条件
		       && newIndicator.TradingTime.substr(11,8) <= Strategy.ForceCloseTime
		       && newDirection != preDirection	&& dayTradingNum < dayTradesNum )
		 {
		    // 生成报单
			 CreateOpenOrder(newIndicator,"BUY",order);
			
			 // 提交报单
			 InsertVecOrders(order);

			 // 保存策略方向
			 preDirection = newDirection;

			 // 当日交易笔数 + 1
			 dayTradingNum += 1;


		 } // 持仓判断


	 }  // 开仓判断：多头

	
	else   // <=
		  // ---- 空头 -----
		 if (newIndicator.Avg10 > 0 && newIndicator.Avg10 <= (newIndicator.AvgDay - Strategy.Rongrendu * newInstrument.PriceTick) )
		 {
			 // 策略方向
			 newDirection = "DOWN";

			 if( ( GetInstOpenOrderNum(newIndicator.InstrumentID) + GetInstPositionNum(newIndicator.InstrumentID,"VOLUME") < 1 ) // 委托单、持仓量条件
				&& newIndicator.TradingTime.substr(11,8) <= Strategy.ForceCloseTime 
				&& newDirection != preDirection  && dayTradingNum < dayTradesNum )
			 {
				 // 生成报单
				CreateOpenOrder(newIndicator,"SELL",order);

				 // 提交报单
				 InsertVecOrders(order);

				 // 保存策略方向
				preDirection = newDirection;

				// 当日交易笔数 + 1
			    dayTradingNum += 1;


			 } // 持仓判断

		 }  // 开仓判断：空头



	 return;

 }

 // 策略：双均线策略
 void FStrategy::Strategy_Two_MA(const IndicatorField& newIndicator)
 {

	 static OrderField order;
	 
	 // 初始化最新报单
	 //memset(&newOrder,0,sizeof(newOrder));

	 //memset(&pairOrders, 0, sizeof(pairOrders));	


	 // ==== 平仓判断：=====================================================//
	 //---- 多头平仓 ----------
	 if ( (newIndicator.Avg120 <= newIndicator.Avg240 - Strategy.Rongrendu * newInstrument.PriceTick) && newIndicator.Avg240 > 0
		 && ( GetInstLongPositionNum(newIndicator.InstrumentID,"LEFTVOLUME") >= 1 ) // 存在可平仓多头时
		 )   
	 {
		 vector<PositionDetailField>::iterator iter;
		 for(iter = vecPositionDetails.begin(); iter != vecPositionDetails.end(); iter++)
		 {
			 if(iter->InstrumentID == newIndicator.InstrumentID && iter->Direction == "BUY" && iter->LeftVolume >= 1 )
			 {

				 // ----多头：卖出 平仓 ----------
					 // 清空报单
					  memset(&order,0,sizeof(order));
					 // 生成报单
					  CreateCloseOrder(newIndicator,*iter,order ); 
					// 提交报单,包括开仓、平仓
					 InsertVecOrders(order);
				
			 }  // InstrumentID ==

		 } // for



	 }  // 开仓判断：多头
	 
	else   // Avg10 >= AvgDay + Rongrendu
		  // ---- 空头平仓 -----
		 if ( (newIndicator.Avg120 >= newIndicator.Avg240 + Strategy.Rongrendu * newInstrument.PriceTick) && newIndicator.Avg240 > 0
			 && ( GetInstShortPositionNum(newIndicator.InstrumentID,"LEFTVOLUME") >= 1 ) // 存在可平仓空头时
			 )
		 {
			 vector<PositionDetailField>::iterator iter;
			 for(iter = vecPositionDetails.begin(); iter != vecPositionDetails.end(); iter++)
			 {
				 if(iter->InstrumentID == newIndicator.InstrumentID && iter->Direction == "SELL" && iter->LeftVolume >= 1 )
				 {

					 // ----空头：买入 平仓 ----------
						 // 清空报单
						  memset(&order,0,sizeof(order));
						 // 生成报单
						  CreateCloseOrder(newIndicator,*iter,order); 
						// 提交报单,包括开仓、平仓
						 InsertVecOrders(order);
				
				 }  // InstrumentID ==

			 } // for

		 }  // 开仓判断：空头


	 // ==== 成交判断：=====================================================//
	 if ( GetInstOrderNum(newIndicator.InstrumentID) >= 1 )
   	 {
		 // 撮合成交，判断订单是否可以成交
		 MatchOrders(newIndicator,"CONTERPARTY"); // priceType:最新价 "NEW"，对手价"CONTERPARTY"

	 }

	 // ==== 撤单判断：=====================================================//
	 if ( GetInstOrderNum(newIndicator.InstrumentID) >= 1 )
   	 {
		 // 撤单操作，撮合之后，若订单不成交则马上撤单
		 CancelOrders(newIndicator); 

	 }


	 // ==== 止损判断：=====================================================//
	 if( GetInstPositionNum(newIndicator.InstrumentID,"LEFTVOLUME") >= 1 )
	 {
		 vector<PositionDetailField>::iterator iter;
		 for(iter = vecPositionDetails.begin(); iter != vecPositionDetails.end(); iter++)
		 {
			 if(iter->InstrumentID == newIndicator.InstrumentID && iter->LeftVolume >= 1)
			 {
				 if(iter->Direction == "BUY")
				 {
					 if(newIndicator.LastPrice <= (iter->StopPrice - Strategy.Rongrendu * newInstrument.PriceTick) )
					 {
						 // ----多头：卖出 平仓 ----------

						 // 清空报单
						  memset(&order,0,sizeof(order));
						 // 生成报单
						  CreateCloseOrder(newIndicator,*iter,order); 
						// 提交报单,包括开仓、平仓
						 InsertVecOrders(order);

					 } // price
				 }    // BUY
				 else // "SELL"
				 {

					 if(newIndicator.LastPrice >= (iter->StopPrice + Strategy.Rongrendu * newInstrument.PriceTick) )
					 {
						 // ----空头：买入 平仓 ----------

						 // 清空报单
						  memset(&order,0,sizeof(order));
						 // 生成报单
						 CreateCloseOrder(newIndicator,*iter,order); 
						// 提交报单,包括开仓、平仓
						 InsertVecOrders(order);

					 } // price

				 }  // "SELL"

			 }  // InstrumentID ==

		 } // for

	 }  // 止损
	 

	 // ==== 动态止损调整 =================================================//
	 if( GetInstPositionNum(newIndicator.InstrumentID,"VOLUME") >= 1 )
	 {
		 UpdateStopPrice(newIndicator);

	 }

	 // ==== 强制平仓 =======================================================//
	 if(newIndicator.TradingTime.substr(11,8) > Strategy.ForceCloseTime)
	 {
		 // 限定时间达到时，强制平仓当天所有持仓，
		 ForceClose();

	 }

	 string strX = newIndicator.TradingTime.substr(11,8);

	 // ==== 开仓判断：=====================================================//
	 //---- 多头 ----------
	 if (newIndicator.Avg240 > 0 && newIndicator.Avg120 >= (newIndicator.Avg240 + Strategy.Rongrendu * newInstrument.PriceTick)  )   
	 {
		 // 策略方向
		 newDirection = "UP";

		 if( ( GetInstOpenOrderNum(newIndicator.InstrumentID) + GetInstPositionNum(newIndicator.InstrumentID,"VOLUME") < 1 ) // 委托单、持仓量条件
		       && newIndicator.TradingTime.substr(11,8) <= Strategy.ForceCloseTime
		       && newDirection != preDirection	&& dayTradingNum < dayTradesNum )
		 {
		    // 生成报单
			 CreateOpenOrder(newIndicator,"BUY",order);
			
			 // 提交报单
			 InsertVecOrders(order);

			 // 保存策略方向
			 preDirection = newDirection;

			 // 当日交易笔数 + 1
			 dayTradingNum += 1;

		 } // 持仓判断


	 }  // 开仓判断：多头

	
	else   // <=
		  // ---- 空头 -----
		 if (newIndicator.Avg240 > 0 && newIndicator.Avg120 <= (newIndicator.Avg240 - Strategy.Rongrendu * newInstrument.PriceTick) )
		 {
			 // 策略方向
			 newDirection = "DOWN";

			 if( ( GetInstOpenOrderNum(newIndicator.InstrumentID) + GetInstPositionNum(newIndicator.InstrumentID,"VOLUME") < 1 ) // 委托单、持仓量条件
				&& newIndicator.TradingTime.substr(11,8) <= Strategy.ForceCloseTime 
				&& newDirection != preDirection && dayTradingNum < dayTradesNum  )
			 {
				 // 生成报单
				CreateOpenOrder(newIndicator,"SELL",order);

				 // 提交报单
				 InsertVecOrders(order);

				 // 保存策略方向
				preDirection = newDirection;

				// 当日交易笔数 + 1
			   dayTradingNum += 1;

			 } // 持仓判断

		 }  // 开仓判断：空头



	 return;

 }



 // 策略：OpenPrice Breaker
void FStrategy::Strategy_OpenPriceBreaker(const IndicatorField& newIndicator)
 {

	 static OrderField order;
	 
	 // 初始化最新报单
	 //memset(&newOrder,0,sizeof(newOrder));

	 //memset(&pairOrders, 0, sizeof(pairOrders));	


	 // ==== 平仓判断：=====================================================//
	 //---- 多头平仓 ----------
	 if ( (newIndicator.LastPrice <= pFQuotes->dayOpenPrice - Strategy.Rongrendu * newInstrument.PriceTick) 
		 && ( GetInstLongPositionNum(newIndicator.InstrumentID,"LEFTVOLUME") >= 1 ) // 存在可平仓多头时
		 )   
	 {
		 vector<PositionDetailField>::iterator iter;
		 for(iter = vecPositionDetails.begin(); iter != vecPositionDetails.end(); iter++)
		 {
			 if(iter->InstrumentID == newIndicator.InstrumentID && iter->Direction == "BUY" && iter->LeftVolume >= 1 )
			 {

				 // ----多头：卖出 平仓 ----------
					 // 清空报单
					  memset(&order,0,sizeof(order));
					 // 生成报单
					  CreateCloseOrder(newIndicator,*iter,order ); 
					// 提交报单,包括开仓、平仓
					 InsertVecOrders(order);
				
			 }  // InstrumentID ==

		 } // for



	 }  // 开仓判断：多头
	 
	else   // Avg10 >= AvgDay + Rongrendu
		  // ---- 空头平仓 -----
		  if ( (newIndicator.LastPrice >= pFQuotes->dayOpenPrice + Strategy.Rongrendu * newInstrument.PriceTick)
			 && ( GetInstShortPositionNum(newIndicator.InstrumentID,"LEFTVOLUME") >= 1 ) // 存在可平仓空头时
			 )
		 {
			 vector<PositionDetailField>::iterator iter;
			 for(iter = vecPositionDetails.begin(); iter != vecPositionDetails.end(); iter++)
			 {
				 if(iter->InstrumentID == newIndicator.InstrumentID && iter->Direction == "SELL" && iter->LeftVolume >= 1 )
				 {

					 // ----空头：买入 平仓 ----------
						 // 清空报单
						  memset(&order,0,sizeof(order));
						 // 生成报单
						  CreateCloseOrder(newIndicator,*iter,order); 
						// 提交报单,包括开仓、平仓
						 InsertVecOrders(order);
				
				 }  // InstrumentID ==

			 } // for

		 }  // 开仓判断：空头


	 // ==== 成交判断：=====================================================//
	 if ( GetInstOrderNum(newIndicator.InstrumentID) >= 1 )
   	 {
		 // 撮合成交，判断订单是否可以成交
		 MatchOrders(newIndicator,"CONTERPARTY"); // priceType:最新价 "NEW"，对手价"CONTERPARTY"

	 }

	 // ==== 撤单判断：=====================================================//
	 if ( GetInstOrderNum(newIndicator.InstrumentID) >= 1 )
   	 {
		 // 撤单操作，撮合之后，若订单不成交则马上撤单
		 CancelOrders(newIndicator); 

	 }


	  //==== 止损判断：=====================================================//
	 //if( GetInstPositionNum(newIndicator.InstrumentID,"LEFTVOLUME") >= 1 )
	 //{
		//vector<PositionDetailField>::iterator iter;
		// for(iter = vecPositionDetails.begin(); iter != vecPositionDetails.end(); iter++)
		// {
		//	 if(iter->InstrumentID == newIndicator.InstrumentID && iter->LeftVolume >= 1)
		//	 {
		//		 if(iter->Direction == "BUY")
		//		 {
		//			 if(newIndicator.LastPrice <= (iter->StopPrice - Strategy.Rongrendu * newInstrument.PriceTick) )
		//			 {
		//				 // ----多头：卖出 平仓 ----------

		//				 // 清空报单
		//				  memset(&order,0,sizeof(order));
		//				 // 生成报单
		//				  CreateCloseOrder(newIndicator,*iter,order); 
		//				// 提交报单,包括开仓、平仓
		//				 InsertVecOrders(order);

		//			 } // price
		//		 }    // BUY
		//		 else // "SELL"
		//		 {

		//			 if(newIndicator.LastPrice >= (iter->StopPrice + Strategy.Rongrendu * newInstrument.PriceTick) )
		//			 {
		//				 // ----空头：买入 平仓 ----------

		//				 // 清空报单
		//				  memset(&order,0,sizeof(order));
		//				 // 生成报单
		//				 CreateCloseOrder(newIndicator,*iter,order); 
		//				// 提交报单,包括开仓、平仓
		//				 InsertVecOrders(order);

		//			 } // price

		//		 }  // "SELL"

		//	 }  // InstrumentID ==

		// } // for

	 //}  // 止损
	 

	  //==== 动态止损调整 =================================================//
	 //if( GetInstPositionNum(newIndicator.InstrumentID,"VOLUME") >= 1 )
	 //{
		// //UpdateStopPrice(newIndicator);
		// UpdateStopPriceFirst(newIndicator);
		// 

	 //}

	 // ==== 强制平仓 =======================================================//
	 if(newIndicator.TradingTime.substr(11,8) > Strategy.ForceCloseTime
		 && GetInstPositionNum(newIndicator.InstrumentID,"LEFTVOLUME") >= 1 )
	 {
		 // 限定时间达到时，强制平仓当天所有持仓，
		 ForceClose();

	 }


	 // ==== 开仓判断：=====================================================//
	 //---- 多头 ----------
	 if (pFQuotes->dayOpenPrice > 0 && newIndicator.LastPrice >= (pFQuotes->dayOpenPrice + Strategy.Rongrendu * newInstrument.PriceTick)  )   
	 {
		 // 策略方向
		 newDirection = "UP";

		 if( ( GetInstOpenOrderNum(newIndicator.InstrumentID) + GetInstPositionNum(newIndicator.InstrumentID,"VOLUME") < 1 ) // 委托单、持仓量条件
		       && newIndicator.TradingTime.substr(11,8) <= Strategy.ForceCloseTime
			   && abs(newIndicator.LastPrice - pFQuotes->dayOpenPrice) <= 2 * Strategy.Rongrendu * newInstrument.PriceTick
		       && newDirection != preDirection	&& dayTradingNum < dayTradesNum )
		 {
		    // 生成报单
			 CreateOpenOrder(newIndicator,"BUY",order);
			
			 // 提交报单
			 InsertVecOrders(order);

			 // 保存策略方向
			 preDirection = newDirection;

			 // 当日交易笔数 + 1
			 dayTradingNum += 1;


		 } // 持仓判断


	 }  // 开仓判断：多头

	
	else   // <=
		  // ---- 空头 -----
		  if (pFQuotes->dayOpenPrice > 0 && newIndicator.LastPrice <= (pFQuotes->dayOpenPrice - Strategy.Rongrendu * newInstrument.PriceTick) )
		 {
			 // 策略方向
			 newDirection = "DOWN";

			 if( ( GetInstOpenOrderNum(newIndicator.InstrumentID) + GetInstPositionNum(newIndicator.InstrumentID,"VOLUME") < 1 ) // 委托单、持仓量条件
				&& newIndicator.TradingTime.substr(11,8) <= Strategy.ForceCloseTime 
				&& abs(newIndicator.LastPrice - pFQuotes->dayOpenPrice) <= 2 * Strategy.Rongrendu * newInstrument.PriceTick
				&& newDirection != preDirection && dayTradingNum < dayTradesNum  )
			 {
				 // 生成报单
				CreateOpenOrder(newIndicator,"SELL",order);

				 // 提交报单
				 InsertVecOrders(order);

				 // 保存策略方向
				preDirection = newDirection;

				// 当日交易笔数 + 1
			    dayTradingNum += 1;

			 } // 持仓判断

		 }  // 开仓判断：空头



	 return;

 }




  // 策略：Stop Loss and Profit of same range
void FStrategy::Strategy_StopLossProfit(const IndicatorField& newIndicator)
 {

	 static OrderField order;

	 static IndicatorField newIndicator_1;
	 
	 
	 // 获取T-1的指标
	 memset(&newIndicator_1,0,sizeof(IndicatorField));
	 pFQuotes->GetInstNewIndicator(newIndicator.InstrumentID,newIndicator_1,1);  // 获取T-1个最新指标

	 if ( newIndicator_1.Avg10 <=0 || newIndicator_1.Avg10 >= 100000 )
		 return;

	 // 初始化最新报单
	 //memset(&newOrder,0,sizeof(newOrder));

	 //memset(&pairOrders, 0, sizeof(pairOrders));	


	 // ==== 平仓判断：=====================================================//
	 //---- 多头平仓 ----------
	 if ( GetInstLongPositionNum(newIndicator.InstrumentID,"LEFTVOLUME") >= 1  // 存在可平仓多头时
		  && newIndicator.Avg10 > 0 && newIndicator.Avg10 < 100000
		  && newIndicator.Avg120 > 0 && newIndicator.Avg120 < 100000
		  && newIndicator_1.Avg10 > 0 && newIndicator_1.Avg10 < 100000
		  && newIndicator.Avg10 < newIndicator.Avg120  // 平仓条件
		  && newIndicator_1.Avg10 > newIndicator.Avg120 
		 )   
	 {
		 vector<PositionDetailField>::iterator iter;
		 for(iter = vecPositionDetails.begin(); iter != vecPositionDetails.end(); iter++)
		 {
			 if(iter->InstrumentID == newIndicator.InstrumentID && iter->Direction == "BUY" && iter->LeftVolume >= 1 )
			 {

				 // ----多头：卖出 平仓 ----------
					 // 清空报单
					  memset(&order,0,sizeof(order));
					 // 生成报单
					  CreateCloseOrder(newIndicator,*iter,order ); 
					// 提交报单,包括开仓、平仓
					 InsertVecOrders(order);
				
			 }  // InstrumentID ==

		 } // for



	 }  // 平仓判断：多头
	 
	else   // Avg10 >= AvgDay + Rongrendu
		  // ---- 空头平仓 -----
		  if ( ( GetInstShortPositionNum(newIndicator.InstrumentID,"LEFTVOLUME") >= 1 ) // 存在可平仓空头时
			  && newIndicator.Avg10 > 0 && newIndicator.Avg10 < 100000
			  && newIndicator.Avg120 > 0 && newIndicator.Avg120 < 100000
			  && newIndicator_1.Avg10 > 0 && newIndicator_1.Avg10 < 100000
			  && newIndicator.Avg10 > newIndicator.Avg120 // 平仓条件
			  && newIndicator_1.Avg10 < newIndicator.Avg120 

			 )
		 {
			 vector<PositionDetailField>::iterator iter;
			 for(iter = vecPositionDetails.begin(); iter != vecPositionDetails.end(); iter++)
			 {
				 if(iter->InstrumentID == newIndicator.InstrumentID && iter->Direction == "SELL" && iter->LeftVolume >= 1 )
				 {

					 // ----空头：买入 平仓 ----------
						 // 清空报单
						  memset(&order,0,sizeof(order));
						 // 生成报单
						  CreateCloseOrder(newIndicator,*iter,order); 
						// 提交报单,包括开仓、平仓
						 InsertVecOrders(order);
				
				 }  // InstrumentID ==

			 } // for

		 }  // 平仓判断：空头


	 // ==== 成交判断：=====================================================//
	 if ( GetInstOrderNum(newIndicator.InstrumentID) >= 1 )
   	 {
		 // 撮合成交，判断订单是否可以成交
		 MatchOrders(newIndicator,"CONTERPARTY"); // priceType:最新价 "NEW"，对手价"CONTERPARTY"

	 }

	 // ==== 撤单判断：=====================================================//
	 if ( GetInstOrderNum(newIndicator.InstrumentID) >= 1 )
   	 {
		 // 撤单操作，撮合之后，若订单不成交则马上撤单
		 CancelOrders(newIndicator); 

	 }


	  //==== 止损判断：=====================================================//
	 if( GetInstPositionNum(newIndicator.InstrumentID,"LEFTVOLUME") >= 1 )
	 {
		vector<PositionDetailField>::iterator iter;
		 for(iter = vecPositionDetails.begin(); iter != vecPositionDetails.end(); iter++)
		 {
			 if(iter->InstrumentID == newIndicator.InstrumentID && iter->LeftVolume >= 1)
			 {
				 if(iter->Direction == "BUY") // 多头
				 {
					 if(    newIndicator.LastPrice <= (iter->StopPrice - Strategy.Rongrendu * newInstrument.PriceTick)  // 止损
						 //|| newIndicator.LastPrice >= (iter->StopPrice + Strategy.Rongrendu * 2 * newInstrument.PriceTick)  // 止盈
						 || newIndicator.BidPrice1 >= (iter->StopPrice + newInstrument.PriceTick)  // 止盈
						)
					 {
						 // ----多头：卖出 平仓 ----------

						 // 清空报单
						  memset(&order,0,sizeof(order));
						 // 生成报单
						  CreateCloseOrder(newIndicator,*iter,order); 
						// 提交报单,包括开仓、平仓
						 InsertVecOrders(order);

					 } // price
				 }    // BUY
				 else // "SELL"
				 {

					 if(    newIndicator.LastPrice >= (iter->StopPrice + Strategy.Rongrendu * newInstrument.PriceTick) // 止损
						 //|| newIndicator.LastPrice <= (iter->StopPrice - Strategy.Rongrendu * 2 * newInstrument.PriceTick) // 止盈
						 || newIndicator.AskPrice1 <= (iter->StopPrice -  newInstrument.PriceTick) // 止盈
					    )
					 {
						 // ----空头：买入 平仓 ----------

						 // 清空报单
						  memset(&order,0,sizeof(order));
						 // 生成报单
						 CreateCloseOrder(newIndicator,*iter,order); 
						// 提交报单,包括开仓、平仓
						 InsertVecOrders(order);

					 } // price

				 }  // "SELL"

			 }  // InstrumentID ==

		 } // for

	 }  // 止损
	 

	  //==== 动态止损调整 =================================================//
	 if( GetInstPositionNum(newIndicator.InstrumentID,"VOLUME") >= 1 )
	 {
		 //UpdateStopPrice(newIndicator);
		 UpdateStopPriceFirst(newIndicator);
		 

	 }

	 // ==== 强制平仓 =======================================================//
	 if(newIndicator.TradingTime.substr(11,8) > Strategy.ForceCloseTime
		 && GetInstPositionNum(newIndicator.InstrumentID,"LEFTVOLUME") >= 1 )
	 {
		 // 限定时间达到时，强制平仓当天所有持仓，
		 ForceClose();

	 }


	 // ==== 开仓判断：=====================================================//

	 // 策略1、双均线+止盈止损--------------------------------------
	 //     开仓：
	 //         avg10上穿avg120，开多仓；
	 //         avg10下穿avg120，开空仓；
	 //    平仓：
	 //         avg10上穿avg120，平空仓；
	 //         avg10下穿avg120，开多仓；
	 //    止盈止损：
	 //         亏损2个最小单位，止损；
	 //         盈利10个最小单位，止盈;
	 //    调整成本价：
	 //         最新价格优于成本价格6个最小单位，则成本价上调3个单位


	 //---- 多头 ----------
	 if (    pFQuotes->dayOpenPrice > 0 
		  && newIndicator.Avg10 > 0 && newIndicator.Avg10 < 100000
		  && newIndicator.Avg120 > 0 && newIndicator.Avg120 < 100000
		  && newIndicator_1.Avg10 > 0 && newIndicator_1.Avg10 < 100000
		  && newIndicator.Avg10 > newIndicator.Avg120 
		  && newIndicator_1.Avg10 < newIndicator.Avg120 
		  
		  //&&  newIndicator.Avg120 > newIndicator.Avg240
		  //&& newIndicator.LastPrice >= (pFQuotes->dayOpenPrice + Strategy.Rongrendu * newInstrument.PriceTick)  
		)   
	 {
		 // 策略方向
		 newDirection = "UP";

		 if( ( GetInstOpenOrderNum(newIndicator.InstrumentID) + GetInstPositionNum(newIndicator.InstrumentID,"VOLUME") < 1 ) // 委托单、持仓量条件
		       && newIndicator.TradingTime.substr(11,8) <= Strategy.ForceCloseTime
			   //&& abs(newIndicator.LastPrice - pFQuotes->dayOpenPrice) <= 2 * Strategy.Rongrendu * newInstrument.PriceTick  // 在dayOpenPrice价格附近
		       //&& newDirection != preDirection	
			   && dayTradingNum < dayTradesNum 
			   //&& newIndicator.LastPrice >= newIndicator.Max120  // 最新价格是最高价格
			)
		 {
		    // 生成报单
			 CreateOpenOrder(newIndicator,"BUY",order);
			
			 // 提交报单
			 InsertVecOrders(order);

			 // 保存策略方向
			 preDirection = newDirection;

			 // 当日交易笔数 + 1
			 dayTradingNum += 1;


		 } // 持仓判断


	 }  // 开仓判断：多头

	
	else   // <=
		  // ---- 空头 -----
		  if (   pFQuotes->dayOpenPrice > 0 
			   && newIndicator.Avg10 > 0 && newIndicator.Avg10 < 100000
			   && newIndicator.Avg120 > 0 && newIndicator.Avg120 < 100000
			   && newIndicator_1.Avg10 > 0 && newIndicator_1.Avg10 < 100000
			   && newIndicator.Avg10 < newIndicator.Avg120 
			   && newIndicator_1.Avg10 > newIndicator.Avg120 
			   //&& newIndicator.LastPrice <= (pFQuotes->dayOpenPrice - Strategy.Rongrendu * newInstrument.PriceTick)
			  )
		 {
			 // 策略方向
			 newDirection = "DOWN";

			 if( ( GetInstOpenOrderNum(newIndicator.InstrumentID) + GetInstPositionNum(newIndicator.InstrumentID,"VOLUME") < 1 ) // 委托单、持仓量条件
				&& newIndicator.TradingTime.substr(11,8) <= Strategy.ForceCloseTime 
				//&& abs(newIndicator.LastPrice - pFQuotes->dayOpenPrice) <= 2 * Strategy.Rongrendu * newInstrument.PriceTick  // 在dayOpenPrice价格附近
				//&& newDirection != preDirection 
				&& dayTradingNum < dayTradesNum  
				//&& newIndicator.LastPrice <= newIndicator.Min120 // 最新价格是最低价格
			   )
			 {
				 // 生成报单
				CreateOpenOrder(newIndicator,"SELL",order);


				 // 提交报单
				 InsertVecOrders(order);

				 // 保存策略方向
				preDirection = newDirection;

				// 当日交易笔数 + 1
			    dayTradingNum += 1;

			 } // 持仓判断

		 }  // 开仓判断：空头



	 return;

 }




 // 生成开仓报单
void FStrategy::CreateOpenOrder(const IndicatorField& newIndicator, string buySell,OrderField& order)  
{
	
	// 构造 报单 ----------------------------------------
	///投资者代码
	order.InvestorID = pFAccount->InvestorID;

	///策略ID
	order.StrategyID = StrategyID;

	///合约代码
	order.InstrumentID =newIndicator.InstrumentID;

	///委托时间
	order.InsertTime =newIndicator.TradingTime;  // yyyy-mm-dd hh:mm:ss.000

	///报单引用
	order.OrderRef = NextOrderRef++;

	///平仓ID
	order.OpenOrderRef = 0; 

	///组合开平标志
	order.CombOffsetFlag = "OPEN";

	///买卖方向
	order.Direction = buySell;
	
	///报单价格 - 根据对手价格买卖
	if(buySell == "BUY")
		order.OrderPrice =newIndicator.AskPrice1 + newInstrument.PriceTick;  
	else
		order.OrderPrice =newIndicator.BidPrice1 - newInstrument.PriceTick;


	///报单数量
	order.OrderVolume = Strategy.BSQty;   // 购买 【默认数量】
	///报单金额
	order.OrderAmount = order.OrderVolume * order.OrderPrice * newInstrument.VolumeMultiple;

	///冻结的保证金
	if(buySell == "BUY")
		order.FrozenMargin = order.OrderAmount * newInstrument.LongMarginRatio;
	else
		order.FrozenMargin = order.OrderAmount * newInstrument.ShortMarginRatio;

	///冻结的手续费
	if (newInstrument.OpenRatioByMoney > 0)
		order.FrozenCommission = order.OrderAmount * newInstrument.OpenRatioByMoney;
	else if (newInstrument.OpenRatioByVolume > 0)
		order.FrozenCommission = order.OrderVolume * newInstrument.OpenRatioByVolume;

	///止损价
	order.StopPrice = order.OrderPrice;
	
	///报单状态
	order.OrderStatus = 1;  // 1-未成交，0-已成交，-1-已撤单
	
	///状态信息
	order.StatusMsg = "未成交";

	///撤销时间
	order.CancelTime ="";

	///成交时间
	order.UpdateTime ="";
	
	///成交价格
	order.Price =0;
	///成交数量
	order.Volume =0;
	///成交金额
	order.Amount =0;

	///保证金
	order.Margin =0;
	///手续费
	order.Commission =0;

	// 平仓盈亏(只有当平仓之后才赋予值)
	order.CloseProfit = 0;

	return;

}

// 生成平仓报单
void FStrategy::CreateCloseOrder(const IndicatorField& newIndicator,const PositionDetailField& positionDetail, OrderField& order )  
{
	
	// 构造 报单 ----------------------------------------
	///投资者代码
	order.InvestorID = pFAccount->InvestorID;

	///策略ID
	order.StrategyID = StrategyID;

	///合约代码
	order.InstrumentID =newIndicator.InstrumentID;

	///委托时间
	order.InsertTime =newIndicator.TradingTime;  // yyyy-mm-dd hh:mm:ss.000

	///报单引用
	order.OrderRef = NextOrderRef++;

	///平仓ID
	order.OpenOrderRef = positionDetail.OrderRef; 

	///组合开平标志
	order.CombOffsetFlag = "CLOSE";

	///买卖方向
	order.Direction = (positionDetail.Direction == "BUY")?"SELL":"BUY";
	
	///报单价格 - 根据对手价格买卖
	if(order.Direction == "BUY")
		order.OrderPrice =newIndicator.AskPrice1;  
	else
		order.OrderPrice =newIndicator.BidPrice1;


	///报单数量
	order.OrderVolume = positionDetail.LeftVolume;   // 【平仓剩余持仓】
	///报单金额
	order.OrderAmount = order.OrderVolume * order.OrderPrice * newInstrument.VolumeMultiple;

	///冻结的保证金,平仓单 保证金 = 0
	order.FrozenMargin = 0;
	
	///冻结的手续费
	if (newInstrument.CloseRatioByMoney > 0)
		order.FrozenCommission = order.OrderAmount * newInstrument.CloseRatioByMoney;
	else if (newInstrument.CloseRatioByVolume > 0)
		order.FrozenCommission = order.OrderVolume * newInstrument.CloseRatioByVolume;
	
	// 如果是今日开仓， 且两种平今手续费率都是0，则平仓手续费为0。
	if(newInstrument.CloseTodayRatioByMoney == 0 && newInstrument.CloseTodayRatioByVolume == 0
		&& positionDetail.OpenDate == newIndicator.TradingTime.substr(0,10)
	  )
	{
		order.FrozenCommission = 0;
	}

	///止损价
	order.StopPrice = 0;
	
	///报单状态
	order.OrderStatus = 1;  // 1-未成交，0-已成交，-1-已撤单
	
	///状态信息
	order.StatusMsg = "未成交";

	///撤销时间
	order.CancelTime ="";

	///成交时间
	order.UpdateTime ="";
	
	///成交价格
	order.Price =0;
	///成交数量
	order.Volume =0;
	///成交金额
	order.Amount =0;

	///保证金
	order.Margin = positionDetail.Margin; // 设置为 持仓的保证金，用于平仓之后解除保证金
	///手续费
	order.Commission =0;

	// 平仓盈亏(只有当平仓之后才赋予值)
	order.CloseProfit = 0;

	return;

}


// 生成强平报单
void FStrategy::CreateForceCloseOrder(const PositionDetailField& positionDetail,OrderField& order)  
{
	InstrumentField instrument1;

	GetNewInstrument(positionDetail.InstrumentID, instrument1);


	// 构造 报单 ----------------------------------------
	///投资者代码
	order.InvestorID = positionDetail.InvestorID;

	///策略ID
	order.StrategyID = positionDetail.StrategyID;

	///合约代码
	order.InstrumentID =positionDetail.InstrumentID;

	///委托时间
	order.InsertTime =pFQuotes->newIndicator.TradingTime;  // yyyy-mm-dd hh:mm:ss.000

	///报单引用
	order.OrderRef = NextOrderRef++;

	///平仓ID
	order.OpenOrderRef = positionDetail.OrderRef; 

	///组合开平标志
	order.CombOffsetFlag = "CLOSE";

	///买卖方向
	order.Direction = (positionDetail.Direction == "BUY") ? "SELL" : "BUY";
	
	///报单价格 - 根据对手价格买卖
	if(order.Direction == "BUY")
		order.OrderPrice = pFQuotes->GetInstNewAskPrice1(order.InstrumentID);  
	else
		order.OrderPrice = pFQuotes->GetInstNewBidPrice1(order.InstrumentID); 


	///报单数量
	order.OrderVolume = positionDetail.Volume;
	///报单金额
	order.OrderAmount = order.OrderVolume * order.OrderPrice * instrument1.VolumeMultiple;

	///冻结的保证金
	order.FrozenMargin = 0;

	/*if(order.Direction == "BUY")
		order.FrozenMargin = order.OrderAmount * instrument1.LongMarginRatio;
	else
		order.FrozenMargin = order.OrderAmount * instrument1.ShortMarginRatio;*/

	
	///冻结的手续费
	if (instrument1.CloseRatioByMoney > 0)
		order.FrozenCommission = order.OrderAmount * instrument1.CloseRatioByMoney;
	else if (instrument1.CloseRatioByVolume > 0)
		order.FrozenCommission = order.OrderVolume * instrument1.CloseRatioByVolume;
	
	// 如果是今日开仓， 且两种平今手续费率都是0，则平仓手续费为0。
	if(instrument1.CloseTodayRatioByMoney == 0 && instrument1.CloseTodayRatioByVolume == 0
		&& positionDetail.OpenDate == pFQuotes->newIndicator.TradingTime.substr(0,10)
	  )
	{
		order.FrozenCommission = 0;
	}

	///止损价
	order.StopPrice = 0;
	
	///报单状态
	order.OrderStatus = 1;  // 1-未成交，0-已成交，-1-已撤单
	
	///状态信息
	order.StatusMsg = "未成交";

	///撤销时间
	order.CancelTime ="";

	///成交时间
	order.UpdateTime ="";
	
	///成交价格
	order.Price =0;
	///成交数量
	order.Volume =0;
	///成交金额
	order.Amount =0;

	///保证金
	order.Margin = positionDetail.Margin; // 设置为 持仓的保证金，用于平仓之后解除保证金
	///手续费
	order.Commission =0;

	// 平仓盈亏(只有当平仓之后才赋予值)
	order.CloseProfit = 0;

	return;

}


// 提交报单,包括开仓、平仓，不包括撤单
void FStrategy::InsertVecOrders(const OrderField& order)
{
	// 提交报单至vecOrders
	vecOrders.push_back(order);

	// 如果是平仓，则更新positionDetail的LeftVolume
	if(order.CombOffsetFlag == "CLOSE")
		UpdatePositionDetailByOrder(order);

	// 打印vecOrders至log文件
	///PrintVecOrders();


	// 冻结保证金、手续费
	pFAccount->UpdateAccountByOrder(order);
		
	return;
}

// 报单操作,处理撤单等，不包括开仓、平仓
void FStrategy::OrderAction(OrderField order,QuoteField &newQuote)
{
	vector<OrderField>::iterator  iter;
	for(iter = vecOrders.begin(); iter != vecOrders.end(); iter++)
	{
		if ((*iter).OrderRef == order.OrderRef)
		{
			(*iter).OrderStatus = -1;
			(*iter).CancelTime = newQuote.TradingTime;
		}
	}

	return;

}

// 撮合成交，判断订单是否可以成交，包括 开仓、平仓
void FStrategy::MatchOrders(const IndicatorField& newIndicator,const string priceType)  // priceType: "NEW"，"CONTERPARTY"
{
	double price = 0;

	vector<OrderField>::iterator iter;
	for(iter = vecOrders.begin(); iter != vecOrders.end(); iter++)
	{
		// 是最新行情对应的合约代码，且报单状态为未成交
		if(iter->InstrumentID == newIndicator.InstrumentID && iter->OrderStatus == 1)
		{

			// 确定交易价格：最新价 or 对手价
			if( priceType == "NEW")
				price = newIndicator.LastPrice;
			else
			{
				if (iter->Direction == "BUY")
					price =newIndicator.AskPrice1;  // OPEN BUY，CLOSE BUY
				else
					price =newIndicator.BidPrice1; // OPEN SELL，CLOSE SELL
			}

			if(iter->CombOffsetFlag == "OPEN")
				MatchOpenOrders(newIndicator,*iter,price); 
			else
				MatchCloseOrders(newIndicator,*iter,price);


		} // OrderStatus == 1

	} // for

	return;

}



// 撮合成交：开仓单
void FStrategy::MatchOpenOrders(const IndicatorField& newIndicator,OrderField& order,const double price)
{

	if(order.Direction == "BUY")
	{
		
		// 委托价格 >= 最新价格
		if(order.OrderPrice >= price)
		{
			// 更新报单的成交数据 -------------------
			order.OrderStatus = 0;
			order.StatusMsg = "已成交";
			order.UpdateTime = newIndicator.TradingTime;

			order.Price = price;  // 最新价、对手价
			order.Volume = order.OrderVolume;
			order.Amount = order.Volume * order.Price * newInstrument.VolumeMultiple;
												
			//if(order.CombOffsetFlag == "OPEN")  
			//{
				order.StopPrice = order.Price; // 开仓，平仓 =0

				order.Margin = order.Amount * newInstrument.LongMarginRatio; // 开仓，平仓 =0

				if (newInstrument.OpenRatioByMoney > 0)
					order.Commission = order.Amount * newInstrument.OpenRatioByMoney; 
				else if (newInstrument.OpenRatioByVolume > 0)
					order.Commission = order.Volume * newInstrument.OpenRatioByVolume; 

				order.CloseProfit = 0;
				
			//}  // "OPEN"
			
			// 打印vecOrders
			///PrintVecOrders();

			// 成交之后，更新Account的资金----------------
			pFAccount->UpdateAccountByOrder(order);

			// 更新持仓明细
			InsertPositionDetail(order);


		}

				

	} // "BUY"
	else 
	{ // "SELL"
		
		// 委托价格 <= 最新价格
		if(order.OrderPrice <= price)
		{
			// 更新报单的成交数据 -------------------
			order.OrderStatus = 0;
			order.StatusMsg = "已成交";
			order.UpdateTime = newIndicator.TradingTime;

			order.Price = price;  // 最新价、对手价
			order.Volume = order.OrderVolume;
			order.Amount = order.Volume * order.Price * newInstrument.VolumeMultiple;
						
						
			//if(order.CombOffsetFlag == "OPEN")  // 开仓，平仓
			//{
				order.StopPrice = order.Price; // 开仓，平仓 =0

				order.Margin = order.Amount * newInstrument.LongMarginRatio; // 开仓，平仓 =0

				if (newInstrument.OpenRatioByMoney > 0)
					order.Commission = order.Amount * newInstrument.OpenRatioByMoney; // 开仓，平仓
				else if (newInstrument.OpenRatioByVolume > 0)
					order.Commission = order.Volume * newInstrument.OpenRatioByVolume; // 开仓，平仓

				order.CloseProfit = 0;
			//}
			
			// 打印vecOrders
			///PrintVecOrders();
						
			// 成交之后，更新Account的资金----------------
			pFAccount->UpdateAccountByOrder(order);

			// 更新持仓明细
			InsertPositionDetail(order);


		}



	}// "SELL"

	return;
}


// 撮合成交: 平仓单
void FStrategy::MatchCloseOrders(const IndicatorField& newIndicator,OrderField& order,const double price)
{

	if(order.Direction == "BUY")
	{
		
		// 委托价格 >= 最新价格
		if(order.OrderPrice >= price)
		{
			// 更新报单的成交数据 -------------------
			order.OrderStatus = 0;
			order.StatusMsg = "已成交";
			order.UpdateTime = newIndicator.TradingTime;

			order.Price = price;
			order.Volume = order.OrderVolume;
			order.Amount = order.Volume * order.Price * newInstrument.VolumeMultiple;
												
			//if(order.CombOffsetFlag == "CLOSE")  
			//{
				//order.Margin = 0; // 平仓 = 开仓单的保证金

				if (newInstrument.CloseRatioByMoney > 0)
					order.Commission = order.Amount * newInstrument.CloseRatioByMoney; 
				else if (newInstrument.CloseRatioByVolume > 0)
					order.Commission = order.Volume * newInstrument.CloseRatioByVolume; 
							
				// 当平今仓手续费 都为 0时，且是当日报单，手续费为零
				if(newInstrument.CloseTodayRatioByMoney == 0 && newInstrument.CloseRatioByVolume == 0
					&& order.InsertTime.substr(0,10) == newIndicator.TradingTime.substr(0,10)  // 当日的报单
					)
					order.Commission = 0;

				order.CloseProfit = CalcCloseProfit(order);

			//}
			
			// 打印vecOrders
			///PrintVecOrders();

			// 成交之后，更新Account的资金----------------
			pFAccount->UpdateAccountByOrder(order);

			// 成交之后，更新持仓明细
			UpdatePositionDetailByTrade(order);


		} //OrderPrice
				

	} // "BUY"

	else 

	{ // "SELL"
		
		// 委托价格 <= 最新价格
		if(order.OrderPrice <= price)
		{
			// 更新报单的成交数据 -------------------
			order.OrderStatus = 0;
			order.StatusMsg = "已成交";
			order.UpdateTime = newIndicator.TradingTime;

			order.Price = price;  // 最新价、对手价
			order.Volume = order.OrderVolume;
			order.Amount = order.Volume * order.Price * newInstrument.VolumeMultiple;
						
						
			//if(order.CombOffsetFlag == "CLOSE")  // 开仓，平仓
			//{
				//order.Margin = 0; // 平仓 = 开仓单的保证金

				if (newInstrument.CloseRatioByMoney > 0)
					order.Commission = order.Amount * newInstrument.CloseRatioByMoney; // 开仓，平仓
				else if (newInstrument.CloseRatioByVolume > 0)
					order.Commission = order.Volume * newInstrument.CloseRatioByVolume; // 开仓，平仓
							
				// 当平今仓手续费 都为 0时，手续费为零,当日开仓的
				if(newInstrument.CloseTodayRatioByMoney == 0 && newInstrument.CloseRatioByVolume == 0
					&& order.InsertTime.substr(0,10) == newIndicator.TradingTime.substr(0,10)  // 当日的报单
					)
					order.Commission = 0;

				order.CloseProfit = CalcCloseProfit(order);
			//}

			// 打印vecOrders
			///PrintVecOrders();
						
			// 成交之后，更新Account的资金----------------
			pFAccount->UpdateAccountByOrder(order);

			// 成交之后，更新持仓明细
			UpdatePositionDetailByTrade(order);


		} // OrderPrice



	}// "SELL"

	return;
}



// 撤单开仓单操作，撮合之后，若订单不成交则马上撤掉开仓单
void FStrategy::CancelOrders(const IndicatorField& newIndicator)
{
	static OrderField order1;
	
	vector<OrderField>::iterator iter;
	unsigned int intNum = 0;
	unsigned int intSize = vecOrders.size();  // 【重要】保存for执行之前的size数量，程序执行过程中新插入的记录不再处理。

	//for(iter = vecOrders.begin(); iter != vecOrders.end(); iter++)
	for(intNum = 0; intNum <= intSize - 1; ++intNum)
	{
		// 是最新行情对应的合约代码，且报单状态为未成交
		if(vecOrders.at(intNum).InstrumentID == newIndicator.InstrumentID && vecOrders.at(intNum).OrderStatus == 1)
		{
			    // 不论开仓、平仓单，均先撤单、马上重新发出
				
			    // 撤单
				vecOrders.at(intNum).OrderStatus = -1;
				vecOrders.at(intNum).StatusMsg = "已撤单";
				vecOrders.at(intNum).CancelTime = newIndicator.TradingTime;
			
				// 撤单之后，更新Account的资金----------------
				pFAccount->UpdateAccountByOrder(vecOrders.at(intNum));

				// 重新发送
				memset(&order1,0,sizeof(OrderField));
				memcpy(&order1,&(vecOrders.at(intNum)),sizeof(order1));

				order1.InsertTime = newIndicator.TradingTime;
				order1.OrderRef = NextOrderRef++;
				order1.OpenOrderRef = (order1.CombOffsetFlag == "CLOSE") ? vecOrders.at(intNum).OpenOrderRef : 0;
				order1.OrderStatus = 1;
				order1.StatusMsg = "未成交";
				order1.OrderPrice = (order1.Direction == "BUY") ? newIndicator.AskPrice1 : newIndicator.BidPrice1;
				order1.OrderAmount = order1.OrderVolume * order1.OrderPrice * newInstrument.VolumeMultiple;

				if(order1.CombOffsetFlag == "OPEN")
				   order1.FrozenMargin = order1.OrderAmount * ((order1.Direction == "BUY") ? newInstrument.LongMarginRatio:newInstrument.ShortMarginRatio);
				else
					order1.FrozenMargin = 0;

				order1.FrozenCommission = vecOrders.at(intNum).FrozenCommission;
				order1.CancelTime = "";

				if(order1.CombOffsetFlag == "OPEN")
				   dayTradingNum -= 1;    // 如果开仓单撤单了，则交易笔数 -1

				// 提交报单
				InsertVecOrders(order1);

				
		} // OrderStatus == 1

	} // for

	
	// 打印vecOrders
	///PrintVecOrders();
	
	return;
}



// 开仓成交之后，生成持仓明细
void FStrategy::InsertPositionDetail(const OrderField& trade)
{
	static PositionDetailField positionDetail;
	memset(&positionDetail,0,sizeof(PositionDetailField));

	// 生成持仓变量
	CreatePositionDetail(trade,positionDetail);

	// 插入持仓明细
	InsertVecPositionDetails(positionDetail);

	return;
}

// 根据成交之后的开仓委托单，生成持仓明细
void FStrategy::CreatePositionDetail(const OrderField& trade, PositionDetailField& positionDetail)
{
	// 构造持仓明细

	///投资者代码
	positionDetail.InvestorID = pFAccount->InvestorID;

	///策略ID
	positionDetail.StrategyID = StrategyID;

	positionDetail.TradingTime = trade.UpdateTime;

	///合约代码
	positionDetail.InstrumentID = trade.InstrumentID;
	
	///成交编号
	positionDetail.OrderRef = trade.OrderRef;

	///买卖
	positionDetail.Direction = trade.Direction;

	///开仓日期
	positionDetail.OpenDate = trade.UpdateTime.substr(0,10);
		
	///数量
	positionDetail.Volume = trade.Volume;

	///成交金额
	positionDetail.Amount = trade.Amount;
	
	///开仓价
	positionDetail.OpenPrice = trade.Price;

	///投资者保证金
	positionDetail.Margin = trade.Margin;

	///止损价
	positionDetail.StopPrice = trade.StopPrice;  // 跟踪止损价格

	positionDetail.CloseTime = "";

	///平仓量
	positionDetail.CloseVolume = 0;
	///平仓金额
	positionDetail.CloseAmount = 0;

	///平仓盈亏
	positionDetail.CloseProfit = 0;

	///可平仓量
	positionDetail.LeftVolume = trade.Volume;



	return;
}



// 将开仓的持仓明细插入vector
void FStrategy::InsertVecPositionDetails(const PositionDetailField& positionDetail)
{
	vecPositionDetails.push_back(positionDetail);

	// 打印vecPositionDetails
	///PrintVecPositionDetails();

	return;
}

// 发出平仓委托之后，更新持仓明细的LeftVolume
void FStrategy::UpdatePositionDetailByOrder(const OrderField& order)
{
	vector<PositionDetailField>::iterator iter;
	for(iter = vecPositionDetails.begin(); iter != vecPositionDetails.end(); iter++)
	{
		if(iter->OrderRef == order.OpenOrderRef && order.CombOffsetFlag == "CLOSE")
		{
			iter->LeftVolume = iter->Volume - order.OrderVolume;			
		}

	} // for

	// 打印vecPositionDetails
	///PrintVecPositionDetails();

	return;

}


// 平仓成交之后，更新持仓明细
void FStrategy::UpdatePositionDetailByTrade(const OrderField& trade)
{
	vector<PositionDetailField>::iterator iter;
	for(iter = vecPositionDetails.begin(); iter != vecPositionDetails.end(); iter++)
	{
		if(iter->OrderRef == trade.OpenOrderRef)
		{
			iter->CloseTime = trade.UpdateTime;
			iter->CloseVolume = trade.Volume;
			iter->CloseAmount = trade.Amount;
			iter->LeftVolume = iter->Volume - iter->CloseVolume;
			iter->CloseProfit = trade.CloseProfit; // (iter->Direction=="BUY") ? (iter->CloseAmount - iter->Amount):(iter->Amount - iter->CloseAmount);
		}

	} // for

	// 打印vecPositionDetails
	///PrintVecPositionDetails();

	return;
}

// 限定时间达到时，强制平仓当天所有持仓，
void FStrategy::ForceClose()
{
	static OrderField order2;

	vector<PositionDetailField>::iterator iter;
	for(iter = vecPositionDetails.begin(); iter != vecPositionDetails.end(); iter++)
	{
		if(iter->LeftVolume >= 1)
		{
			memset(&order2,0,sizeof(OrderField));

			// 构造 强制平仓委托单
			CreateForceCloseOrder(*iter,order2);
			
			// 提交报单
			InsertVecOrders(order2);

		}

	} // for

}



// 计算平仓盈亏(order必须先更新成交结果)
double FStrategy::CalcCloseProfit( const OrderField& order)
{
	double closeProfit = 0;

	vector<OrderField>::iterator iter;
	for(iter = vecOrders.begin(); iter != vecOrders.end(); iter++)
	{
		if(iter->OrderRef == order.OpenOrderRef && order.Amount > 0)
		{
			if (iter->Direction == "BUY")  // 开仓单是买单时
				closeProfit = order.Amount - iter->Amount;
			else  // == "SELL"
				closeProfit = iter->Amount - order.Amount;
		}

	} // for

	return closeProfit;
}

// 获取 某合约 尚未成交的全部委托单数量（含开仓、平仓）
int FStrategy::GetInstOrderNum(const string instrumentId)
{
	int intNum = 0;

	vector<OrderField>::iterator iter ;
	for (iter = vecOrders.begin(); iter != vecOrders.end(); iter++)
	{
		if(iter->InstrumentID == instrumentId && iter->OrderStatus == 1 )
		{
			intNum += 1;
		}
	}

	return intNum;

}


// 获取 某合约 尚未成交的“开仓”委托单数量
int FStrategy::GetInstOpenOrderNum(string instrumentId)
{
	int intNum = 0;

	vector<OrderField>::iterator iter ;
	for (iter = vecOrders.begin(); iter != vecOrders.end(); iter++)
	{
		if(iter->InstrumentID == instrumentId && iter->CombOffsetFlag == "OPEN" && iter->OrderStatus == 1 )
		{
			intNum += 1;
		}
	}

	return intNum;
}

// 获取 某合约 尚未成交的“开仓”委托单数量
int FStrategy::GetInstCloseOrderNum(string instrumentId)
{
	int intNum = 0;

	vector<OrderField>::iterator iter ;
	for (iter = vecOrders.begin(); iter != vecOrders.end(); iter++)
	{
		if(iter->InstrumentID == instrumentId && iter->CombOffsetFlag == "CLOSE" && iter->OrderStatus == 1 )
		{
			intNum += 1;
		}
	}

	return intNum;
}


// 获取 全部合约 尚未成交的“开仓”委托单数量
int FStrategy::GetOpenOrderNum()
{
	int intNum = 0;

	vector<OrderField>::iterator iter ;
	for (iter = vecOrders.begin(); iter != vecOrders.end(); iter++)
	{
		if( iter->CombOffsetFlag == "OPEN" && iter->OrderStatus == 1)
		{
			intNum += 1;
		}
	}

	return intNum;
}

// 获取 某合约的持仓数量
int FStrategy::GetInstPositionNum(const string instrumentId,const string volumeType)
{//volumeType: VOLUME、LEFTVOLUME
	int intNum = 0;

	vector<PositionDetailField>::iterator iter ;
	for (iter = vecPositionDetails.begin(); iter != vecPositionDetails.end(); iter++)
	{
		if(iter->InstrumentID == instrumentId && volumeType == "VOLUME" && (iter->Volume - iter->CloseVolume) >= 1)
			{
				intNum += 1;
			}
			else if (iter->InstrumentID == instrumentId && volumeType == "LEFTVOLUME" && iter->LeftVolume >= 1)
			{
				intNum += 1;
			}
	}

	return intNum;
	
}


// 获取 某合约的（多头持仓、空头持仓；当前持仓、当前可平仓）持仓数量
int  FStrategy::GetInstLongPositionNum(const string instrumentId,const string volumeType)
{	//volumeType: VOLUME、LEFTVOLUME

	int intNum = 0;

	vector<PositionDetailField>::iterator iter ;
	for (iter = vecPositionDetails.begin(); iter != vecPositionDetails.end(); iter++)
	{
		if(iter->InstrumentID == instrumentId && iter->Direction == "BUY" )
		{
			if(volumeType == "VOLUME" && iter->Volume >= 1)
			{
				intNum += 1;
			}
			else if (volumeType == "LEFTVOLUME" && iter->LeftVolume >= 1)
			{
				intNum += 1;
			}

		} // InstrumentID
	} // for

	return intNum;
	
}

// 获取 某合约的空头持仓数量
int  FStrategy::GetInstShortPositionNum(const string instrumentId,const string volumeType)
{   //volumeType: VOLUME、LEFTVOLUME

	int intNum = 0;

	vector<PositionDetailField>::iterator iter ;
	for (iter = vecPositionDetails.begin(); iter != vecPositionDetails.end(); iter++)
	{
		if(iter->InstrumentID == instrumentId && iter->Direction == "SELL" )
		{
			if(volumeType == "VOLUME" && iter->Volume >= 1)
			{
				intNum += 1;
			}
			else if (volumeType == "LEFTVOLUME" && iter->LeftVolume >= 1)
			{
				intNum += 1;
			}

		} // InstrumentID
	} // for

	return intNum;
	
}


// 获取 全部合约的持仓数量
int FStrategy::GetPositionNum()
{
	int intNum = 0;

	vector<PositionDetailField>::iterator iter ;
	for (iter = vecPositionDetails.begin(); iter != vecPositionDetails.end(); iter++)
	{
		if( iter->Volume == 1)
		{
			intNum += 1;
		}
	}

	return intNum;
}

// 获取 某合约的可平仓数量
int FStrategy::GetInstLeftPositionNum(string instrumentId)
{
	int intNum = 0;

	vector<PositionDetailField>::iterator iter ;
	for (iter = vecPositionDetails.begin(); iter != vecPositionDetails.end(); iter++)
	{
		if(iter->InstrumentID == instrumentId && iter->LeftVolume == 1)
		{
			intNum += 1;
		}
	}

	return intNum;
}
//// 获取 全部合约的可平仓数量
//int FStrategy::GetLeftPositionNum()
//{
//
//}

// 获取某合约的持仓盈亏
double FStrategy::GetInstPositionProfit(string instrumentId)
{
	double dblProfit = 0;
	double dblMarketValue = 0;

	vector<PositionDetailField>::iterator iter ;
	for (iter = vecPositionDetails.begin(); iter != vecPositionDetails.end(); iter++)
	{
		if( iter->InstrumentID == instrumentId && iter->Volume > 0)
		{
			dblMarketValue = iter->Volume * pFQuotes->GetInstLastPrice(instrumentId) * newInstrument.VolumeMultiple;

			dblProfit += iter->Volume * pFQuotes->GetInstLastPrice(instrumentId);
		}
	}

	return dblProfit;

}
// 获取全部合约的持仓盈亏
double FStrategy::GetPositionProfit()
{
	double dblProfit = 0;
	double dblMarketValue = 0;

	vector<PositionDetailField>::iterator iter ;
	for (iter = vecPositionDetails.begin(); iter != vecPositionDetails.end(); iter++)
	{
		if( iter->Volume > 0)
		{
			// Market Value
			dblMarketValue = iter->Volume * pFQuotes->GetInstLastPrice(iter->InstrumentID) * GetInstVolumeMultiple(iter->InstrumentID);

			// 持仓盈亏
			if (iter->Direction == "BUY")
				dblProfit += dblMarketValue - iter->Amount;
			else
				dblProfit += iter->Amount - dblMarketValue;
		}

	} // for
	
	return dblProfit;
}

// 获取 全部合约 当天的 总盈亏 （持仓盈亏+平仓盈亏）
double FStrategy::GetProfit()
{
	double dblProfit = 0;
	double dblMarketValue = 0;

	vector<PositionDetailField>::iterator iter ;
	for (iter = vecPositionDetails.begin(); iter != vecPositionDetails.end(); iter++)
	{

		if( iter->Volume > 0)
		{
			// Market Value
			dblMarketValue = iter->Volume * pFQuotes->GetInstLastPrice(iter->InstrumentID) * GetInstVolumeMultiple(iter->InstrumentID);
		

			// 持仓盈亏
			if (iter->Direction == "BUY")
				dblProfit += dblMarketValue - iter->Amount;
			else
				dblProfit += iter->Amount - dblMarketValue;
		}

		// + 平仓盈亏
		dblProfit += iter->CloseProfit;

	} // for
	
	return dblProfit;
}




// 获取合约基础信息
void  FStrategy::GetNewInstrument(const string instrumentId, InstrumentField& newInstrument)
{
	
	map<string, InstrumentField>::iterator iter= mapInstruments.find(instrumentId);

	if(iter != mapInstruments.end()) 
	{
		try 
		{
			//更新当前最新行情对应的合约信息
			memset(&newInstrument,0,sizeof(InstrumentField));
			memcpy(&newInstrument, &iter->second,sizeof(InstrumentField));
		
		}
		catch(exception e)
		{
			cerr<<"GetNewInstrument Error!"<<e.what()<<endl;
			int intX = 0;
		}
	}
	else 
	{
		//没找到,报错
		cerr<<"GetNewInstrument Error：mapInstruments里面不存在此合约!"<<endl;

		return;
	}


}

// 获取 合约乘数
int FStrategy::GetInstVolumeMultiple(const string instrumentId)
{
	int intVolumeMultiple = 0;

	map<string, InstrumentField>::iterator iter= mapInstruments.find(instrumentId);

	if(iter != mapInstruments.end()) 
	{
		// 找到Key
		intVolumeMultiple = iter->second.VolumeMultiple;
	}
	else 
	{
		//没找到,报错
		cerr<<"GetInstVolumeMultiple Error：mapInstruments里面不存在此合约!"<<endl;
	}

	return intVolumeMultiple;

}

// 根据最新行情，调整持仓合约的止盈止损价格
void FStrategy::UpdateStopPrice(const IndicatorField& newIndicator)
{
	vector<PositionDetailField>::iterator iter;
	for(iter = vecPositionDetails.begin(); iter != vecPositionDetails.end(); iter++)
	{
		if( iter->InstrumentID == newIndicator.InstrumentID && (iter->Volume - iter->CloseVolume) >=1 )
		{
			if(iter->Direction == "BUY")
			{
				// 上调买单的止损价格
				if(newIndicator.LastPrice >= (iter->StopPrice + Strategy.ModifyRange) )
				{
					iter->StopPrice += (newIndicator.LastPrice - iter->StopPrice) * Strategy.ModifyRatio;

					// 打印 vecPositionDetails
					///PrintVecPositionDetails();

				}

				// “初次”上调买单的止损价格,变化4倍PriceTick之后调整
				if( iter->OpenPrice == iter->StopPrice && newIndicator.LastPrice >= (iter->OpenPrice + 4 * newInstrument.PriceTick) )
				{
					// 调整 2 倍 PriceTick
					iter->StopPrice += 2 * newInstrument.PriceTick;

					// 打印 vecPositionDetails
					///PrintVecPositionDetails();

				}


			}  // BUY
			else  // "SELL"
			{
				// 下调卖单的止损价格
				if(newIndicator.LastPrice <= (iter->StopPrice - Strategy.ModifyRange) )
				{
					iter->StopPrice -= (iter->StopPrice - newIndicator.LastPrice ) * Strategy.ModifyRatio;

					// 打印 vecPositionDetails
					///PrintVecPositionDetails();

				}

				// “初次”上调买单的止损价格,变化4倍PriceTick之后调整
				if( iter->OpenPrice == iter->StopPrice && newIndicator.LastPrice <= (iter->OpenPrice - 4 * newInstrument.PriceTick) )
				{
					// 调整 2 倍 PriceTick
					iter->StopPrice -= 2 * newInstrument.PriceTick;

					// 打印 vecPositionDetails
					///PrintVecPositionDetails();

				}


			} // SELL

		} // InstrumentID
	} // for

	return;
}

// 根据最新行情，调整持仓合约的止盈止损价格,仅调整一次，目的控制交易损失
void FStrategy::UpdateStopPriceFirst(const IndicatorField& newIndicator)
{
	vector<PositionDetailField>::iterator iter;
	for(iter = vecPositionDetails.begin(); iter != vecPositionDetails.end(); iter++)
	{
		if( iter->InstrumentID == newIndicator.InstrumentID && (iter->Volume - iter->CloseVolume) >=1 )
		{
			if(iter->Direction == "BUY")
			{
				//// 上调买单的止损价格
				//if(newIndicator.LastPrice >= (iter->StopPrice + Strategy.ModifyRange) )
				//{
				//	iter->StopPrice += (newIndicator.LastPrice - iter->StopPrice) * Strategy.ModifyRatio;

				//	// 打印 vecPositionDetails
				//	PrintVecPositionDetails();

				//}

				// “初次”上调买单的止损价格,变化4倍PriceTick之后调整
				if( iter->OpenPrice == iter->StopPrice && newIndicator.LastPrice >= (iter->OpenPrice + 4 * newInstrument.PriceTick) )
				{
					// 调整 2 倍 PriceTick
					iter->StopPrice += 2 * newInstrument.PriceTick;

					// 打印 vecPositionDetails
					///PrintVecPositionDetails();

				}


			}  // BUY
			else  // "SELL"
			{
				//// 下调卖单的止损价格
				//if(newIndicator.LastPrice <= (iter->StopPrice - Strategy.ModifyRange) )
				//{
				//	iter->StopPrice -= (iter->StopPrice - newIndicator.LastPrice ) * Strategy.ModifyRatio;

				//	// 打印 vecPositionDetails
				//	PrintVecPositionDetails();

				//}

				// “初次”上调买单的止损价格,变化4倍PriceTick之后调整
				if( iter->OpenPrice == iter->StopPrice && newIndicator.LastPrice <= (iter->OpenPrice - 4 * newInstrument.PriceTick) )
				{
					// 调整 2 倍 PriceTick
					iter->StopPrice -= 2 * newInstrument.PriceTick;

					// 打印 vecPositionDetails
					///PrintVecPositionDetails();

				}


			} // SELL

		} // InstrumentID
	} // for

	return;
}


// 每日交易及盈亏分析
void FStrategy::DailyStat(const string TradingDate)
{
	 
	// 资金余额

	// 平仓盈亏

	// 持仓盈亏

	// 总盈亏

	// 总资产

	// 交易笔数

	// 交易手续费

	int intX = 0;

	return;

}


// 将vecOrders输出至log文件
void FStrategy::PrintVecOrders()
{
	    // 获取当前系统时间，精确到毫秒
		string strNewTime=GetNowMillisecond(); 

		ofstream log_md("AATS_vecOrders_log.csv",ios::app);
		//log_md<<strNewTime<<",ATS_vecOrders_log,begin,---"<<endl;

		vector<OrderField>::iterator iter;
		for(iter = vecOrders.begin(); iter != vecOrders.end(); iter++)
		{
			
			///投资者代码
			log_md<<iter->InvestorID;

			///策略ID
			log_md<<","<<iter->StrategyID;

			///合约代码
			log_md<<","<<iter->InstrumentID;

			///委托时间
			log_md<<","<<iter->InsertTime;  // yyyy-mm-dd hh:mm:ss.000

			///报单引用
			log_md<<","<<iter->OrderRef;

			///平仓ID
			log_md<<","<<iter->OpenOrderRef;  // = TradeID

			///组合开平标志
			log_md<<","<<iter->CombOffsetFlag;

			///买卖方向
			log_md<<","<<iter->Direction;
	
			///报单价格
			log_md<<","<<iter->OrderPrice;
			///报单数量
			log_md<<","<<iter->OrderVolume;
			///报单金额
			log_md<<","<<iter->OrderAmount;

			///冻结的保证金
			log_md<<","<<iter->FrozenMargin;
			///冻结的手续费
			log_md<<","<<iter->FrozenCommission;

			///止损价
			log_md<<","<<iter->StopPrice;
	
			///报单状态
			log_md<<","<<iter->OrderStatus;  // 1-未成交，0-已成交，-1-已撤单
	
			///状态信息
			log_md<<","<<iter->StatusMsg;

			///撤销时间
			log_md<<","<<iter->CancelTime;

			///成交时间
			log_md<<","<<iter->UpdateTime;
	
			///成交价格
			log_md<<","<<iter->Price;
			///成交数量
			log_md<<","<<iter->Volume;
			///成交金额
			log_md<<","<<iter->Amount;

			///保证金
			log_md<<","<<iter->Margin;
			///手续费
			log_md<<","<<iter->Commission;

			// 平仓盈亏(只有当平仓之后才赋予值)
			log_md<<","<<iter->CloseProfit;
			
			log_md<<endl;


		} // for

		//log_md<<"---,vecOrders,end,---"<<endl;
		log_md.close();

}



// 将vecPositionDetails输出至log文件
void FStrategy::PrintVecPositionDetails()
{
	// 获取当前系统时间，精确到毫秒
		string strNewTime=GetNowMillisecond(); 

		//ofstream log_md("ATS_vecOrders_log.csv",ios::app);
		ofstream log_md("AATS_vecPositionDetails_log.csv",ios::app);
		//log_md<<strNewTime<<",ATS_vecPositionDetails_log,begin,---"<<endl;

		vector<PositionDetailField>::iterator iter;
		for(iter = vecPositionDetails.begin(); iter != vecPositionDetails.end(); iter++)
		{

			///投资者代码
			log_md<<iter->InvestorID;

			///策略ID
			log_md<<","<<iter->StrategyID;

			// 成交时间
			log_md<<","<<iter->TradingTime;

			///合约代码
			log_md<<","<<iter->InstrumentID;
	
			///成交编号
			log_md<<","<<iter->OrderRef;

			///买卖
			log_md<<","<<iter->Direction;
			///开仓日期
			log_md<<","<<iter->OpenDate;
		
			///数量
			log_md<<","<<iter->Volume;

			///开仓价
			log_md<<","<<iter->OpenPrice;

			///成交金额
			log_md<<","<<iter->Amount;
	

			///投资者保证金
			log_md<<","<<iter->Margin;

			///止损价
			log_md<<","<<iter->StopPrice;  // 跟踪止损价格

			/// 平仓时间
			log_md<<","<<iter->CloseTime; 
			///平仓量
			log_md<<","<<iter->CloseVolume;
			///平仓金额
			log_md<<","<<iter->CloseAmount;

			///平仓盈亏
			log_md<<","<<iter->CloseProfit;

			///可平仓量
			log_md<<","<<iter->LeftVolume;

			log_md<<endl;


		} // for

		//log_md<<"---,vecPositionDetails,end,---"<<endl;
		log_md.close();

}


//// 将信息写入日志文件
//	void FStrategy::printMessage2Log(string strMessage)
//	{
//	    // 获取当前系统时间，精确到毫秒
//		string strNewTime=GetNowMillisecond(); 
//
//		ofstream log_md("TestData/fun_order_log.txt",ios::app);
//		log_md<<"["<<strNewTime<<"] "<<strMessage<<endl;
//		log_md.close();
//
//	}

	// 获取系统当前时间的毫秒数
	string FStrategy::GetNowMillisecond(void)
	{
		// 获取当前系统时间，精确到毫秒-----------------
		SYSTEMTIME sys;          // 需在程序头部添加 #include "windows.h"
		GetLocalTime( &sys ); 
		char buf1[100];
		sprintf_s(buf1, "%4d-%02d-%02d %02d:%02d:%02d.%03d", sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute,sys.wSecond,sys.wMilliseconds);
		//sprintf_s(buf1, "%02d:%02d:%02d.%03d", sys.wHour,sys.wMinute,sys.wSecond,sys.wMilliseconds);
		string strNewTime= buf1;  // 本地系统最新时间，精确到毫秒
		// 获取当前系统时间，精确到毫秒  end-----------

		return strNewTime;
	}

	
//// 撮合成交，包括开仓、平仓
//void FStrategy::MatchOrders(const IndicatorField& newIndicator,OrderField& order,const string buySell,const double price)
//{
//
//	if(buySell == "BUY")
//	{
//		
//		// 不论 开仓单、平仓单,委托价格 >= 最新价格
//		if(order.OrderPrice >= price)
//		{
//			// 更新报单的成交数据 -------------------
//			order.OrderStatus = 0;
//			order.StatusMsg = "已成交";
//			order.UpdateTime = newIndicator.TradingTime;
//
//			order.Price = price;
//			order.Volume = order.OrderVolume;
//			order.Amount = order.Volume * order.Price * newInstrument.VolumeMultiple;
//												
//			if(order.CombOffsetFlag == "OPEN")  
//			{
//				order.StopPrice = order.Price; // 开仓，平仓 =0
//
//				order.Margin = order.Amount * newInstrument.LongMarginRatio; // 开仓，平仓 =0
//
//				if (newInstrument.OpenRatioByMoney > 0)
//					order.Commission = order.Amount * newInstrument.OpenRatioByMoney; 
//				else if (newInstrument.OpenRatioByVolume > 0)
//					order.Commission = order.Volume * newInstrument.OpenRatioByVolume; 
//
//				order.CloseProfit = 0;
//				
//			}  // "OPEN"
//			else  // == "CLOSE"
//			{
//		
//				order.Margin = 0; // 开仓，平仓 =0
//
//				if (newInstrument.CloseRatioByMoney > 0)
//					order.Commission = order.Amount * newInstrument.CloseRatioByMoney; 
//				else if (newInstrument.CloseRatioByVolume > 0)
//					order.Commission = order.Volume * newInstrument.CloseRatioByVolume; 
//							
//				// 当平今仓手续费 都为 0时，且是当日报单，手续费为零
//				if(newInstrument.CloseTodayRatioByMoney == 0 && newInstrument.CloseRatioByVolume == 0
//					&& order.InsertTime.substr(0,10) == newIndicator.TradingTime.substr(0,10)  // 当日的报单
//					)
//					order.Commission = 0;
//
//				order.CloseProfit = CalcCloseProfit(order);
//
//			}
//						
//			// 成交之后，更新Account的资金----------------
//			pFAccount->UpdateAccountByOrder(order);
//
//			// 更新持仓明细
//			InsertPositionDetail(order);
//
//
//		}
//
//				
//
//	} // "BUY"
//	else 
//	{ // "SELL"
//		
//		// 不论 开仓单、平仓单,委托价格 <= 最新价格
//		if(order.OrderPrice <= price)
//		{
//			// 更新报单的成交数据 -------------------
//			order.OrderStatus = 0;
//			order.StatusMsg = "已成交";
//			order.UpdateTime = newIndicator.TradingTime;
//
//			order.Price = price;  // 成交价格按照行情价格
//			order.Volume = order.OrderVolume;
//			order.Amount = order.Volume * order.Price * newInstrument.VolumeMultiple;
//						
//						
//			if(order.CombOffsetFlag == "OPEN")  // 开仓，平仓
//			{
//				order.StopPrice = order.Price; // 开仓，平仓 =0
//
//				order.Margin = order.Amount * newInstrument.LongMarginRatio; // 开仓，平仓 =0
//
//				if (newInstrument.OpenRatioByMoney > 0)
//					order.Commission = order.Amount * newInstrument.OpenRatioByMoney; // 开仓，平仓
//				else if (newInstrument.OpenRatioByVolume > 0)
//					order.Commission = order.Volume * newInstrument.OpenRatioByVolume; // 开仓，平仓
//
//				order.CloseProfit = 0;
//			}
//			else  // == "CLOSE"
//			{
//	
//				order.Margin = 0; // 开仓，平仓 =0
//
//				if (newInstrument.CloseRatioByMoney > 0)
//					order.Commission = order.Amount * newInstrument.CloseRatioByMoney; // 开仓，平仓
//				else if (newInstrument.CloseRatioByVolume > 0)
//					order.Commission = order.Volume * newInstrument.CloseRatioByVolume; // 开仓，平仓
//							
//				// 当平今仓手续费 都为 0时，手续费为零。【TODO】 假定没有隔夜仓
//				if(newInstrument.CloseTodayRatioByMoney == 0 && newInstrument.CloseRatioByVolume == 0
//					&& order.InsertTime.substr(0,10) == newIndicator.TradingTime.substr(0,10)  // 当日的报单
//					)
//					order.Commission = 0;
//
//				order.CloseProfit = CalcCloseProfit(order);
//			}
//
//						
//			// 成交之后，更新Account的资金----------------
//			pFAccount->UpdateAccountByOrder(order);
//
//		}
//
//
//
//	}// "SELL"
//
//	return;
//}
