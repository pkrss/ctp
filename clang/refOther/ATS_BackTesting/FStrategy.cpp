#include "FStrategy.h"
#include <Windows.h>
#include <iomanip>

FStrategy::FStrategy(string stragetyId)
{
	StrategyID = stragetyId;

	// ��ʼ�� ���Բ���
	Strategy.BSQty = 1;						// ÿ�������ĺ�Լ����
	Strategy.ModifyRange = 10;				// ����ֹ�𣬵����ķ�ֵ
	Strategy.ModifyRatio = 0.1;				// ����ֹ�𣬵����ķ���
	Strategy.Rongrendu = 2.5;				// ���ֺ�ƽ��ʱ�ļ۲� ���̶�
	Strategy.TradeBegTime = "9:00:02";		// ÿ����Կ�ʼִ�е���ʼʱ��
	Strategy.ForceCloseTime = "14:59:00";	// ÿ��ǿƽʱ�䣬��14:58:00

	NextOrderRef = 101 ;					// ��һ����������
	DataFile = "SCrb0001_201401.csv";

	preDirection = "";   // UP,DOWN
	newDirection = "";   // UP,DOWN

	dayTradesNum = 50;  // ÿ�ս��ױ�������

	// ���ô�����������Ƶ������
	vecOrders.reserve(1000); 

}


FStrategy::~FStrategy(void)
{
}

// ��ʼ�� ��Լmap
void FStrategy::InitMapInstruments()
{
	string instrumentId;
	InstrumentField instrumentInfo;


	// ---- ��Լ1 ----------------------------------------------------------//
	//��Լ���� ------------------------------------
	instrumentId = "rb1405";
	
	// ��Լ��Ϣ-----------------------------------
	///��Լ����
	instrumentInfo.InstrumentID = "rb1405";
	///��Լ����
	instrumentInfo.InstrumentName = "rb1405";
	///����������
	instrumentInfo.ExchangeID = "SHFE";
	///��Լ��������
	instrumentInfo.VolumeMultiple = 10;
	///��С�䶯��λ
	instrumentInfo.PriceTick = 1;
	
	// ��֤����---------------------
	///��ͷ��֤����
	instrumentInfo.LongMarginRatio = 0.09;
	///��ͷ��֤����
	instrumentInfo.ShortMarginRatio = 0.09;

	// ��������---------------------
	///����������
	instrumentInfo.OpenRatioByVolume = 0;
	///ƽ��������
	instrumentInfo.CloseRatioByVolume = 0;
	///ƽ��������
	instrumentInfo.CloseTodayRatioByVolume = 0;

	///������������
	instrumentInfo.OpenRatioByMoney = 0.000045 ; //0.000054;
	///ƽ����������
	instrumentInfo.CloseRatioByMoney = 0.000045 ; //0.000054;
	///ƽ����������
	instrumentInfo.CloseTodayRatioByMoney = 0;

	///��ǰ�Ƿ���
	instrumentInfo.IsTrading = 1;

	
	// ��Լ��Ϣ����map
	mapInstruments.insert(map<string, InstrumentField>::value_type(instrumentId, instrumentInfo));


	// ---- ��Լ2 ----------------------------------------------------------//
	//��Լ���� ------------------------------------
	instrumentId = "rb1410";
	
	// ��Լ��Ϣ-----------------------------------
	///��Լ����
	instrumentInfo.InstrumentID = "rb1410";
	///��Լ����
	instrumentInfo.InstrumentName = "rb1410";
	///����������
	instrumentInfo.ExchangeID = "SHFE";
	///��Լ��������
	instrumentInfo.VolumeMultiple = 10;
	///��С�䶯��λ
	instrumentInfo.PriceTick = 1;
	
	// ��֤����---------------------
	///��ͷ��֤����
	instrumentInfo.LongMarginRatio = 0.09;
	///��ͷ��֤����
	instrumentInfo.ShortMarginRatio = 0.09;

	// ��������---------------------
	///����������
	instrumentInfo.OpenRatioByVolume = 0;
	///ƽ��������
	instrumentInfo.CloseRatioByVolume = 0;
	///ƽ��������
	instrumentInfo.CloseTodayRatioByVolume = 0;

	///������������
	instrumentInfo.OpenRatioByMoney = 0.000045 ; //0.000054;
	///ƽ����������
	instrumentInfo.CloseRatioByMoney = 0.000045 ; //0.000054;
	///ƽ����������
	instrumentInfo.CloseTodayRatioByMoney = 0;

	///��ǰ�Ƿ���
	instrumentInfo.IsTrading = 1;



	// ��Լ��Ϣ����map
	mapInstruments.insert(map<string, InstrumentField>::value_type(instrumentId, instrumentInfo));


	// ---- ��Լ3 -------------------------------------------------------------------//
	//��Լ���� ------------------------------------
	instrumentId = "ru1405";
	
	// ��Լ��Ϣ-----------------------------------
	///��Լ����
	instrumentInfo.InstrumentID = "ru1405";
	///��Լ����
	instrumentInfo.InstrumentName = "ru1405";
	///����������
	instrumentInfo.ExchangeID = "SHFE";
	///��Լ��������
	instrumentInfo.VolumeMultiple = 10;
	///��С�䶯��λ
	instrumentInfo.PriceTick = 5;
	
	// ��֤����---------------------
	///��ͷ��֤����
	instrumentInfo.LongMarginRatio = 0.12;
	///��ͷ��֤����
	instrumentInfo.ShortMarginRatio = 0.12;

	// ��������---------------------
	///����������
	instrumentInfo.OpenRatioByVolume = 0;
	///ƽ��������
	instrumentInfo.CloseRatioByVolume = 0;
	///ƽ��������
	instrumentInfo.CloseTodayRatioByVolume = 0;

	///������������
	instrumentInfo.OpenRatioByMoney  = 0.00005; 
	///ƽ����������
	instrumentInfo.CloseRatioByMoney = 0.00005;
	///ƽ����������
	instrumentInfo.CloseTodayRatioByMoney = 0;

	///��ǰ�Ƿ���
	instrumentInfo.IsTrading = 1;



	// ��Լ��Ϣ����map
	mapInstruments.insert(map<string, InstrumentField>::value_type(instrumentId, instrumentInfo));

	// ---- ��Լ4 -------------------------------------------------------------------//
	//��Լ���� ------------------------------------
	instrumentId = "ru1409";
	
	// ��Լ��Ϣ-----------------------------------
	///��Լ����
	instrumentInfo.InstrumentID = "ru1409";
	///��Լ����
	instrumentInfo.InstrumentName = "ru1409";
	///����������
	instrumentInfo.ExchangeID = "SHFE";
	///��Լ��������
	instrumentInfo.VolumeMultiple = 10;
	///��С�䶯��λ
	instrumentInfo.PriceTick = 5;
	
	// ��֤����---------------------
	///��ͷ��֤����
	instrumentInfo.LongMarginRatio = 0.12;
	///��ͷ��֤����
	instrumentInfo.ShortMarginRatio = 0.12;

	// ��������---------------------
	///����������
	instrumentInfo.OpenRatioByVolume = 0;
	///ƽ��������
	instrumentInfo.CloseRatioByVolume = 0;
	///ƽ��������
	instrumentInfo.CloseTodayRatioByVolume = 0;

	///������������
	instrumentInfo.OpenRatioByMoney  = 0.00005; 
	///ƽ����������
	instrumentInfo.CloseRatioByMoney = 0.00005;
	///ƽ����������
	instrumentInfo.CloseTodayRatioByMoney = 0;

	///��ǰ�Ƿ���
	instrumentInfo.IsTrading = 1;



	// ��Լ��Ϣ����map
	mapInstruments.insert(map<string, InstrumentField>::value_type(instrumentId, instrumentInfo));


	return;
}



// ����FQuote �����
void FStrategy::SetFQuote(FQuote *pQuote)
{
	pFQuotes = pQuote;
}

// ����FAccount �����
void FStrategy::SetFAccount(FAccount *pAccount)
{
	pFAccount = pAccount;
}

//// ����FQuote���newInstrument
//void FStrategy::UpdateNewInstrument(const InstrumentField& newInstrument1)
//{
//	//���µ�ǰ���������Ӧ�ĺ�Լ��Ϣ
//	memset(&newInstrument,0,sizeof(InstrumentField));
//	memcpy(&newInstrument, &newInstrument1,sizeof(InstrumentField));
//
//}


// �����������飬ִ�н��ײ���
void FStrategy::ExecStrategy(const IndicatorField& newIndicator)
{

	// R-Breaker Strategy
	//Strategy_R_Breaker(newIndicator);

	//Strategy_Two_MA(newIndicator);

	Strategy_StopLossProfit(newIndicator);


}




