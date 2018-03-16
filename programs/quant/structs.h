#ifndef _STRUCTS_H
#define _STRUCTS_H

#include <string>
#include "ThostFtdcUserApiDataType.h"

using namespace std;




// ���Խṹ
struct StrategyField
{
	int    BSQty;	// ÿ�������ĺ�Լ����

	double ModifyRange;		// ����ֹ�𣬵����ķ�ֵ
	double ModifyRatio;		// ����ֹ�𣬵����ķ���
	double Rongrendu;		// ���ֺ�ƽ��ʱ�ļ۲� ���̶�
	string TradeBegTime;	// ÿ����Կ�ʼִ�е���ʼʱ��
	string ForceCloseTime;	// ÿ��ǿƽʱ�䣬��14:58:00


};



///�ʽ��˻�
struct AccountField
{

	/////Ͷ���ߴ���
	//string	InvestorID;
		
	///�����
	TThostFtdcMoneyType	Deposit;   // ��ʼ�ʽ�

	///��ǰ���
	TThostFtdcMoneyType	NowCash;

	///����ı�֤��
	TThostFtdcMoneyType	FrozenMargin;
	///�����������
	TThostFtdcMoneyType	FrozenCommission;
		
	///��ǰ��֤���ܶ�
	TThostFtdcMoneyType	CurrMargin;
	
	///������
	TThostFtdcMoneyType	Commission;

	///ƽ��ӯ��
	TThostFtdcMoneyType	CloseProfit;
	///�ֲ�ӯ��
	TThostFtdcMoneyType	PositionProfit;
		
	///�����ʽ�
	TThostFtdcMoneyType	Available;  
	
	///�䶯ʱ��
	string	UpdateTime;

	// ���+ƽ��ӯ��+�ֲ�ӯ��-������ = ��̬Ȩ�棻 
	// ��̬Ȩ�� -ռ�ñ�֤�� -���ᱣ֤�� -���������� = �����ʽ�
	
};


/*
==========================================
  �ϴν���׼���𣺡�      500,000,000.00
- �ϴ����ö�ȣ�����                0.00
- �ϴ���Ѻ������                0.00
+ ��Ѻ����������                0.00
- ���ճ��𣺡�������                0.00
+ ������𣺡�������                0.00
------------------------------------------
= ��̬Ȩ�棺��������      500,000,000.00
+ ƽ��ӯ������������                0.00
+ �ֲ�ӯ������������                0.00
- �����ѣ�����������                0.00
------------------------------------------
= ��̬Ȩ�棺��������      500,000,000.00
- ռ�ñ�֤�𣺡�����                0.00
- ���ᱣ֤�𣺡�����                0.00
- ���������ѣ�������                0.00
- ���֤�𣺡�����                0.00
+ ���ý���������                0.00
------------------------------------------
= �����ʽ𣺡�������      500,000,000.00
==========================================

==========================================
  �����ʽ𣺡�������                0.00
  ��ȡ�ʽ𣺡�������      500,000,000.00
==========================================
*/

///��Լ
struct InstrumentField
{
	///��Լ����
	string	InstrumentID;
	///��Լ����
	string	InstrumentName;
	///����������
	string	ExchangeID;

	///��Լ��������
	TThostFtdcVolumeMultipleType	VolumeMultiple;
	///��С�䶯��λ
	TThostFtdcPriceType	PriceTick;
	
	// ��֤����-------------------------
	///��ͷ��֤����
	TThostFtdcRatioType	LongMarginRatio;
	///��ͷ��֤����
	TThostFtdcRatioType	ShortMarginRatio;

	// ��������---------------------------
	///����������
	TThostFtdcRatioType	OpenRatioByVolume;
	///ƽ��������
	TThostFtdcRatioType	CloseRatioByVolume;
	///ƽ��������
	TThostFtdcRatioType	CloseTodayRatioByVolume;


