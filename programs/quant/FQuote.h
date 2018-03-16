#ifndef _FQUOTE_H
#define _FQUOTE_H

#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <map>

#include "structs.h"
#include "ThostFtdcUserApiDataType.h"


using namespace std;

class FQuote
{

public:

	FQuote(void);
	~FQuote(void);

	// 初始化 newIndicator 结构向量
	void InitNewIndicator(IndicatorField& newIndicator);
	void ClearMapQuotes();
	void ClearMapIndicators();

	void InitNewDayQuotes();

	// 根据指标vector，存入指标mapQuotes
	void Update2mapNewQuotes(const QuoteField& newQuote);
	// 根据行情vector，对应指标，然后存入行情mapQuotes
	void Update2mapQuotes(const QuoteField& newQuote) ;

	// 获取某合约的最新价格
	double GetInstLastPrice(const string instrumentID);

	// 获取某合约的最新卖一价格
	double FQuote::GetInstNewAskPrice1(const string instrumentID);

	// 获取某合约的最新买一价格
	double FQuote::GetInstNewBidPrice1(const string instrumentID);


	// 根据最新行情vector，计算指标，存入指标mapIndicators
	void CalcIndicators(const QuoteField& newQuote);
	// 根据指标vector，存入指标mapQuotes
	void Update2mapIndicators(const IndicatorField& newIndicator) ;
	// 获取T时刻的最新指标（T=0,1,2,...）
	void GetInstNewIndicator(const string instrumentID, IndicatorField& newIndicator, const int intT);
	
	
	// 计算最新价格的算术平均值
	double CalcLastPriceAvg(const string instrumentID,const int intNum) ;
	// 计算日均值
	double CalcLastPriceAvgDay(const string instrumentID);
	
	// 计算价格的最高值
	double CalcLastPriceMax(const string instrumentID,const int intNum);
	// 计算价格的最低值
	double CalcLastPriceMin(const string instrumentID,const int intNum);



public:
	
	//QuoteField newQuote;			// 最新行情
	IndicatorField newIndicator;	// 最新指标

	double dayOpenPrice;  // 当日开盘价

private:


	map<string,vector<QuoteField>> mapQuotes;			// 行情map： Key：Symbol，Value：vecQuotes
	map<string,vector<IndicatorField>> mapIndicators;	// 指标map： Key：Symbol, Value: vecIndicators
	map<string,QuoteField> mapNewQuotes;				// 所有合约的最新行情map

};

#endif