// ���ԣ�R-Breaker Strategy
 void FStrategy::Strategy_R_Breaker(const IndicatorField& newIndicator)
 {

	 static OrderField order;
	 
	 // ��ʼ�����±���
	 //memset(&newOrder,0,sizeof(newOrder));

	 //memset(&pairOrders, 0, sizeof(pairOrders));	


	 // ==== ƽ���жϣ�=====================================================//
	 //---- ��ͷƽ�� ----------
	 if ( (newIndicator.Avg10 <= newIndicator.AvgDay - Strategy.Rongrendu * newInstrument.PriceTick) && newIndicator.AvgDay > 0
		 && ( GetInstLongPositionNum(newIndicator.InstrumentID,"LEFTVOLUME") >= 1 ) // ���ڿ�ƽ�ֶ�ͷʱ
		 )   
	 {
		 vector<PositionDetailField>::iterator iter;
		 for(iter = vecPositionDetails.begin(); iter != vecPositionDetails.end(); iter++)
		 {
			 if(iter->InstrumentID == newIndicator.InstrumentID && iter->Direction == "BUY" && iter->LeftVolume >= 1 )
			 {

				 // ----��ͷ������ ƽ�� ----------
					 // ��ձ���
					  memset(&order,0,sizeof(order));
					 // ���ɱ���
					  CreateCloseOrder(newIndicator,*iter,order ); 
					// �ύ����,�������֡�ƽ��
					 InsertVecOrders(order);
				
			 }  // InstrumentID ==

		 } // for



	 }  // �����жϣ���ͷ
	 
	else   // Avg10 >= AvgDay + Rongrendu
		  // ---- ��ͷƽ�� -----
		 if ( (newIndicator.Avg10 >= newIndicator.AvgDay + Strategy.Rongrendu * newInstrument.PriceTick) && newIndicator.Avg10 > 0
			 && ( GetInstShortPositionNum(newIndicator.InstrumentID,"LEFTVOLUME") >= 1 ) // ���ڿ�ƽ�ֿ�ͷʱ
			 )
		 {
			 vector<PositionDetailField>::iterator iter;
			 for(iter = vecPositionDetails.begin(); iter != vecPositionDetails.end(); iter++)
			 {
				 if(iter->InstrumentID == newIndicator.InstrumentID && iter->Direction == "SELL" && iter->LeftVolume >= 1 )
				 {

					 // ----��ͷ������ ƽ�� ----------
						 // ��ձ���
						  memset(&order,0,sizeof(order));
						 // ���ɱ���
						  CreateCloseOrder(newIndicator,*iter,order); 
						// �ύ����,�������֡�ƽ��
						 InsertVecOrders(order);
				
				 }  // InstrumentID ==

			 } // for

		 }  // �����жϣ���ͷ


	 // ==== �ɽ��жϣ�=====================================================//
	 if ( GetInstOrderNum(newIndicator.InstrumentID) >= 1 )
   	 {
		 // ��ϳɽ����ж϶����Ƿ���Գɽ�
		 MatchOrders(newIndicator,"CONTERPARTY"); // priceType:���¼� "NEW"�����ּ�"CONTERPARTY"

	 }

	 // ==== �����жϣ�=====================================================//
	 if ( GetInstOrderNum(newIndicator.InstrumentID) >= 1 )
   	 {
		 // �������������֮�����������ɽ������ϳ���
		 CancelOrders(newIndicator); 

	 }


	 // ==== ֹ���жϣ�=====================================================//
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
						 // ----��ͷ������ ƽ�� ----------

						 // ��ձ���
						  memset(&order,0,sizeof(order));
						 // ���ɱ���
						  CreateCloseOrder(newIndicator,*iter,order); 
						// �ύ����,�������֡�ƽ��
						 InsertVecOrders(order);

					 } // price
				 }    // BUY
				 else // "SELL"
				 {

					 if(newIndicator.LastPrice >= (iter->StopPrice + Strategy.Rongrendu * newInstrument.PriceTick) )
					 {
						 // ----��ͷ������ ƽ�� ----------

						 // ��ձ���
						  memset(&order,0,sizeof(order));
						 // ���ɱ���
						 CreateCloseOrder(newIndicator,*iter,order); 
						// �ύ����,�������֡�ƽ��
						 InsertVecOrders(order);

					 } // price

				 }  // "SELL"

			 }  // InstrumentID ==

		 } // for

	 }  // ֹ��
	 

	 // ==== ��ֹ̬����� =================================================//
	 if( GetInstPositionNum(newIndicator.InstrumentID,"VOLUME") >= 1 )
	 {
		 UpdateStopPrice(newIndicator);

	 }

	 // ==== ǿ��ƽ�� =======================================================//
	 if(newIndicator.TradingTime.substr(11,8) > Strategy.ForceCloseTime)
	 {
		 // �޶�ʱ��ﵽʱ��ǿ��ƽ�ֵ������гֲ֣�
		 ForceClose();

	 }

	 string strX = newIndicator.TradingTime.substr(11,8);

	 // ==== �����жϣ�=====================================================//
	 //---- ��ͷ ----------
	 if (newIndicator.Avg10 >= (newIndicator.AvgDay + Strategy.Rongrendu * newInstrument.PriceTick) && newIndicator.AvgDay > 0 )   
	 {
		 // ���Է���
		 newDirection = "UP";

		 if( ( GetInstOpenOrderNum(newIndicator.InstrumentID) + GetInstPositionNum(newIndicator.InstrumentID,"VOLUME") < 1 ) // ί�е����ֲ�������
		       && newIndicator.TradingTime.substr(11,8) <= Strategy.ForceCloseTime
		       && newDirection != preDirection	&& dayTradingNum < dayTradesNum )
		 {
		    // ���ɱ���
			 CreateOpenOrder(newIndicator,"BUY",order);
			
			 // �ύ����
			 InsertVecOrders(order);

			 // ������Է���
			 preDirection = newDirection;

			 // ���ս��ױ��� + 1
			 dayTradingNum += 1;


		 } // �ֲ��ж�


	 }  // �����жϣ���ͷ

	
	else   // <=
		  // ---- ��ͷ -----
		 if (newIndicator.Avg10 > 0 && newIndicator.Avg10 <= (newIndicator.AvgDay - Strategy.Rongrendu * newInstrument.PriceTick) )
		 {
			 // ���Է���
			 newDirection = "DOWN";

			 if( ( GetInstOpenOrderNum(newIndicator.InstrumentID) + GetInstPositionNum(newIndicator.InstrumentID,"VOLUME") < 1 ) // ί�е����ֲ�������
				&& newIndicator.TradingTime.substr(11,8) <= Strategy.ForceCloseTime 
				&& newDirection != preDirection  && dayTradingNum < dayTradesNum )
			 {
				 // ���ɱ���
				CreateOpenOrder(newIndicator,"SELL",order);

				 // �ύ����
				 InsertVecOrders(order);

				 // ������Է���
				preDirection = newDirection;

				// ���ս��ױ��� + 1
			    dayTradingNum += 1;


			 } // �ֲ��ж�

		 }  // �����жϣ���ͷ



	 return;

 }

 // ���ԣ�˫���߲���
 void FStrategy::Strategy_Two_MA(const IndicatorField& newIndicator)
 {

	 static OrderField order;
	 
	 // ��ʼ�����±���
	 //memset(&newOrder,0,sizeof(newOrder));

	 //memset(&pairOrders, 0, sizeof(pairOrders));	


	 // ==== ƽ���жϣ�=====================================================//
	 //---- ��ͷƽ�� ----------
	 if ( (newIndicator.Avg120 <= newIndicator.Avg240 - Strategy.Rongrendu * newInstrument.PriceTick) && newIndicator.Avg240 > 0
		 && ( GetInstLongPositionNum(newIndicator.InstrumentID,"LEFTVOLUME") >= 1 ) // ���ڿ�ƽ�ֶ�ͷʱ
		 )   
	 {
		 vector<PositionDetailField>::iterator iter;
		 for(iter = vecPositionDetails.begin(); iter != vecPositionDetails.end(); iter++)
		 {
			 if(iter->InstrumentID == newIndicator.InstrumentID && iter->Direction == "BUY" && iter->LeftVolume >= 1 )
			 {

				 // ----��ͷ������ ƽ�� ----------
					 // ��ձ���
					  memset(&order,0,sizeof(order));
					 // ���ɱ���
					  CreateCloseOrder(newIndicator,*iter,order ); 
					// �ύ����,�������֡�ƽ��
					 InsertVecOrders(order);
				
			 }  // InstrumentID ==

		 } // for



	 }  // �����жϣ���ͷ
	 
	else   // Avg10 >= AvgDay + Rongrendu
		  // ---- ��ͷƽ�� -----
		 if ( (newIndicator.Avg120 >= newIndicator.Avg240 + Strategy.Rongrendu * newInstrument.PriceTick) && newIndicator.Avg240 > 0
			 && ( GetInstShortPositionNum(newIndicator.InstrumentID,"LEFTVOLUME") >= 1 ) // ���ڿ�ƽ�ֿ�ͷʱ
			 )
		 {
			 vector<PositionDetailField>::iterator iter;
			 for(iter = vecPositionDetails.begin(); iter != vecPositionDetails.end(); iter++)
			 {
				 if(iter->InstrumentID == newIndicator.InstrumentID && iter->Direction == "SELL" && iter->LeftVolume >= 1 )
				 {

					 // ----��ͷ������ ƽ�� ----------
						 // ��ձ���
						  memset(&order,0,sizeof(order));
						 // ���ɱ���
						  CreateCloseOrder(newIndicator,*iter,order); 
						// �ύ����,�������֡�ƽ��
						 InsertVecOrders(order);
				
				 }  // InstrumentID ==

			 } // for

		 }  // �����жϣ���ͷ


	 // ==== �ɽ��жϣ�=====================================================//
	 if ( GetInstOrderNum(newIndicator.InstrumentID) >= 1 )
   	 {
		 // ��ϳɽ����ж϶����Ƿ���Գɽ�
		 MatchOrders(newIndicator,"CONTERPARTY"); // priceType:���¼� "NEW"�����ּ�"CONTERPARTY"

	 }

	 // ==== �����жϣ�=====================================================//
	 if ( GetInstOrderNum(newIndicator.InstrumentID) >= 1 )
   	 {
		 // �������������֮�����������ɽ������ϳ���
		 CancelOrders(newIndicator); 

	 }


	 // ==== ֹ���жϣ�=====================================================//
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
						 // ----��ͷ������ ƽ�� ----------

						 // ��ձ���
						  memset(&order,0,sizeof(order));
						 // ���ɱ���
						  CreateCloseOrder(newIndicator,*iter,order); 
						// �ύ����,�������֡�ƽ��
						 InsertVecOrders(order);

					 } // price
				 }    // BUY
				 else // "SELL"
				 {

					 if(newIndicator.LastPrice >= (iter->StopPrice + Strategy.Rongrendu * newInstrument.PriceTick) )
					 {
						 // ----��ͷ������ ƽ�� ----------

						 // ��ձ���
						  memset(&order,0,sizeof(order));
						 // ���ɱ���
						 CreateCloseOrder(newIndicator,*iter,order); 
						// �ύ����,�������֡�ƽ��
						 InsertVecOrders(order);

					 } // price

				 }  // "SELL"

			 }  // InstrumentID ==

		 } // for

	 }  // ֹ��
	 

	 // ==== ��ֹ̬����� =================================================//
	 if( GetInstPositionNum(newIndicator.InstrumentID,"VOLUME") >= 1 )
	 {
		 UpdateStopPrice(newIndicator);

	 }

	 // ==== ǿ��ƽ�� =======================================================//
	 if(newIndicator.TradingTime.substr(11,8) > Strategy.ForceCloseTime)
	 {
		 // �޶�ʱ��ﵽʱ��ǿ��ƽ�ֵ������гֲ֣�
		 ForceClose();

	 }

	 string strX = newIndicator.TradingTime.substr(11,8);

	 // ==== �����жϣ�=====================================================//
	 //---- ��ͷ ----------
	 if (newIndicator.Avg240 > 0 && newIndicator.Avg120 >= (newIndicator.Avg240 + Strategy.Rongrendu * newInstrument.PriceTick)  )   
	 {
		 // ���Է���
		 newDirection = "UP";

		 if( ( GetInstOpenOrderNum(newIndicator.InstrumentID) + GetInstPositionNum(newIndicator.InstrumentID,"VOLUME") < 1 ) // ί�е����ֲ�������
		       && newIndicator.TradingTime.substr(11,8) <= Strategy.ForceCloseTime
		       && newDirection != preDirection	&& dayTradingNum < dayTradesNum )
		 {
		    // ���ɱ���
			 CreateOpenOrder(newIndicator,"BUY",order);
			
			 // �ύ����
			 InsertVecOrders(order);

			 // ������Է���
			 preDirection = newDirection;

			 // ���ս��ױ��� + 1
			 dayTradingNum += 1;

		 } // �ֲ��ж�


	 }  // �����жϣ���ͷ

	
	else   // <=
		  // ---- ��ͷ -----
		 if (newIndicator.Avg240 > 0 && newIndicator.Avg120 <= (newIndicator.Avg240 - Strategy.Rongrendu * newInstrument.PriceTick) )
		 {
			 // ���Է���
			 newDirection = "DOWN";

			 if( ( GetInstOpenOrderNum(newIndicator.InstrumentID) + GetInstPositionNum(newIndicator.InstrumentID,"VOLUME") < 1 ) // ί�е����ֲ�������
				&& newIndicator.TradingTime.substr(11,8) <= Strategy.ForceCloseTime 
				&& newDirection != preDirection && dayTradingNum < dayTradesNum  )
			 {
				 // ���ɱ���
				CreateOpenOrder(newIndicator,"SELL",order);

				 // �ύ����
				 InsertVecOrders(order);

				 // ������Է���
				preDirection = newDirection;

				// ���ս��ױ��� + 1
			   dayTradingNum += 1;

			 } // �ֲ��ж�

		 }  // �����жϣ���ͷ



	 return;

 }



 // ���ԣ�OpenPrice Breaker
