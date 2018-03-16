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

	// ��ʼ�� newIndicator �ṹ����
	void InitNewIndicator(IndicatorField& newIndicator);
	void ClearMapQuotes();
	void ClearMapIndicators();

	void InitNewDayQuotes();

	// ����ָ��vector������ָ��mapQuotes
	void Update2mapNewQuotes(const QuoteField& newQuote);
	// ��������vector����Ӧָ�꣬Ȼ���������mapQuotes
	void Update2mapQuotes(const QuoteField& newQuote) ;

	// ��ȡĳ��Լ�����¼۸�
	double GetInstLastPrice(const string instrumentID);

	// ��ȡĳ��Լ��������һ�۸�
	double FQuote::GetInstNewAskPrice1(const string instrumentID);

	// ��ȡĳ��Լ��������һ�۸�
	double FQuote::GetInstNewBidPrice1(const string instrumentID);


	// ������������vector������ָ�꣬����ָ��mapIndicators
	void CalcIndicators(const QuoteField& newQuote);
	// ����ָ��vector������ָ��mapQuotes
	void Update2mapIndicators(const IndicatorField& newIndicator) ;
	// ��ȡTʱ�̵�����ָ�꣨T=0,1,2,...��
	void GetInstNewIndicator(const string instrumentID, IndicatorField& newIndicator, const int intT);
	
	
	// �������¼۸������ƽ��ֵ
	double CalcLastPriceAvg(const string instrumentID,const int intNum) ;
	// �����վ�ֵ
	double CalcLastPriceAvgDay(const string instrumentID);
	
	// ����۸�����ֵ
	double CalcLastPriceMax(const string instrumentID,const int intNum);
	// ����۸�����ֵ
	double CalcLastPriceMin(const string instrumentID,const int intNum);



public:
	
	//QuoteField newQuote;			// ��������
	IndicatorField newIndicator;	// ����ָ��

	double dayOpenPrice;  // ���տ��̼�

private:


	map<string,vector<QuoteField>> mapQuotes;			// ����map�� Key��Symbol��Value��vecQuotes
	map<string,vector<IndicatorField>> mapIndicators;	// ָ��map�� Key��Symbol, Value: vecIndicators
	map<string,QuoteField> mapNewQuotes;				// ���к�Լ����������map

};

#endif