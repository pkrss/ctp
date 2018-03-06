#pragma once
#include "structs.h"
#include "FQuote.h"
#include "FAccount.h"


class FStrategy
{
public:
	
	FStrategy(string stragetyId);
	~FStrategy(void);

	//// ����FQuote���newInstrument
	//void UpdateNewInstrument(const InstrumentField& newInstrument);


	// ��������ָ�ִ꣬�н��ײ���
	void ExecStrategy(const IndicatorField& newIndicator);
	
	 // ���ԣ�R-Breaker Strategy
	void Strategy_R_Breaker(const IndicatorField& newIndicator);

	// ���ԣ�˫���߲���
	void Strategy_Two_MA(const IndicatorField& newIndicator);

	// ���ԣ�OpenPrice Breaker
	void Strategy_OpenPriceBreaker(const IndicatorField& newIndicator);

	// ���ԣ���ֹͬӯֹ����ȣ������Ʒ������ʤ��
	void Strategy_StopLossProfit(const IndicatorField& newIndicator);


	 // ���ɿ��ֱ���
	void CreateOpenOrder(const IndicatorField& newIndicator, string buySell,OrderField& order); 

	// ����ƽ�ֱ���
	void CreateCloseOrder(const IndicatorField& newIndicator,const PositionDetailField& positionDetail, OrderField& order); 

	// ����ǿƽ����
	void CreateForceCloseOrder(const PositionDetailField& positionDetail,OrderField& order) ;
		
	// ����������vector,�������֡�ƽ�֣�����������
	void InsertVecOrders(const OrderField& order);

	// ��������,�������ȣ����������֡�ƽ��
	void OrderAction(OrderField order,QuoteField& quote);

	// ��ϳɽ����ж϶����Ƿ���Գɽ�
	void MatchOrders(const IndicatorField& newIndicator,const string priceType); // priceType: "NEW"��"CONTERPARTY"
	
	// ��ϳɽ�
	//void MatchOrders(const IndicatorField& newIndicator,OrderField& order,const string buySell,const double price); // priceType: "NEW"��"CONTERPARTY"

	// ��ϳɽ������ֵ�
	void MatchOpenOrders(const IndicatorField& newIndicator,OrderField& order,const double price); // priceType: "NEW"��"CONTERPARTY"
	// ��ϳɽ���ƽ�ֵ�
	void MatchCloseOrders(const IndicatorField& newIndicator,OrderField& order,const double price); // priceType: "NEW"��"CONTERPARTY"


	// �������������֮�����������ɽ������ϳ���
	void CancelOrders(const IndicatorField& newIndicator); 
	
	 // �޶�ʱ��ﵽʱ��ǿ��ƽ�ֵ������гֲ֣�
	void ForceClose();


	// ����ƽ��ί��֮�󣬸��³ֲ���ϸ
	void UpdatePositionDetailByOrder(const OrderField& order);

	// ���ֳɽ�֮�����ɳֲ���ϸ
	void InsertPositionDetail(const OrderField& trade);
	// ƽ�ֳɽ�֮�󣬸��³ֲ���ϸ
	void UpdatePositionDetailByTrade(const OrderField& trade);

	// �����������飬�����ֲֺ�Լ��ֹӯֹ��۸�
	void UpdateStopPrice(const IndicatorField& newIndicator);
	// �����������飬�����ֲֺ�Լ��ֹӯֹ��۸�,������һ�Σ�Ŀ�Ŀ��ƽ�����ʧ
	void FStrategy::UpdateStopPriceFirst(const IndicatorField& newIndicator);


	// ���ݳɽ�֮���ί�е������ɳֲ���ϸ
	void CreatePositionDetail(const OrderField& trade, PositionDetailField& positionDetail);
	// ���ֲ���ϸ����vector
	void InsertVecPositionDetails(const PositionDetailField& positionDetail);