void FStrategy::Strategy_OpenPriceBreaker(const IndicatorField& newIndicator)
 {

	 static OrderField order;
	 
	 // ��ʼ�����±���
	 //memset(&newOrder,0,sizeof(newOrder));

	 //memset(&pairOrders, 0, sizeof(pairOrders));	


	 // ==== ƽ���жϣ�=====================================================//
	 //---- ��ͷƽ�� ----------
	 if ( (newIndicator.LastPrice <= pFQuotes->dayOpenPrice - Strategy.Rongrendu * newInstrument.PriceTick) 
		 && ( GetInstLongPositionNum(newIndicator.InstrumentID,"LEFTVOLUME") >= 1 ) // ���ڿ�ƽ�ֶ�ͷʱ
		 )   
	 {
		 vector<PositionDetailField>::iterator iter;
		 for(iter = vecPositionDetails.begin(); iter != vecPositionDetails.end(); iter++)
		 {
			 if(iter->InstrumentID == newIndicator.InstrumentID && iter->Direction == "BUY" && iter->LeftVolume >= 1 )
			 {

				 // ----��ͷ������ ƽ�� ----------
					 // ��ձ���
					  memset(&order,0,sizeof(order));
					 // ���ɱ���
					  CreateCloseOrder(newIndicator,*iter,order ); 
					// �ύ����,�������֡�ƽ��
					 InsertVecOrders(order);
				
			 }  // InstrumentID ==

		 } // for



	 }  // �����жϣ���ͷ
	 
	else   // Avg10 >= AvgDay + Rongrendu
		  // ---- ��ͷƽ�� -----
		  if ( (newIndicator.LastPrice >= pFQuotes->dayOpenPrice + Strategy.Rongrendu * newInstrument.PriceTick)
			 && ( GetInstShortPositionNum(newIndicator.InstrumentID,"LEFTVOLUME") >= 1 ) // ���ڿ�ƽ�ֿ�ͷʱ
			 )
		 {
			 vector<PositionDetailField>::iterator iter;
			 for(iter = vecPositionDetails.begin(); iter != vecPositionDetails.end(); iter++)
			 {
				 if(iter->InstrumentID == newIndicator.InstrumentID && iter->Direction == "SELL" && iter->LeftVolume >= 1 )
				 {

					 // ----��ͷ������ ƽ�� ----------
						 // ��ձ���
						  memset(&order,0,sizeof(order));
						 // ���ɱ���
						  CreateCloseOrder(newIndicator,*iter,order); 
						// �ύ����,�������֡�ƽ��
						 InsertVecOrders(order);
				
				 }  // InstrumentID ==

			 } // for

		 }  // �����жϣ���ͷ


	 // ==== �ɽ��жϣ�=====================================================//
	 if ( GetInstOrderNum(newIndicator.InstrumentID) >= 1 )
   	 {
		 // ��ϳɽ����ж϶����Ƿ���Գɽ�
		 MatchOrders(newIndicator,"CONTERPARTY"); // priceType:���¼� "NEW"�����ּ�"CONTERPARTY"

	 }

	 // ==== �����жϣ�=====================================================//
	 if ( GetInstOrderNum(newIndicator.InstrumentID) >= 1 )
   	 {
		 // �������������֮�����������ɽ������ϳ���
		 CancelOrders(newIndicator); 

	 }


	  //==== ֹ���жϣ�=====================================================//
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
		//				 // ----��ͷ������ ƽ�� ----------

		//				 // ��ձ���
		//				  memset(&order,0,sizeof(order));
		//				 // ���ɱ���
		//				  CreateCloseOrder(newIndicator,*iter,order); 
		//				// �ύ����,�������֡�ƽ��
		//				 InsertVecOrders(order);

		//			 } // price
		//		 }    // BUY
		//		 else // "SELL"
		//		 {

		//			 if(newIndicator.LastPrice >= (iter->StopPrice + Strategy.Rongrendu * newInstrument.PriceTick) )
		//			 {
		//				 // ----��ͷ������ ƽ�� ----------

		//				 // ��ձ���
		//				  memset(&order,0,sizeof(order));
		//				 // ���ɱ���
		//				 CreateCloseOrder(newIndicator,*iter,order); 
		//				// �ύ����,�������֡�ƽ��
		//				 InsertVecOrders(order);

		//			 } // price

		//		 }  // "SELL"

		//	 }  // InstrumentID ==

		// } // for

	 //}  // ֹ��
	 

	  //==== ��ֹ̬����� =================================================//
	 //if( GetInstPositionNum(newIndicator.InstrumentID,"VOLUME") >= 1 )
	 //{
		// //UpdateStopPrice(newIndicator);
		// UpdateStopPriceFirst(newIndicator);
		// 

	 //}

	 // ==== ǿ��ƽ�� =======================================================//
	 if(newIndicator.TradingTime.substr(11,8) > Strategy.ForceCloseTime
		 && GetInstPositionNum(newIndicator.InstrumentID,"LEFTVOLUME") >= 1 )
	 {
		 // �޶�ʱ��ﵽʱ��ǿ��ƽ�ֵ������гֲ֣�
		 ForceClose();

	 }


	 // ==== �����жϣ�=====================================================//
	 //---- ��ͷ ----------
	 if (pFQuotes->dayOpenPrice > 0 && newIndicator.LastPrice >= (pFQuotes->dayOpenPrice + Strategy.Rongrendu * newInstrument.PriceTick)  )   
	 {
		 // ���Է���
		 newDirection = "UP";

		 if( ( GetInstOpenOrderNum(newIndicator.InstrumentID) + GetInstPositionNum(newIndicator.InstrumentID,"VOLUME") < 1 ) // ί�е����ֲ�������
		       && newIndicator.TradingTime.substr(11,8) <= Strategy.ForceCloseTime
			   && abs(newIndicator.LastPrice - pFQuotes->dayOpenPrice) <= 2 * Strategy.Rongrendu * newInstrument.PriceTick
		       && newDirection != preDirection	&& dayTradingNum < dayTradesNum )
		 {
		    // ���ɱ���
			 CreateOpenOrder(newIndicator,"BUY",order);
			
			 // �ύ����
			 InsertVecOrders(order);

			 // ������Է���
			 preDirection = newDirection;

			 // ���ս��ױ��� + 1
			 dayTradingNum += 1;


		 } // �ֲ��ж�


	 }  // �����жϣ���ͷ

	
	else   // <=
		  // ---- ��ͷ -----
		  if (pFQuotes->dayOpenPrice > 0 && newIndicator.LastPrice <= (pFQuotes->dayOpenPrice - Strategy.Rongrendu * newInstrument.PriceTick) )
		 {
			 // ���Է���
			 newDirection = "DOWN";

			 if( ( GetInstOpenOrderNum(newIndicator.InstrumentID) + GetInstPositionNum(newIndicator.InstrumentID,"VOLUME") < 1 ) // ί�е����ֲ�������
				&& newIndicator.TradingTime.substr(11,8) <= Strategy.ForceCloseTime 
				&& abs(newIndicator.LastPrice - pFQuotes->dayOpenPrice) <= 2 * Strategy.Rongrendu * newInstrument.PriceTick
				&& newDirection != preDirection && dayTradingNum < dayTradesNum  )
			 {
				 // ���ɱ���
				CreateOpenOrder(newIndicator,"SELL",order);

				 // �ύ����
				 InsertVecOrders(order);

				 // ������Է���
				preDirection = newDirection;

				// ���ս��ױ��� + 1
			    dayTradingNum += 1;

			 } // �ֲ��ж�

		 }  // �����жϣ���ͷ



	 return;

 }




  // ���ԣ�Stop Loss and Profit of same range