	///������������
	TThostFtdcRatioType	OpenRatioByMoney;
	///ƽ����������
	TThostFtdcRatioType	CloseRatioByMoney;
	///ƽ����������
	TThostFtdcRatioType	CloseTodayRatioByMoney;

	///��ǰ�Ƿ���
	TThostFtdcBoolType	IsTrading;
};

///��Լ��������
struct CommissionRateField
{
	
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;

	///����������
	TThostFtdcRatioType	OpenRatioByVolume;
	///ƽ��������
	TThostFtdcRatioType	CloseRatioByVolume;
	///ƽ��������
	TThostFtdcRatioType	CloseTodayRatioByVolume;


	///������������
	TThostFtdcRatioType	OpenRatioByMoney;
	///ƽ����������
	TThostFtdcRatioType	CloseRatioByMoney;
	///ƽ����������
	TThostFtdcRatioType	CloseTodayRatioByMoney;
	
};


///�������
struct QuoteField
{

	//����ʱ��  yyyy-mm-dd hh:mm:ss.000
    string TradingTime;  

	///��Լ����
	string	InstrumentID;
	///����������
	string	ExchangeID;
	///��Լ�ڽ������Ĵ���
	//TThostFtdcExchangeInstIDType	ExchangeInstID;
	
	///�ϴν����
	TThostFtdcPriceType	PreSettlementPrice;
	///������
	TThostFtdcPriceType	PreClosePrice;
	///��ֲ���
	TThostFtdcLargeVolumeType	PreOpenInterest;
	///����
	TThostFtdcPriceType	OpenPrice;
	///��߼�
	TThostFtdcPriceType	HighestPrice;
	///��ͼ�
	TThostFtdcPriceType	LowestPrice;
	///���¼�
	TThostFtdcPriceType	LastPrice;

	///����
	TThostFtdcVolumeType	Volume;
	///�ɽ����
	TThostFtdcMoneyType	Turnover;
	///�ֲ���
	TThostFtdcLargeVolumeType	OpenInterest;

	
	// ����
    double AddPosition;  
	// ����
    double OpenPosition;
	// ƽ��
	double ClosePosition;
	// �ɽ�����
	string TradingType;  // �࿪���տ�����ƽ����ƽ���໻���ջ���˫����˫ƽ��

	// ����
	string BuySell;  // ������ B/S

	///��ͣ���
	//TThostFtdcPriceType	UpperLimitPrice;
	///��ͣ���
	//TThostFtdcPriceType	LowerLimitPrice;

	///����޸�ʱ��
	//TThostFtdcTimeType	UpdateTime;
	/////����޸ĺ���
	//TThostFtdcMillisecType	UpdateMillisec;

	///�����һ
	TThostFtdcPriceType	BidPrice1;
	///������һ
	TThostFtdcVolumeType	BidVolume1;
	///������һ
	TThostFtdcPriceType	AskPrice1;
	///������һ
	TThostFtdcVolumeType	AskVolume1;

	///������
	TThostFtdcPriceType	ClosePrice;
	///���ν����
	TThostFtdcPriceType	SettlementPrice;


};

///�������
struct DailyQuoteField
{

	//������  yyyy-mm-dd
    string TradingDate;  

	///��Լ����
	string	InstrumentID;
	///����������
	string	ExchangeID;
	
	///����
	TThostFtdcPriceType	OpenPrice;
	///��߼�
	TThostFtdcPriceType	HighestPrice;
	///��ͼ�
	TThostFtdcPriceType	LowestPrice;
	///���¼�
	TThostFtdcPriceType	LastPrice;

	///����
	TThostFtdcVolumeType	Volume;
	///�ɽ����
	TThostFtdcMoneyType	Turnover;
	///�ֲ���
	TThostFtdcLargeVolumeType	OpenInterest;
	
};


// ָ��ṹ
struct IndicatorField
{
	//����ʱ��  yyyy-mm-dd hh:mm:ss.000
    string TradingTime;  

	///��Լ����
	string	InstrumentID;

	///���¼�
	TThostFtdcPriceType	LastPrice;

