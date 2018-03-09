#ifndef _ATS_BACKTESTING_H
#define _ATS_BACKTESTING_H


#include "structs.h"
#include "FStrategy.h"
#include "FAccount.h"
#include "FQuote.h"


/*-----------------------------------------------------------//
 * ��ȡ���飬���� newQuote
 * ���� mapQuotes��KeyΪ��Լ���룬ValueΪ����vector��ÿ��һ���
 * ָ�꣺AvgDay��Avg10��Avg120��Avg240
 * �˻���Account����ʼ�ʽ𡢿����ʽ𡢶��ᱣ֤�𡢶��������ѵ�
 * ������newOrder��vecOrders
 * �ɽ���newTrade��vecTrades
 * �ֲ֣�vecPositionDetails
 * Ȩ�棺�����ֵ
//-----------------------------------------------------------*/

// ----�Զ��� ����------------------------------------------------------//
//typedef map<TThostFtdcInstrumentIDType, vector<QuoteField>> MapQuotesType;
typedef map<string, InstrumentField> MapInstrumentType;
typedef map<string,vector<CommissionRateField>> MapCommissionRateType;
typedef map<string, vector<QuoteField>> MapQuotesType;
typedef map<string, vector<IndicatorField>> MapIndicatorsType;


// ---- �Զ������ -----------------------------------------------------//
string vInvestorID = "Kevin2002";	// �û�ID
string vStrategyID = "R-Breaker-M1";	// ����ID
double vInitMoney  = 100000;			// ��ʼ���ʽ�

FAccount myAccount(vInvestorID,vInitMoney);
FStrategy myStrategy(vStrategyID);
FQuote myQuotes;


// ������
string TradingDate; // yyyy-mm-dd

//// ��һ���������ñ���
//long NextOrderRef ;  

//// �˻���Ϣ
//AccountField myAccount;
//
//// ������Ϣ
//StrategyField_R_Breaker myStrategy;			

QuoteField newQuote;			// ��������


//QuoteField newQuote;			// ��������
//IndicatorField newIndicator;	// ����ָ��
//OrderField newOrder;				// ���±���

//MapQuotesType mapQuotes;			// ����map�� Key��Symbol��Value��vecQuotes
//MapIndicatorsType mapIndicators;	// ָ��map�� Key��Symbol, Value: vecIndicators



// ---- �Զ��� ���� ---------------------------------------------------//
// ��ʼ�� newQuote �ṹ����
void InitNewQuote(QuoteField& newQuote);

// ��������vector����Ӧָ�꣬������������vecQuotes
void Convert2NewQuote(const vector<string>& dest, QuoteField& newQuote);


// ��ʼ������
//void InitStrategy();


 

// ��ʼ�� �µ�һ�� ������ ��ָ��
//void InitNewDay();


// �ύ�±����󣬸����˻��ʽ�
//void UpdateAccount(const OrderField& insertOrder);



// ---- �Զ��� ������������ ------------------------------------------------//

// ��ÿһ���������ݲ�ֳɲ�ͬ�ı���������vector
void split(const string& strSrc, const string& strFlag, vector<string>& vecDest) ;

#endif