void FStrategy::Strategy_StopLossProfit(const IndicatorField& newIndicator)
 {

	 static OrderField order;

	 static IndicatorField newIndicator_1;
	 
	 
	 // ��ȡT-1��ָ��
	 memset(&newIndicator_1,0,sizeof(IndicatorField));
	 pFQuotes->GetInstNewIndicator(newIndicator.InstrumentID,newIndicator_1,1);  // ��ȡT-1������ָ��

	 if ( newIndicator_1.Avg10 <=0 || newIndicator_1.Avg10 >= 100000 )
		 return;

	 // ��ʼ�����±���
	 //memset(&newOrder,0,sizeof(newOrder));

	 //memset(&pairOrders, 0, sizeof(pairOrders));	


	 // ==== ƽ���жϣ�=====================================================//
	 //---- ��ͷƽ�� ----------
	 if ( GetInstLongPositionNum(newIndicator.InstrumentID,"LEFTVOLUME") >= 1  // ���ڿ�ƽ�ֶ�ͷʱ
		  && newIndicator.Avg10 > 0 && newIndicator.Avg10 < 100000
		  && newIndicator.Avg120 > 0 && newIndicator.Avg120 < 100000
		  && newIndicator_1.Avg10 > 0 && newIndicator_1.Avg10 < 100000
		  && newIndicator.Avg10 < newIndicator.Avg120  // ƽ������
		  && newIndicator_1.Avg10 > newIndicator.Avg120 
		 )   
	 {
		 vector<PositionDetailField>::iterator iter;
		 for(iter = vecPositionDetails.begin(); iter != vecPositionDetails.end(); iter++)
		 {
			 if(iter->InstrumentID == newIndicator.InstrumentID && iter->Direction == "BUY" && iter->LeftVolume >= 1 )
			 {

				 // ----��ͷ������ ƽ�� ----------
					 // ��ձ���
					  memset(&order,0,sizeof(order));
					 // ���ɱ���
					  CreateCloseOrder(newIndicator,*iter,order ); 
					// �ύ����,�������֡�ƽ��
					 InsertVecOrders(order);
				
			 }  // InstrumentID ==

		 } // for



	 }  // ƽ���жϣ���ͷ
	 
	else   // Avg10 >= AvgDay + Rongrendu
		  // ---- ��ͷƽ�� -----
		  if ( ( GetInstShortPositionNum(newIndicator.InstrumentID,"LEFTVOLUME") >= 1 ) // ���ڿ�ƽ�ֿ�ͷʱ
			  && newIndicator.Avg10 > 0 && newIndicator.Avg10 < 100000
			  && newIndicator.Avg120 > 0 && newIndicator.Avg120 < 100000
			  && newIndicator_1.Avg10 > 0 && newIndicator_1.Avg10 < 100000
			  && newIndicator.Avg10 > newIndicator.Avg120 // ƽ������
			  && newIndicator_1.Avg10 < newIndicator.Avg120 

			 )
		 {
			 vector<PositionDetailField>::iterator iter;
			 for(iter = vecPositionDetails.begin(); iter != vecPositionDetails.end(); iter++)
			 {
				 if(iter->InstrumentID == newIndicator.InstrumentID && iter->Direction == "SELL" && iter->LeftVolume >= 1 )
				 {

					 // ----��ͷ������ ƽ�� ----------
						 // ��ձ���
						  memset(&order,0,sizeof(order));
						 // ���ɱ���
						  CreateCloseOrder(newIndicator,*iter,order); 
						// �ύ����,�������֡�ƽ��
						 InsertVecOrders(order);
				
				 }  // InstrumentID ==

			 } // for

		 }  // ƽ���жϣ���ͷ


	 // ==== �ɽ��жϣ�=====================================================//
	 if ( GetInstOrderNum(newIndicator.InstrumentID) >= 1 )
   	 {
		 // ��ϳɽ����ж϶����Ƿ���Գɽ�
		 MatchOrders(newIndicator,"CONTERPARTY"); // priceType:���¼� "NEW"�����ּ�"CONTERPARTY"

	 }

	 // ==== �����жϣ�=====================================================//
	 if ( GetInstOrderNum(newIndicator.InstrumentID) >= 1 )
   	 {
		 // �������������֮�����������ɽ������ϳ���
		 CancelOrders(newIndicator); 

	 }


	  //==== ֹ���жϣ�=====================================================//
	 if( GetInstPositionNum(newIndicator.InstrumentID,"LEFTVOLUME") >= 1 )
	 {
		vector<PositionDetailField>::iterator iter;
		 for(iter = vecPositionDetails.begin(); iter != vecPositionDetails.end(); iter++)
		 {
			 if(iter->InstrumentID == newIndicator.InstrumentID && iter->LeftVolume >= 1)
			 {
				 if(iter->Direction == "BUY") // ��ͷ
				 {
					 if(    newIndicator.LastPrice <= (iter->StopPrice - Strategy.Rongrendu * newInstrument.PriceTick)  // ֹ��
						 //|| newIndicator.LastPrice >= (iter->StopPrice + Strategy.Rongrendu * 2 * newInstrument.PriceTick)  // ֹӯ
						 || newIndicator.BidPrice1 >= (iter->StopPrice + newInstrument.PriceTick)  // ֹӯ
						)
					 {
						 // ----��ͷ������ ƽ�� ----------

						 // ��ձ���
						  memset(&order,0,sizeof(order));
						 // ���ɱ���
						  CreateCloseOrder(newIndicator,*iter,order); 
						// �ύ����,�������֡�ƽ��
						 InsertVecOrders(order);

					 } // price
				 }    // BUY
				 else // "SELL"
				 {

					 if(    newIndicator.LastPrice >= (iter->StopPrice + Strategy.Rongrendu * newInstrument.PriceTick) // ֹ��
						 //|| newIndicator.LastPrice <= (iter->StopPrice - Strategy.Rongrendu * 2 * newInstrument.PriceTick) // ֹӯ
						 || newIndicator.AskPrice1 <= (iter->StopPrice -  newInstrument.PriceTick) // ֹӯ
					    )
					 {
						 // ----��ͷ������ ƽ�� ----------

						 // ��ձ���
						  memset(&order,0,sizeof(order));
						 // ���ɱ���
						 CreateCloseOrder(newIndicator,*iter,order); 
						// �ύ����,�������֡�ƽ��
						 InsertVecOrders(order);

					 } // price

				 }  // "SELL"

			 }  // InstrumentID ==

		 } // for

	 }  // ֹ��
	 

	  //==== ��ֹ̬����� =================================================//
	 if( GetInstPositionNum(newIndicator.InstrumentID,"VOLUME") >= 1 )
	 {
		 //UpdateStopPrice(newIndicator);
		 UpdateStopPriceFirst(newIndicator);
		 

	 }

	 // ==== ǿ��ƽ�� =======================================================//
	 if(newIndicator.TradingTime.substr(11,8) > Strategy.ForceCloseTime
		 && GetInstPositionNum(newIndicator.InstrumentID,"LEFTVOLUME") >= 1 )
	 {
		 // �޶�ʱ��ﵽʱ��ǿ��ƽ�ֵ������гֲ֣�
		 ForceClose();

	 }


	 // ==== �����жϣ�=====================================================//

	 // ����1��˫����+ֹӯֹ��--------------------------------------
	 //     ���֣�
	 //         avg10�ϴ�avg120������֣�
	 //         avg10�´�avg120�����ղ֣�
	 //    ƽ�֣�
	 //         avg10�ϴ�avg120��ƽ�ղ֣�
	 //         avg10�´�avg120������֣�
	 //    ֹӯֹ��
	 //         ����2����С��λ��ֹ��
	 //         ӯ��10����С��λ��ֹӯ;
	 //    �����ɱ��ۣ�
	 //         ���¼۸����ڳɱ��۸�6����С��λ����ɱ����ϵ�3����λ


	 //---- ��ͷ ----------
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
		 // ���Է���
		 newDirection = "UP";

		 if( ( GetInstOpenOrderNum(newIndicator.InstrumentID) + GetInstPositionNum(newIndicator.InstrumentID,"VOLUME") < 1 ) // ί�е����ֲ�������
		       && newIndicator.TradingTime.substr(11,8) <= Strategy.ForceCloseTime
			   //&& abs(newIndicator.LastPrice - pFQuotes->dayOpenPrice) <= 2 * Strategy.Rongrendu * newInstrument.PriceTick  // ��dayOpenPrice�۸񸽽�
		       //&& newDirection != preDirection	
			   && dayTradingNum < dayTradesNum 
			   //&& newIndicator.LastPrice >= newIndicator.Max120  // ���¼۸�����߼۸�
			)
		 {
		    // ���ɱ���
			 CreateOpenOrder(newIndicator,"BUY",order);
			
			 // �ύ����
			 InsertVecOrders(order);

			 // ������Է���
			 preDirection = newDirection;

			 // ���ս��ױ��� + 1
			 dayTradingNum += 1;


		 } // �ֲ��ж�


	 }  // �����жϣ���ͷ

	
	else   // <=
		  // ---- ��ͷ -----
		  if (   pFQuotes->dayOpenPrice > 0 
			   && newIndicator.Avg10 > 0 && newIndicator.Avg10 < 100000
			   && newIndicator.Avg120 > 0 && newIndicator.Avg120 < 100000
			   && newIndicator_1.Avg10 > 0 && newIndicator_1.Avg10 < 100000
			   && newIndicator.Avg10 < newIndicator.Avg120 
			   && newIndicator_1.Avg10 > newIndicator.Avg120 
			   //&& newIndicator.LastPrice <= (pFQuotes->dayOpenPrice - Strategy.Rongrendu * newInstrument.PriceTick)
			  )
		 {
			 // ���Է���
			 newDirection = "DOWN";

			 if( ( GetInstOpenOrderNum(newIndicator.InstrumentID) + GetInstPositionNum(newIndicator.InstrumentID,"VOLUME") < 1 ) // ί�е����ֲ�������
				&& newIndicator.TradingTime.substr(11,8) <= Strategy.ForceCloseTime 
				//&& abs(newIndicator.LastPrice - pFQuotes->dayOpenPrice) <= 2 * Strategy.Rongrendu * newInstrument.PriceTick  // ��dayOpenPrice�۸񸽽�
				//&& newDirection != preDirection 
				&& dayTradingNum < dayTradesNum  
				//&& newIndicator.LastPrice <= newIndicator.Min120 // ���¼۸�����ͼ۸�
			   )
			 {
				 // ���ɱ���
				CreateOpenOrder(newIndicator,"SELL",order);


				 // �ύ����
				 InsertVecOrders(order);

				 // ������Է���
				preDirection = newDirection;

				// ���ս��ױ��� + 1
			    dayTradingNum += 1;

			 } // �ֲ��ж�

		 }  // �����жϣ���ͷ



	 return;

 }




 // ���ɿ��ֱ���
