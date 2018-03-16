#include <Windows.h>
#include "FAccount.h"
#include <iomanip>

// 构造函数，初始化账户信息
FAccount::FAccount(string investorId,double money)
{

	/////投资者代码
	InvestorID = investorId;
		
	///入金金额
	Account.Deposit = money;   // 初始资金

	/// 当前资金总额
	Account.NowCash = money;

	///冻结的保证金
	Account.FrozenMargin = 0;

	///冻结的手续费
	Account.FrozenCommission = 0;
		
	///当前保证金总额
	Account.CurrMargin = 0;
	
	///手续费
	Account.Commission = 0;

	///平仓盈亏
	Account.CloseProfit = 0;
	///持仓盈亏
	Account.PositionProfit = 0;
		
	///可用资金
	Account.Available = money;  
	
	///变动时间
	Account.UpdateTime = "";


}

//析构函数，干什么
FAccount::~FAccount(void)
{
	
}


// 根据报单，更新账户资金
void FAccount::UpdateAccountByOrder(const OrderField& order)
{
	// 开仓报单
	if(order.CombOffsetFlag == "OPEN")
	{
		
		if(order.OrderStatus == 1)
		{
			// 新提交： 冻结保证金、手续费;
			Account.FrozenMargin += order.FrozenMargin;
			Account.FrozenCommission += order.FrozenCommission;
			
			// 可用资金
			// -- 入金+平仓盈亏+持仓盈亏-手续费 = 动态权益； 
			// -- 动态权益 -占用保证金 -冻结保证金 -冻结手续费 = 可用资金
			Account.Available = Account.Available - order.FrozenMargin - order.FrozenCommission;
			Account.UpdateTime = order.InsertTime;

		}
		else if (order.OrderStatus == 0)  
		{
			// 成交： 解冻冻结的保证金、手续费；正式扣减保证金、手续费；

			// 解冻
			Account.FrozenMargin -= order.FrozenMargin;
			Account.FrozenCommission -= order.FrozenCommission;
			
			// 可用资金
			// -- 入金+平仓盈亏+持仓盈亏-手续费 = 动态权益； 
			// -- 动态权益 -占用保证金 -冻结保证金 -冻结手续费 = 可用资金
			Account.Available = Account.Available + order.FrozenMargin + order.FrozenCommission;

			// 扣减
			Account.CurrMargin += order.Margin;
			Account.Commission += order.Commission;

			Account.NowCash -= order.Commission;
			Account.Available = Account.Available - order.Margin - order.Commission;

			Account.UpdateTime = order.UpdateTime;
		}
		else // = -1
		{
			// 撤单： 解冻保证金、手续费;
			// 解冻
			Account.FrozenMargin -= order.FrozenMargin;
			Account.FrozenCommission -= order.FrozenCommission;
			
			// 可用资金
			// -- 入金+平仓盈亏+持仓盈亏-手续费 = 动态权益； 
			// -- 动态权益 -占用保证金 -冻结保证金 -冻结手续费 = 可用资金
			Account.Available = Account.Available + order.FrozenMargin + order.FrozenCommission;

			Account.UpdateTime = order.CancelTime;

		} 

	} // if(order.CombOffsetFlag == "OPEN")
	
	else // 平仓报单
	{
		//CLOSE：
		if(order.OrderStatus == 1)
		{
			// 新提交： 冻结手续费,不冻结保证金
			//Account.FrozenMargin += order.FrozenMargin;
			Account.FrozenCommission += order.FrozenCommission;
			
			// 可用资金
			// -- 入金+平仓盈亏+持仓盈亏-手续费 = 动态权益； 
			// -- 动态权益 -占用保证金 -冻结保证金 -冻结手续费 = 可用资金
			Account.Available = Account.Available - order.FrozenCommission;

			Account.UpdateTime = order.InsertTime;
		}
		else if (order.OrderStatus == 0)
		{
			// 成交： 释放保证金，扣减手续费;
			// 解冻
			//Account.FrozenMargin = 0 ; //-= order.Margin;  平仓单不冻结保证金 
			Account.FrozenCommission -= order.FrozenCommission;
			
			// 可用资金
			// -- 入金+平仓盈亏+持仓盈亏-手续费 = 动态权益； 
			// -- 动态权益 -占用保证金 -冻结保证金 -冻结手续费 = 可用资金
			Account.Available = Account.Available  + order.FrozenCommission;

			// 扣减
			Account.CurrMargin -= order.Margin;
			Account.Commission += order.Commission;

			Account.CloseProfit += order.CloseProfit;
			Account.PositionProfit = 0;               // 【TODO】 暂不处理

			Account.NowCash = Account.NowCash - order.Commission + order.CloseProfit;
			Account.Available = Account.Available + order.Margin - order.Commission + order.CloseProfit;

			Account.UpdateTime = order.UpdateTime;
		}
		else if (order.OrderStatus == -1) // = -1
		{
			// 撤单： 解冻手续费;
			// 解冻
			//Account.FrozenMargin -= order.FrozenMargin;
			Account.FrozenCommission -= order.FrozenCommission;
			
			// 可用资金
			// -- 入金+平仓盈亏+持仓盈亏-手续费 = 动态权益； 
			// -- 动态权益 -占用保证金 -冻结保证金 -冻结手续费 = 可用资金
			Account.Available = Account.Available + order.FrozenMargin ;

			Account.UpdateTime = order.CancelTime;
		}
		else
		{
			cerr<<"UpdateAccountByOrder Error:"<<endl;
			system("pause");
		}

	}
	
	// 打印Account至log
	PrintAccount();

	return;

}


// 获取当前可用资金
double FAccount::GetAvailableCash()
{
	return Account.Available;
}


// 将资金变动情况输出至log文件
void FAccount::PrintAccount()
{
	// 获取当前系统时间，精确到毫秒
		string strNewTime=GetNowMillisecond(); 

		//ofstream log_md("ATS_vecOrders_log.csv",ios::app);
		ofstream log_md("AATS_Account_log.csv",ios::app);

		log_md<<strNewTime;
					
		///入金金额
		log_md<<","<<Account.Deposit;   // 初始资金

		///当前金额
		log_md<<","<<Account.NowCash;

		///冻结的保证金
		log_md<<","<<Account.FrozenMargin;
		///冻结的手续费
		log_md<<","<<Account.FrozenCommission;
		
		///当前保证金总额
		log_md<<","<<Account.CurrMargin;
	
		///手续费
		log_md<<","<<Account.Commission;

		///平仓盈亏
		log_md<<","<<Account.CloseProfit;
		///持仓盈亏
		log_md<<","<<Account.PositionProfit;
		
		///可用资金
		log_md<<","<<Account.Available;  
	
		///变动时间
		log_md<<","<<Account.UpdateTime;
								
		log_md<<endl;


		//log_md<<"---,Account,end,--- "<<endl;
		log_md.close();

}

// 获取系统当前时间的毫秒数
string FAccount::GetNowMillisecond(void)
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