	// ��ȡ ĳ��Լ ��δ�ɽ���ȫ��ί�е������������֡�ƽ�֣�
	int GetInstOrderNum(const string instrumentId);
	// ��ȡ ĳ��Լ ��δ�ɽ��ġ����֡�ί�е�����
	int GetInstOpenOrderNum(const string instrumentId);
	// ��ȡ ĳ��Լ ��δ�ɽ��ġ�ƽ�֡�ί�е�����
	int GetInstCloseOrderNum(const string instrumentId);


	// ��ȡ ȫ����Լ ��δ�ɽ��ġ����֡�ί�е�����
	int GetOpenOrderNum();

	// ��ȡ ĳ��Լ�ĳֲ�����,volumeType: VOLUME��LEFTVOLUME
	int GetInstPositionNum(const string instrumentId,const string volumeType);

	// ��ȡ ĳ��Լ�Ķ�ͷ�ֲ�����,volumeType: VOLUME��LEFTVOLUME
	int GetInstLongPositionNum(const string instrumentId,const string volumeType);

	// ��ȡ ĳ��Լ�Ŀ�ͷ�ֲ�����,volumeType: VOLUME��LEFTVOLUME
	int GetInstShortPositionNum(const string instrumentId,const string volumeType);

	// ��ȡ ȫ����Լ�ĳֲ�����
	int GetPositionNum();

	// ��ȡ ĳ��Լ�Ŀ�ƽ������
	int GetInstLeftPositionNum(const string instrumentId);
	//// ��ȡ ȫ����Լ�Ŀ�ƽ������
	//int FStrategy::GetLeftPositionNum();

	// ��ȡĳ��Լ�ĳֲ�ӯ��
	double GetInstPositionProfit(const string instrumentId);
	// ��ȡȫ����Լ�ĳֲ�ӯ��
	double GetPositionProfit();

	// ��ȡ ȫ����Լ ����� ��ӯ�� ���ֲ�ӯ��+ƽ��ӯ����
	double GetProfit();

	// ����ƽ��ӯ��
	double CalcCloseProfit( const OrderField& order);


	// ��ʼ�� ��Լmap
	void InitMapInstruments();

	// ������������ĺ�Լ���룬��ȡ���µĺ�Լ��Ϣ
	void GetNewInstrument(const string instrumentId, InstrumentField& newInstrument);

	// ��ȡ ��Լ����
	int GetInstVolumeMultiple(const string instrumentId);

	// ����FQuote �����
	void SetFQuote(FQuote *pQuote);

	// ����FAccount �����
	void SetFAccount(FAccount *pAccount);

	// ��vecOrders�����log�ļ�
	void PrintVecOrders();
	// ��vecPositionDetails�����log�ļ�
	void PrintVecPositionDetails();
	
	// ÿ�ս��׼�ӯ������
	void DailyStat(const string TradingDate);


	// ��ȡϵͳ��ǰʱ��ĺ�����
	string GetNowMillisecond(void);

public:

	string StrategyID;
		
	StrategyField Strategy;			// ������Ϣ

	string DataFile;	// �����ļ���Դ
	long NextOrderRef;	// ��һ����������

	InstrumentField newInstrument;		// ���������Ӧ�ĺ�Լ��Ϣ
	OrderField newOrder; // ���±���

	//�������������浱ǰ���к�Լ�ı���
	vector<OrderField> vecOrders;

	//�ֲ���ϸ���������浱ǰ���к�Լ�ĳֲ���ϸ
	vector<PositionDetailField> vecPositionDetails;

	map<string, InstrumentField> mapInstruments;			// ��Լmap
	//MapCommissionRateType mapCommissionRates;	// ��������map

	string preDirection;   // UP,DOWN
	string newDirection;   // UP,DOWN
	int dayTradingNum;     // �ս��ױ���
	int dayTradesNum;

private:

	FQuote*   pFQuotes;   // ָ�����ָ��
	FAccount* pFAccount; // ָ�����ָ��

	
};