void FStrategy::CreateOpenOrder(const IndicatorField& newIndicator, string buySell,OrderField& order)  
{
	
	// ���� ���� ----------------------------------------
	///Ͷ���ߴ���
	order.InvestorID = pFAccount->InvestorID;

	///����ID
	order.StrategyID = StrategyID;

	///��Լ����
	order.InstrumentID =newIndicator.InstrumentID;

	///ί��ʱ��
	order.InsertTime =newIndicator.TradingTime;  // yyyy-mm-dd hh:mm:ss.000

	///��������
	order.OrderRef = NextOrderRef++;

	///ƽ��ID
	order.OpenOrderRef = 0; 

	///��Ͽ�ƽ��־
	order.CombOffsetFlag = "OPEN";

	///��������
	order.Direction = buySell;
	
	///�����۸� - ���ݶ��ּ۸�����
	if(buySell == "BUY")
		order.OrderPrice =newIndicator.AskPrice1 + newInstrument.PriceTick;  
	else
		order.OrderPrice =newIndicator.BidPrice1 - newInstrument.PriceTick;


	///��������
	order.OrderVolume = Strategy.BSQty;   // ���� ��Ĭ��������
	///�������
	order.OrderAmount = order.OrderVolume * order.OrderPrice * newInstrument.VolumeMultiple;

	///����ı�֤��
	if(buySell == "BUY")
		order.FrozenMargin = order.OrderAmount * newInstrument.LongMarginRatio;
	else
		order.FrozenMargin = order.OrderAmount * newInstrument.ShortMarginRatio;

	///�����������
	if (newInstrument.OpenRatioByMoney > 0)
		order.FrozenCommission = order.OrderAmount * newInstrument.OpenRatioByMoney;
	else if (newInstrument.OpenRatioByVolume > 0)
		order.FrozenCommission = order.OrderVolume * newInstrument.OpenRatioByVolume;

	///ֹ���
	order.StopPrice = order.OrderPrice;
	
	///����״̬
	order.OrderStatus = 1;  // 1-δ�ɽ���0-�ѳɽ���-1-�ѳ���
	
	///״̬��Ϣ
	order.StatusMsg = "δ�ɽ�";

	///����ʱ��
	order.CancelTime ="";

	///�ɽ�ʱ��
	order.UpdateTime ="";
	
	///�ɽ��۸�
	order.Price =0;
	///�ɽ�����
	order.Volume =0;
	///�ɽ����
	order.Amount =0;

	///��֤��
	order.Margin =0;
	///������
	order.Commission =0;

	// ƽ��ӯ��(ֻ�е�ƽ��֮��Ÿ���ֵ)
	order.CloseProfit = 0;

	return;

}

// ����ƽ�ֱ���
void FStrategy::CreateCloseOrder(const IndicatorField& newIndicator,const PositionDetailField& positionDetail, OrderField& order )  
{
	
	// ���� ���� ----------------------------------------
	///Ͷ���ߴ���
	order.InvestorID = pFAccount->InvestorID;

	///����ID
	order.StrategyID = StrategyID;

	///��Լ����
	order.InstrumentID =newIndicator.InstrumentID;

	///ί��ʱ��
	order.InsertTime =newIndicator.TradingTime;  // yyyy-mm-dd hh:mm:ss.000

	///��������
	order.OrderRef = NextOrderRef++;

	///ƽ��ID
	order.OpenOrderRef = positionDetail.OrderRef; 

	///��Ͽ�ƽ��־
	order.CombOffsetFlag = "CLOSE";

	///��������
	order.Direction = (positionDetail.Direction == "BUY")?"SELL":"BUY";
	
	///�����۸� - ���ݶ��ּ۸�����
	if(order.Direction == "BUY")
		order.OrderPrice =newIndicator.AskPrice1;  
	else
		order.OrderPrice =newIndicator.BidPrice1;


	///��������
	order.OrderVolume = positionDetail.LeftVolume;   // ��ƽ��ʣ��ֲ֡�
	///�������
	order.OrderAmount = order.OrderVolume * order.OrderPrice * newInstrument.VolumeMultiple;

	///����ı�֤��,ƽ�ֵ� ��֤�� = 0
	order.FrozenMargin = 0;
	
	///�����������
	if (newInstrument.CloseRatioByMoney > 0)
		order.FrozenCommission = order.OrderAmount * newInstrument.CloseRatioByMoney;
	else if (newInstrument.CloseRatioByVolume > 0)
		order.FrozenCommission = order.OrderVolume * newInstrument.CloseRatioByVolume;
	
	// ����ǽ��տ��֣� ������ƽ���������ʶ���0����ƽ��������Ϊ0��
	if(newInstrument.CloseTodayRatioByMoney == 0 && newInstrument.CloseTodayRatioByVolume == 0
		&& positionDetail.OpenDate == newIndicator.TradingTime.substr(0,10)
	  )
	{
		order.FrozenCommission = 0;
	}

	///ֹ���
	order.StopPrice = 0;
	
	///����״̬
	order.OrderStatus = 1;  // 1-δ�ɽ���0-�ѳɽ���-1-�ѳ���
	
	///״̬��Ϣ
	order.StatusMsg = "δ�ɽ�";

	///����ʱ��
	order.CancelTime ="";

	///�ɽ�ʱ��
	order.UpdateTime ="";
	
	///�ɽ��۸�
	order.Price =0;
	///�ɽ�����
	order.Volume =0;
	///�ɽ����
	order.Amount =0;

	///��֤��
	order.Margin = positionDetail.Margin; // ����Ϊ �ֲֵı�֤������ƽ��֮������֤��
	///������
	order.Commission =0;

	// ƽ��ӯ��(ֻ�е�ƽ��֮��Ÿ���ֵ)
	order.CloseProfit = 0;

	return;

}


// ����ǿƽ����
void FStrategy::CreateForceCloseOrder(const PositionDetailField& positionDetail,OrderField& order)  
{
	InstrumentField instrument1;

	GetNewInstrument(positionDetail.InstrumentID, instrument1);


	// ���� ���� ----------------------------------------
	///Ͷ���ߴ���
	order.InvestorID = positionDetail.InvestorID;

	///����ID
	order.StrategyID = positionDetail.StrategyID;

	///��Լ����
	order.InstrumentID =positionDetail.InstrumentID;

	///ί��ʱ��
	order.InsertTime =pFQuotes->newIndicator.TradingTime;  // yyyy-mm-dd hh:mm:ss.000

	///��������
	order.OrderRef = NextOrderRef++;

	///ƽ��ID
	order.OpenOrderRef = positionDetail.OrderRef; 

	///��Ͽ�ƽ��־
	order.CombOffsetFlag = "CLOSE";

	///��������
	order.Direction = (positionDetail.Direction == "BUY") ? "SELL" : "BUY";
	
	///�����۸� - ���ݶ��ּ۸�����
	if(order.Direction == "BUY")
		order.OrderPrice = pFQuotes->GetInstNewAskPrice1(order.InstrumentID);  
	else
		order.OrderPrice = pFQuotes->GetInstNewBidPrice1(order.InstrumentID); 


	///��������
	order.OrderVolume = positionDetail.Volume;
	///�������
	order.OrderAmount = order.OrderVolume * order.OrderPrice * instrument1.VolumeMultiple;

	///����ı�֤��
	order.FrozenMargin = 0;

	/*if(order.Direction == "BUY")
		order.FrozenMargin = order.OrderAmount * instrument1.LongMarginRatio;
	else
		order.FrozenMargin = order.OrderAmount * instrument1.ShortMarginRatio;*/

	
	///�����������
	if (instrument1.CloseRatioByMoney > 0)
		order.FrozenCommission = order.OrderAmount * instrument1.CloseRatioByMoney;
	else if (instrument1.CloseRatioByVolume > 0)
		order.FrozenCommission = order.OrderVolume * instrument1.CloseRatioByVolume;
	
	// ����ǽ��տ��֣� ������ƽ���������ʶ���0����ƽ��������Ϊ0��
	if(instrument1.CloseTodayRatioByMoney == 0 && instrument1.CloseTodayRatioByVolume == 0
		&& positionDetail.OpenDate == pFQuotes->newIndicator.TradingTime.substr(0,10)
	  )
	{
		order.FrozenCommission = 0;
	}

	///ֹ���
	order.StopPrice = 0;
	
	///����״̬
	order.OrderStatus = 1;  // 1-δ�ɽ���0-�ѳɽ���-1-�ѳ���
	
	///״̬��Ϣ
	order.StatusMsg = "δ�ɽ�";

	///����ʱ��
	order.CancelTime ="";

	///�ɽ�ʱ��
	order.UpdateTime ="";
	
	///�ɽ��۸�
	order.Price =0;
	///�ɽ�����
	order.Volume =0;
	///�ɽ����
	order.Amount =0;

	///��֤��
	order.Margin = positionDetail.Margin; // ����Ϊ �ֲֵı�֤������ƽ��֮������֤��
	///������
	order.Commission =0;

	// ƽ��ӯ��(ֻ�е�ƽ��֮��Ÿ���ֵ)
	order.CloseProfit = 0;

	return;

}


// �ύ����,�������֡�ƽ�֣�����������
void FStrategy::InsertVecOrders(const OrderField& order)
{
	// �ύ������vecOrders
	vecOrders.push_back(order);

	// �����ƽ�֣������positionDetail��LeftVolume
	if(order.CombOffsetFlag == "CLOSE")
		UpdatePositionDetailByOrder(order);

	// ��ӡvecOrders��log�ļ�
	///PrintVecOrders();


	// ���ᱣ֤��������
	pFAccount->UpdateAccountByOrder(order);
		
	return;
}

// ��������,�������ȣ����������֡�ƽ��
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

// ��ϳɽ����ж϶����Ƿ���Գɽ������� ���֡�ƽ��
void FStrategy::MatchOrders(const IndicatorField& newIndicator,const string priceType)  // priceType: "NEW"��"CONTERPARTY"
{
	double price = 0;

	vector<OrderField>::iterator iter;
	for(iter = vecOrders.begin(); iter != vecOrders.end(); iter++)
	{
		// �����������Ӧ�ĺ�Լ���룬�ұ���״̬Ϊδ�ɽ�
		if(iter->InstrumentID == newIndicator.InstrumentID && iter->OrderStatus == 1)
		{

			// ȷ�����׼۸����¼� or ���ּ�
			if( priceType == "NEW")
				price = newIndicator.LastPrice;
			else
			{
				if (iter->Direction == "BUY")
					price =newIndicator.AskPrice1;  // OPEN BUY��CLOSE BUY
				else
					price =newIndicator.BidPrice1; // OPEN SELL��CLOSE SELL
			}

			if(iter->CombOffsetFlag == "OPEN")
				MatchOpenOrders(newIndicator,*iter,price); 
			else
				MatchCloseOrders(newIndicator,*iter,price);


		} // OrderStatus == 1

	} // for

	return;

}



