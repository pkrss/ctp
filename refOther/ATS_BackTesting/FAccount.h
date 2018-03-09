#pragma once
#include <iostream>
#include <fstream>
#include "structs.h"


class FAccount
{

public:

	// 构造函数，初始化账户信息
	FAccount(string investorId,double money);

	//析构函数，干什么
	~FAccount(void);

	// 初始化 资金、策略ID
	void InitAccount(string strategyId, double initMoney);

	// 根据委托报单，更新账户资金
	void UpdateAccountByOrder(const OrderField& order);
	
	// 获取当前可用资金
	double GetAvailableCash();

	// 将资金变动情况输出至log文件
	void PrintAccount();

	// 获取系统当前时间的毫秒数
	string GetNowMillisecond(void);

public:
	
	// 账户名称
	string InvestorID;

private:
	// 账户信息
	AccountField Account;




};