	///�����һ
	TThostFtdcPriceType	BidPrice1;
	///������һ
	TThostFtdcVolumeType	BidVolume1;
	///������һ
	TThostFtdcPriceType	AskPrice1;
	///������һ
	TThostFtdcVolumeType	AskVolume1;

	double Avg10;
	double Avg120;
	double Avg240;
	double AvgDay;

	double MaxPrice; // ���տ�����������߼�
	double Max10;
	double Max120;

	double MinPrice; // ���տ�����������ͼ�
	double Min10;
	double Min120;


	string BuySell;

};



///����
struct OrderField
{

	///Ͷ���ߴ���
	string	InvestorID;

	///����ID
	string StrategyID;

	///��Լ����
	string	InstrumentID;

	///ί��ʱ��
	string	InsertTime;  // yyyy-mm-dd hh:mm:ss.000

	///��������
	long	OrderRef;

	///ƽ��ID
	long OpenOrderRef;  // = TradeID

	///��Ͽ�ƽ��־
	string	CombOffsetFlag;

	///��������
	string	Direction;
	
	///�����۸�
	double	OrderPrice;
	///��������
	int	OrderVolume;
	///�������
	double OrderAmount;

	///����ı�֤��
	TThostFtdcMoneyType	FrozenMargin;
	///�����������
	TThostFtdcMoneyType	FrozenCommission;

	///ֹ���
	double	StopPrice;
	
	///����״̬
	int	OrderStatus;  // 1-δ�ɽ���0-�ѳɽ���-1-�ѳ���
	
	///״̬��Ϣ
	string	StatusMsg;

	///����ʱ��
	string	CancelTime;

	///�ɽ�ʱ��
	string	UpdateTime;
	
	///�ɽ��۸�
	double	Price;
	///�ɽ�����
	int	Volume;
	///�ɽ����
	double Amount;

	///��֤��
	TThostFtdcMoneyType	Margin;
	///������
	TThostFtdcMoneyType	Commission;

	// ƽ��ӯ��(ֻ�е�ƽ��֮��Ÿ���ֵ)
	double CloseProfit;
	
};



///�ɽ�
struct TradeField
{
	///Ͷ���ߴ���
	string	InvestorID;

	///����ID
	string StrategyID;

	///��Լ����
	string	InstrumentID;

	///�ɽ�ʱ��
	string	TradeTime;

	///�ɽ����
	long	TradeID;  //= OrderRef

	///��������
	string	Direction;
	
	///��ƽ��־
	string	OffsetFlag;

	///�۸�
	TThostFtdcPriceType	Price;
	///����
	TThostFtdcVolumeType	Volume;
	///���
	double Amount;
	///��֤��
	TThostFtdcMoneyType	Margin;
	///������
	TThostFtdcMoneyType	Commission;

	///ֹ���
	double	StopPrice;

	
};


///Ͷ���ֲ߳���ϸ
struct PositionDetailField
{
	///Ͷ���ߴ���
	string	InvestorID;

	///����ID
	string StrategyID;

	// �ɽ�ʱ��
	string TradingTime;

	///��Լ����
	string	InstrumentID;
	
	///�ɽ����
	long	OrderRef;

	///����
	string	Direction;
	///��������
	string	OpenDate;
		
	///����
	TThostFtdcVolumeType Volume;

	///�ɽ����
	TThostFtdcMoneyType	Amount;
	
	///���ּ�
	TThostFtdcPriceType	OpenPrice;

	///Ͷ���߱�֤��
	TThostFtdcMoneyType	Margin;

	///ֹ���
	double	StopPrice;  // ����ֹ��۸�

	/// ƽ��ʱ��
	string CloseTime; 
	///ƽ����
	TThostFtdcVolumeType CloseVolume;
	///ƽ�ֽ��
	TThostFtdcMoneyType	CloseAmount;

	///ƽ��ӯ��
	double CloseProfit;

	///��ƽ����
	int	LeftVolume;

};


#endif