// ��ϳɽ������ֵ�
void FStrategy::MatchOpenOrders(const IndicatorField& newIndicator,OrderField& order,const double price)
{

	if(order.Direction == "BUY")
	{
		
		// ί�м۸� >= ���¼۸�
		if(order.OrderPrice >= price)
		{
			// ���±����ĳɽ����� -------------------
			order.OrderStatus = 0;
			order.StatusMsg = "�ѳɽ�";
			order.UpdateTime = newIndicator.TradingTime;

			order.Price = price;  // ���¼ۡ����ּ�
			order.Volume = order.OrderVolume;
			order.Amount = order.Volume * order.Price * newInstrument.VolumeMultiple;
												
			//if(order.CombOffsetFlag == "OPEN")  
			//{
				order.StopPrice = order.Price; // ���֣�ƽ�� =0

				order.Margin = order.Amount * newInstrument.LongMarginRatio; // ���֣�ƽ�� =0

				if (newInstrument.OpenRatioByMoney > 0)
					order.Commission = order.Amount * newInstrument.OpenRatioByMoney; 
				else if (newInstrument.OpenRatioByVolume > 0)
					order.Commission = order.Volume * newInstrument.OpenRatioByVolume; 

				order.CloseProfit = 0;
				
			//}  // "OPEN"
			
			// ��ӡvecOrders
			///PrintVecOrders();

			// �ɽ�֮�󣬸���Account���ʽ�----------------
			pFAccount->UpdateAccountByOrder(order);

			// ���³ֲ���ϸ
			InsertPositionDetail(order);


		}

				

	} // "BUY"
	else 
	{ // "SELL"
		
		// ί�м۸� <= ���¼۸�
		if(order.OrderPrice <= price)
		{
			// ���±����ĳɽ����� -------------------
			order.OrderStatus = 0;
			order.StatusMsg = "�ѳɽ�";
			order.UpdateTime = newIndicator.TradingTime;

			order.Price = price;  // ���¼ۡ����ּ�
			order.Volume = order.OrderVolume;
			order.Amount = order.Volume * order.Price * newInstrument.VolumeMultiple;
						
						
			//if(order.CombOffsetFlag == "OPEN")  // ���֣�ƽ��
			//{
				order.StopPrice = order.Price; // ���֣�ƽ�� =0

				order.Margin = order.Amount * newInstrument.LongMarginRatio; // ���֣�ƽ�� =0

				if (newInstrument.OpenRatioByMoney > 0)
					order.Commission = order.Amount * newInstrument.OpenRatioByMoney; // ���֣�ƽ��
				else if (newInstrument.OpenRatioByVolume > 0)
					order.Commission = order.Volume * newInstrument.OpenRatioByVolume; // ���֣�ƽ��

				order.CloseProfit = 0;
			//}
			
			// ��ӡvecOrders
			///PrintVecOrders();
						
			// �ɽ�֮�󣬸���Account���ʽ�----------------
			pFAccount->UpdateAccountByOrder(order);

			// ���³ֲ���ϸ
			InsertPositionDetail(order);


		}



	}// "SELL"

	return;
}


// ��ϳɽ�: ƽ�ֵ�
void FStrategy::MatchCloseOrders(const IndicatorField& newIndicator,OrderField& order,const double price)
{

	if(order.Direction == "BUY")
	{
		
		// ί�м۸� >= ���¼۸�
		if(order.OrderPrice >= price)
		{
			// ���±����ĳɽ����� -------------------
			order.OrderStatus = 0;
			order.StatusMsg = "�ѳɽ�";
			order.UpdateTime = newIndicator.TradingTime;

			order.Price = price;
			order.Volume = order.OrderVolume;
			order.Amount = order.Volume * order.Price * newInstrument.VolumeMultiple;
												
			//if(order.CombOffsetFlag == "CLOSE")  
			//{
				//order.Margin = 0; // ƽ�� = ���ֵ��ı�֤��

				if (newInstrument.CloseRatioByMoney > 0)
					order.Commission = order.Amount * newInstrument.CloseRatioByMoney; 
				else if (newInstrument.CloseRatioByVolume > 0)
					order.Commission = order.Volume * newInstrument.CloseRatioByVolume; 
							
				// ��ƽ��������� ��Ϊ 0ʱ�����ǵ��ձ�����������Ϊ��
				if(newInstrument.CloseTodayRatioByMoney == 0 && newInstrument.CloseRatioByVolume == 0
					&& order.InsertTime.substr(0,10) == newIndicator.TradingTime.substr(0,10)  // ���յı���
					)
					order.Commission = 0;

				order.CloseProfit = CalcCloseProfit(order);

			//}
			
			// ��ӡvecOrders
			///PrintVecOrders();

			// �ɽ�֮�󣬸���Account���ʽ�----------------
			pFAccount->UpdateAccountByOrder(order);

			// �ɽ�֮�󣬸��³ֲ���ϸ
			UpdatePositionDetailByTrade(order);


		} //OrderPrice
				

	} // "BUY"

	else 

	{ // "SELL"
		
		// ί�м۸� <= ���¼۸�
		if(order.OrderPrice <= price)
		{
			// ���±����ĳɽ����� -------------------
			order.OrderStatus = 0;
			order.StatusMsg = "�ѳɽ�";
			order.UpdateTime = newIndicator.TradingTime;

			order.Price = price;  // ���¼ۡ����ּ�
			order.Volume = order.OrderVolume;
			order.Amount = order.Volume * order.Price * newInstrument.VolumeMultiple;
						
						
			//if(order.CombOffsetFlag == "CLOSE")  // ���֣�ƽ��
			//{
				//order.Margin = 0; // ƽ�� = ���ֵ��ı�֤��

				if (newInstrument.CloseRatioByMoney > 0)
					order.Commission = order.Amount * newInstrument.CloseRatioByMoney; // ���֣�ƽ��
				else if (newInstrument.CloseRatioByVolume > 0)
					order.Commission = order.Volume * newInstrument.CloseRatioByVolume; // ���֣�ƽ��
							
				// ��ƽ��������� ��Ϊ 0ʱ��������Ϊ��,���տ��ֵ�
				if(newInstrument.CloseTodayRatioByMoney == 0 && newInstrument.CloseRatioByVolume == 0
					&& order.InsertTime.substr(0,10) == newIndicator.TradingTime.substr(0,10)  // ���յı���
					)
					order.Commission = 0;

				order.CloseProfit = CalcCloseProfit(order);
			//}

			// ��ӡvecOrders
			///PrintVecOrders();
						
			// �ɽ�֮�󣬸���Account���ʽ�----------------
			pFAccount->UpdateAccountByOrder(order);

			// �ɽ�֮�󣬸��³ֲ���ϸ
			UpdatePositionDetailByTrade(order);


		} // OrderPrice



	}// "SELL"

	return;
}



// �������ֵ����������֮�����������ɽ������ϳ������ֵ�
void FStrategy::CancelOrders(const IndicatorField& newIndicator)
{
	static OrderField order1;
	
	vector<OrderField>::iterator iter;
	unsigned int intNum = 0;
	unsigned int intSize = vecOrders.size();  // ����Ҫ������forִ��֮ǰ��size����������ִ�й������²���ļ�¼���ٴ���

	//for(iter = vecOrders.begin(); iter != vecOrders.end(); iter++)
	for(intNum = 0; intNum <= intSize - 1; ++intNum)
	{
		// �����������Ӧ�ĺ�Լ���룬�ұ���״̬Ϊδ�ɽ�
		if(vecOrders.at(intNum).InstrumentID == newIndicator.InstrumentID && vecOrders.at(intNum).OrderStatus == 1)
		{
			    // ���ۿ��֡�ƽ�ֵ������ȳ������������·���
				
			    // ����
				vecOrders.at(intNum).OrderStatus = -1;
				vecOrders.at(intNum).StatusMsg = "�ѳ���";
				vecOrders.at(intNum).CancelTime = newIndicator.TradingTime;
			
				// ����֮�󣬸���Account���ʽ�----------------
				pFAccount->UpdateAccountByOrder(vecOrders.at(intNum));

				// ���·���
				memset(&order1,0,sizeof(OrderField));
				memcpy(&order1,&(vecOrders.at(intNum)),sizeof(order1));

				order1.InsertTime = newIndicator.TradingTime;
				order1.OrderRef = NextOrderRef++;
				order1.OpenOrderRef = (order1.CombOffsetFlag == "CLOSE") ? vecOrders.at(intNum).OpenOrderRef : 0;
				order1.OrderStatus = 1;
				order1.StatusMsg = "δ�ɽ�";
				order1.OrderPrice = (order1.Direction == "BUY") ? newIndicator.AskPrice1 : newIndicator.BidPrice1;
				order1.OrderAmount = order1.OrderVolume * order1.OrderPrice * newInstrument.VolumeMultiple;

				if(order1.CombOffsetFlag == "OPEN")
				   order1.FrozenMargin = order1.OrderAmount * ((order1.Direction == "BUY") ? newInstrument.LongMarginRatio:newInstrument.ShortMarginRatio);
				else
					order1.FrozenMargin = 0;

				order1.FrozenCommission = vecOrders.at(intNum).FrozenCommission;
				order1.CancelTime = "";

				if(order1.CombOffsetFlag == "OPEN")
				   dayTradingNum -= 1;    // ������ֵ������ˣ����ױ��� -1

				// �ύ����
				InsertVecOrders(order1);

				
		} // OrderStatus == 1

	} // for

	
	// ��ӡvecOrders
	///PrintVecOrders();
	
	return;
}



// ���ֳɽ�֮�����ɳֲ���ϸ
void FStrategy::InsertPositionDetail(const OrderField& trade)
{
	static PositionDetailField positionDetail;
	memset(&positionDetail,0,sizeof(PositionDetailField));

	// ���ɳֱֲ���
	CreatePositionDetail(trade,positionDetail);

	// ����ֲ���ϸ
	InsertVecPositionDetails(positionDetail);

	return;
}

// ���ݳɽ�֮��Ŀ���ί�е������ɳֲ���ϸ
void FStrategy::CreatePositionDetail(const OrderField& trade, PositionDetailField& positionDetail)
{
	// ����ֲ���ϸ

	///Ͷ���ߴ���
	positionDetail.InvestorID = pFAccount->InvestorID;

	///����ID
	positionDetail.StrategyID = StrategyID;

	positionDetail.TradingTime = trade.UpdateTime;

	///��Լ����
	positionDetail.InstrumentID = trade.InstrumentID;
	
	///�ɽ����
	positionDetail.OrderRef = trade.OrderRef;

	///����
	positionDetail.Direction = trade.Direction;

	///��������
	positionDetail.OpenDate = trade.UpdateTime.substr(0,10);
		
	///����
	positionDetail.Volume = trade.Volume;

	///�ɽ����
	positionDetail.Amount = trade.Amount;
	
	///���ּ�
	positionDetail.OpenPrice = trade.Price;

	///Ͷ���߱�֤��
	positionDetail.Margin = trade.Margin;

	///ֹ���
	positionDetail.StopPrice = trade.StopPrice;  // ����ֹ��۸�

	positionDetail.CloseTime = "";

	///ƽ����
	positionDetail.CloseVolume = 0;
	///ƽ�ֽ��
	positionDetail.CloseAmount = 0;

	///ƽ��ӯ��
	positionDetail.CloseProfit = 0;

	///��ƽ����
	positionDetail.LeftVolume = trade.Volume;



	return;
}



// �����ֵĳֲ���ϸ����vector
void FStrategy::InsertVecPositionDetails(const PositionDetailField& positionDetail)
{
	vecPositionDetails.push_back(positionDetail);

	// ��ӡvecPositionDetails
	///PrintVecPositionDetails();

	return;
}

// ����ƽ��ί��֮�󣬸��³ֲ���ϸ��LeftVolume
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

	// ��ӡvecPositionDetails
	///PrintVecPositionDetails();

	return;

}


// ƽ�ֳɽ�֮�󣬸��³ֲ���ϸ
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

	// ��ӡvecPositionDetails
	///PrintVecPositionDetails();

	return;
}

// �޶�ʱ��ﵽʱ��ǿ��ƽ�ֵ������гֲ֣�
void FStrategy::ForceClose()
{
	static OrderField order2;

	vector<PositionDetailField>::iterator iter;
	for(iter = vecPositionDetails.begin(); iter != vecPositionDetails.end(); iter++)
	{
		if(iter->LeftVolume >= 1)
		{
			memset(&order2,0,sizeof(OrderField));

			// ���� ǿ��ƽ��ί�е�
			CreateForceCloseOrder(*iter,order2);
			
			// �ύ����
			InsertVecOrders(order2);

		}

	} // for

}



// ����ƽ��ӯ��(order�����ȸ��³ɽ����)
double FStrategy::CalcCloseProfit( const OrderField& order)
{
	double closeProfit = 0;

	vector<OrderField>::iterator iter;
	for(iter = vecOrders.begin(); iter != vecOrders.end(); iter++)
	{
		if(iter->OrderRef == order.OpenOrderRef && order.Amount > 0)
		{
			if (iter->Direction == "BUY")  // ���ֵ�����ʱ
				closeProfit = order.Amount - iter->Amount;
			else  // == "SELL"
				closeProfit = iter->Amount - order.Amount;
		}

	} // for

	return closeProfit;
}

// ��ȡ ĳ��Լ ��δ�ɽ���ȫ��ί�е������������֡�ƽ�֣�
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


// ��ȡ ĳ��Լ ��δ�ɽ��ġ����֡�ί�е�����
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

// ��ȡ ĳ��Լ ��δ�ɽ��ġ����֡�ί�е�����
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


// ��ȡ ȫ����Լ ��δ�ɽ��ġ����֡�ί�е�����
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

// ��ȡ ĳ��Լ�ĳֲ�����
int FStrategy::GetInstPositionNum(const string instrumentId,const string volumeType)
{//volumeType: VOLUME��LEFTVOLUME
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


// ��ȡ ĳ��Լ�ģ���ͷ�ֲ֡���ͷ�ֲ֣���ǰ�ֲ֡���ǰ��ƽ�֣��ֲ�����
int  FStrategy::GetInstLongPositionNum(const string instrumentId,const string volumeType)
{	//volumeType: VOLUME��LEFTVOLUME

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

// ��ȡ ĳ��Լ�Ŀ�ͷ�ֲ�����
int  FStrategy::GetInstShortPositionNum(const string instrumentId,const string volumeType)
{   //volumeType: VOLUME��LEFTVOLUME

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


// ��ȡ ȫ����Լ�ĳֲ�����
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

// ��ȡ ĳ��Լ�Ŀ�ƽ������
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
//// ��ȡ ȫ����Լ�Ŀ�ƽ������
//int FStrategy::GetLeftPositionNum()
//{
//
//}

// ��ȡĳ��Լ�ĳֲ�ӯ��
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
// ��ȡȫ����Լ�ĳֲ�ӯ��
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

			// �ֲ�ӯ��
			if (iter->Direction == "BUY")
				dblProfit += dblMarketValue - iter->Amount;
			else
				dblProfit += iter->Amount - dblMarketValue;
		}

	} // for
	
	return dblProfit;
}

// ��ȡ ȫ����Լ ����� ��ӯ�� ���ֲ�ӯ��+ƽ��ӯ����
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
		

			// �ֲ�ӯ��
			if (iter->Direction == "BUY")
				dblProfit += dblMarketValue - iter->Amount;
			else
				dblProfit += iter->Amount - dblMarketValue;
		}

		// + ƽ��ӯ��
		dblProfit += iter->CloseProfit;

	} // for
	
	return dblProfit;
}




// ��ȡ��Լ������Ϣ
void  FStrategy::GetNewInstrument(const string instrumentId, InstrumentField& newInstrument)
{
	
	map<string, InstrumentField>::iterator iter= mapInstruments.find(instrumentId);

	if(iter != mapInstruments.end()) 
	{
		try 
		{
			//���µ�ǰ���������Ӧ�ĺ�Լ��Ϣ
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
		//û�ҵ�,����
		cerr<<"GetNewInstrument Error��mapInstruments���治���ڴ˺�Լ!"<<endl;

		return;
	}


}

// ��ȡ ��Լ����
int FStrategy::GetInstVolumeMultiple(const string instrumentId)
{
	int intVolumeMultiple = 0;

	map<string, InstrumentField>::iterator iter= mapInstruments.find(instrumentId);

	if(iter != mapInstruments.end()) 
	{
		// �ҵ�Key
		intVolumeMultiple = iter->second.VolumeMultiple;
	}
	else 
	{
		//û�ҵ�,����
		cerr<<"GetInstVolumeMultiple Error��mapInstruments���治���ڴ˺�Լ!"<<endl;
	}

	return intVolumeMultiple;

}

// �����������飬�����ֲֺ�Լ��ֹӯֹ��۸�
void FStrategy::UpdateStopPrice(const IndicatorField& newIndicator)
{
	vector<PositionDetailField>::iterator iter;
	for(iter = vecPositionDetails.begin(); iter != vecPositionDetails.end(); iter++)
	{
		if( iter->InstrumentID == newIndicator.InstrumentID && (iter->Volume - iter->CloseVolume) >=1 )
		{
			if(iter->Direction == "BUY")
			{
				// �ϵ��򵥵�ֹ��۸�
				if(newIndicator.LastPrice >= (iter->StopPrice + Strategy.ModifyRange) )
				{
					iter->StopPrice += (newIndicator.LastPrice - iter->StopPrice) * Strategy.ModifyRatio;

					// ��ӡ vecPositionDetails
					///PrintVecPositionDetails();

				}

				// �����Ρ��ϵ��򵥵�ֹ��۸�,�仯4��PriceTick֮�����
				if( iter->OpenPrice == iter->StopPrice && newIndicator.LastPrice >= (iter->OpenPrice + 4 * newInstrument.PriceTick) )
				{
					// ���� 2 �� PriceTick
					iter->StopPrice += 2 * newInstrument.PriceTick;

					// ��ӡ vecPositionDetails
					///PrintVecPositionDetails();

				}


			}  // BUY
			else  // "SELL"
			{
				// �µ�������ֹ��۸�
				if(newIndicator.LastPrice <= (iter->StopPrice - Strategy.ModifyRange) )
				{
					iter->StopPrice -= (iter->StopPrice - newIndicator.LastPrice ) * Strategy.ModifyRatio;

					// ��ӡ vecPositionDetails
					///PrintVecPositionDetails();

				}

				// �����Ρ��ϵ��򵥵�ֹ��۸�,�仯4��PriceTick֮�����
				if( iter->OpenPrice == iter->StopPrice && newIndicator.LastPrice <= (iter->OpenPrice - 4 * newInstrument.PriceTick) )
				{
					// ���� 2 �� PriceTick
					iter->StopPrice -= 2 * newInstrument.PriceTick;

					// ��ӡ vecPositionDetails
					///PrintVecPositionDetails();

				}


			} // SELL

		} // InstrumentID
	} // for

	return;
}

// �����������飬�����ֲֺ�Լ��ֹӯֹ��۸�,������һ�Σ�Ŀ�Ŀ��ƽ�����ʧ
void FStrategy::UpdateStopPriceFirst(const IndicatorField& newIndicator)
{
	vector<PositionDetailField>::iterator iter;
	for(iter = vecPositionDetails.begin(); iter != vecPositionDetails.end(); iter++)
	{
		if( iter->InstrumentID == newIndicator.InstrumentID && (iter->Volume - iter->CloseVolume) >=1 )
		{
			if(iter->Direction == "BUY")
			{
				//// �ϵ��򵥵�ֹ��۸�
				//if(newIndicator.LastPrice >= (iter->StopPrice + Strategy.ModifyRange) )
				//{
				//	iter->StopPrice += (newIndicator.LastPrice - iter->StopPrice) * Strategy.ModifyRatio;

				//	// ��ӡ vecPositionDetails
				//	PrintVecPositionDetails();

				//}

				// �����Ρ��ϵ��򵥵�ֹ��۸�,�仯4��PriceTick֮�����
				if( iter->OpenPrice == iter->StopPrice && newIndicator.LastPrice >= (iter->OpenPrice + 4 * newInstrument.PriceTick) )
				{
					// ���� 2 �� PriceTick
					iter->StopPrice += 2 * newInstrument.PriceTick;

					// ��ӡ vecPositionDetails
					///PrintVecPositionDetails();

				}


			}  // BUY
			else  // "SELL"
			{
				//// �µ�������ֹ��۸�
				//if(newIndicator.LastPrice <= (iter->StopPrice - Strategy.ModifyRange) )
				//{
				//	iter->StopPrice -= (iter->StopPrice - newIndicator.LastPrice ) * Strategy.ModifyRatio;

				//	// ��ӡ vecPositionDetails
				//	PrintVecPositionDetails();

				//}

				// �����Ρ��ϵ��򵥵�ֹ��۸�,�仯4��PriceTick֮�����
				if( iter->OpenPrice == iter->StopPrice && newIndicator.LastPrice <= (iter->OpenPrice - 4 * newInstrument.PriceTick) )
				{
					// ���� 2 �� PriceTick
					iter->StopPrice -= 2 * newInstrument.PriceTick;

					// ��ӡ vecPositionDetails
					///PrintVecPositionDetails();

				}


			} // SELL

		} // InstrumentID
	} // for

	return;
}


// ÿ�ս��׼�ӯ������
void FStrategy::DailyStat(const string TradingDate)
{
	 
	// �ʽ����

	// ƽ��ӯ��

	// �ֲ�ӯ��

	// ��ӯ��

	// ���ʲ�

	// ���ױ���

	// ����������

	int intX = 0;

	return;

}


// ��vecOrders�����log�ļ�
void FStrategy::PrintVecOrders()
{
	    // ��ȡ��ǰϵͳʱ�䣬��ȷ������
		string strNewTime=GetNowMillisecond(); 

		ofstream log_md("AATS_vecOrders_log.csv",ios::app);
		//log_md<<strNewTime<<",ATS_vecOrders_log,begin,---"<<endl;

		vector<OrderField>::iterator iter;
		for(iter = vecOrders.begin(); iter != vecOrders.end(); iter++)
		{
			
			///Ͷ���ߴ���
			log_md<<iter->InvestorID;

			///����ID
			log_md<<","<<iter->StrategyID;

			///��Լ����
			log_md<<","<<iter->InstrumentID;

			///ί��ʱ��
			log_md<<","<<iter->InsertTime;  // yyyy-mm-dd hh:mm:ss.000

			///��������
			log_md<<","<<iter->OrderRef;

			///ƽ��ID
			log_md<<","<<iter->OpenOrderRef;  // = TradeID

			///��Ͽ�ƽ��־
			log_md<<","<<iter->CombOffsetFlag;

			///��������
			log_md<<","<<iter->Direction;
	
			///�����۸�
			log_md<<","<<iter->OrderPrice;
			///��������
			log_md<<","<<iter->OrderVolume;
			///�������
			log_md<<","<<iter->OrderAmount;

			///����ı�֤��
			log_md<<","<<iter->FrozenMargin;
			///�����������
			log_md<<","<<iter->FrozenCommission;

			///ֹ���
			log_md<<","<<iter->StopPrice;
	
			///����״̬
			log_md<<","<<iter->OrderStatus;  // 1-δ�ɽ���0-�ѳɽ���-1-�ѳ���
	
			///״̬��Ϣ
			log_md<<","<<iter->StatusMsg;

			///����ʱ��
			log_md<<","<<iter->CancelTime;

			///�ɽ�ʱ��
			log_md<<","<<iter->UpdateTime;
	
			///�ɽ��۸�
			log_md<<","<<iter->Price;
			///�ɽ�����
			log_md<<","<<iter->Volume;
			///�ɽ����
			log_md<<","<<iter->Amount;

			///��֤��
			log_md<<","<<iter->Margin;
			///������
			log_md<<","<<iter->Commission;

			// ƽ��ӯ��(ֻ�е�ƽ��֮��Ÿ���ֵ)
			log_md<<","<<iter->CloseProfit;
			
			log_md<<endl;


		} // for

		//log_md<<"---,vecOrders,end,---"<<endl;
		log_md.close();

}



// ��vecPositionDetails�����log�ļ�
void FStrategy::PrintVecPositionDetails()
{
	// ��ȡ��ǰϵͳʱ�䣬��ȷ������
		string strNewTime=GetNowMillisecond(); 

		//ofstream log_md("ATS_vecOrders_log.csv",ios::app);
		ofstream log_md("AATS_vecPositionDetails_log.csv",ios::app);
		//log_md<<strNewTime<<",ATS_vecPositionDetails_log,begin,---"<<endl;

		vector<PositionDetailField>::iterator iter;
		for(iter = vecPositionDetails.begin(); iter != vecPositionDetails.end(); iter++)
		{

			///Ͷ���ߴ���
			log_md<<iter->InvestorID;

			///����ID
			log_md<<","<<iter->StrategyID;

			// �ɽ�ʱ��
			log_md<<","<<iter->TradingTime;

			///��Լ����
			log_md<<","<<iter->InstrumentID;
	
			///�ɽ����
			log_md<<","<<iter->OrderRef;

			///����
			log_md<<","<<iter->Direction;
			///��������
			log_md<<","<<iter->OpenDate;
		
			///����
			log_md<<","<<iter->Volume;

			///���ּ�
			log_md<<","<<iter->OpenPrice;

			///�ɽ����
			log_md<<","<<iter->Amount;
	

			///Ͷ���߱�֤��
			log_md<<","<<iter->Margin;

			///ֹ���
			log_md<<","<<iter->StopPrice;  // ����ֹ��۸�

			/// ƽ��ʱ��
			log_md<<","<<iter->CloseTime; 
			///ƽ����
			log_md<<","<<iter->CloseVolume;
			///ƽ�ֽ��
			log_md<<","<<iter->CloseAmount;

			///ƽ��ӯ��
			log_md<<","<<iter->CloseProfit;

			///��ƽ����
			log_md<<","<<iter->LeftVolume;

			log_md<<endl;


		} // for

		//log_md<<"---,vecPositionDetails,end,---"<<endl;
		log_md.close();

}


//// ����Ϣд����־�ļ�
//	void FStrategy::printMessage2Log(string strMessage)
//	{
//	    // ��ȡ��ǰϵͳʱ�䣬��ȷ������
//		string strNewTime=GetNowMillisecond(); 
//
//		ofstream log_md("TestData/fun_order_log.txt",ios::app);
//		log_md<<"["<<strNewTime<<"] "<<strMessage<<endl;
//		log_md.close();
//
//	}

	// ��ȡϵͳ��ǰʱ��ĺ�����
	string FStrategy::GetNowMillisecond(void)
	{
		// ��ȡ��ǰϵͳʱ�䣬��ȷ������-----------------
		SYSTEMTIME sys;          // ���ڳ���ͷ����� #include "windows.h"
		GetLocalTime( &sys ); 
		char buf1[100];
		sprintf_s(buf1, "%4d-%02d-%02d %02d:%02d:%02d.%03d", sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute,sys.wSecond,sys.wMilliseconds);
		//sprintf_s(buf1, "%02d:%02d:%02d.%03d", sys.wHour,sys.wMinute,sys.wSecond,sys.wMilliseconds);
		string strNewTime= buf1;  // ����ϵͳ����ʱ�䣬��ȷ������
		// ��ȡ��ǰϵͳʱ�䣬��ȷ������  end-----------

		return strNewTime;
	}

	
//// ��ϳɽ����������֡�ƽ��
//void FStrategy::MatchOrders(const IndicatorField& newIndicator,OrderField& order,const string buySell,const double price)
//{
//
//	if(buySell == "BUY")
//	{
//		
//		// ���� ���ֵ���ƽ�ֵ�,ί�м۸� >= ���¼۸�
//		if(order.OrderPrice >= price)
//		{
//			// ���±����ĳɽ����� -------------------
//			order.OrderStatus = 0;
//			order.StatusMsg = "�ѳɽ�";
//			order.UpdateTime = newIndicator.TradingTime;
//
//			order.Price = price;
//			order.Volume = order.OrderVolume;
//			order.Amount = order.Volume * order.Price * newInstrument.VolumeMultiple;
//												
//			if(order.CombOffsetFlag == "OPEN")  
//			{
//				order.StopPrice = order.Price; // ���֣�ƽ�� =0
//
//				order.Margin = order.Amount * newInstrument.LongMarginRatio; // ���֣�ƽ�� =0
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
//				order.Margin = 0; // ���֣�ƽ�� =0
//
//				if (newInstrument.CloseRatioByMoney > 0)
//					order.Commission = order.Amount * newInstrument.CloseRatioByMoney; 
//				else if (newInstrument.CloseRatioByVolume > 0)
//					order.Commission = order.Volume * newInstrument.CloseRatioByVolume; 
//							
//				// ��ƽ��������� ��Ϊ 0ʱ�����ǵ��ձ�����������Ϊ��
//				if(newInstrument.CloseTodayRatioByMoney == 0 && newInstrument.CloseRatioByVolume == 0
//					&& order.InsertTime.substr(0,10) == newIndicator.TradingTime.substr(0,10)  // ���յı���
//					)
//					order.Commission = 0;
//
//				order.CloseProfit = CalcCloseProfit(order);
//
//			}
//						
//			// �ɽ�֮�󣬸���Account���ʽ�----------------
//			pFAccount->UpdateAccountByOrder(order);
//
//			// ���³ֲ���ϸ
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
//		// ���� ���ֵ���ƽ�ֵ�,ί�м۸� <= ���¼۸�
//		if(order.OrderPrice <= price)
//		{
//			// ���±����ĳɽ����� -------------------
//			order.OrderStatus = 0;
//			order.StatusMsg = "�ѳɽ�";
//			order.UpdateTime = newIndicator.TradingTime;
//
//			order.Price = price;  // �ɽ��۸�������۸�
//			order.Volume = order.OrderVolume;
//			order.Amount = order.Volume * order.Price * newInstrument.VolumeMultiple;
//						
//						
//			if(order.CombOffsetFlag == "OPEN")  // ���֣�ƽ��
//			{
//				order.StopPrice = order.Price; // ���֣�ƽ�� =0
//
//				order.Margin = order.Amount * newInstrument.LongMarginRatio; // ���֣�ƽ�� =0
//
//				if (newInstrument.OpenRatioByMoney > 0)
//					order.Commission = order.Amount * newInstrument.OpenRatioByMoney; // ���֣�ƽ��
//				else if (newInstrument.OpenRatioByVolume > 0)
//					order.Commission = order.Volume * newInstrument.OpenRatioByVolume; // ���֣�ƽ��
//
//				order.CloseProfit = 0;
//			}
//			else  // == "CLOSE"
//			{
//	
//				order.Margin = 0; // ���֣�ƽ�� =0
//
//				if (newInstrument.CloseRatioByMoney > 0)
//					order.Commission = order.Amount * newInstrument.CloseRatioByMoney; // ���֣�ƽ��
//				else if (newInstrument.CloseRatioByVolume > 0)
//					order.Commission = order.Volume * newInstrument.CloseRatioByVolume; // ���֣�ƽ��
//							
//				// ��ƽ��������� ��Ϊ 0ʱ��������Ϊ�㡣��TODO�� �ٶ�û�и�ҹ��
//				if(newInstrument.CloseTodayRatioByMoney == 0 && newInstrument.CloseRatioByVolume == 0
//					&& order.InsertTime.substr(0,10) == newIndicator.TradingTime.substr(0,10)  // ���յı���
//					)
//					order.Commission = 0;
//
//				order.CloseProfit = CalcCloseProfit(order);
//			}
//
//						
//			// �ɽ�֮�󣬸���Account���ʽ�----------------
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
