
#include <iostream>
#include <fstream>
#include <vector>
#include <map>

#include "FQuote.h"

using namespace std;

FQuote::FQuote(void)
{

}


FQuote::~FQuote(void)
{
}



// ��������vector����Ӧָ�꣬Ȼ���������mapQuotes
void FQuote::Update2mapQuotes(const QuoteField& newQuote)  
{  
  
	if (newQuote.LastPrice <= 0)
	{
		cerr<<"��ȡ���������������⣬newQuote��������"<<endl;
		return;
	}


	//MapQuotesType::iterator iter= mapQuotes.find(newQuote.InstrumentID);
	map<string,vector<QuoteField>>::iterator iter= mapQuotes.find(newQuote.InstrumentID);

	if(iter != mapQuotes.end()) 
	{
		//�ҵ�Key��������vecQuotes����
		iter->second.push_back(newQuote);

	}
	else 
	{
		//û�ҵ�,�����µ�Key

		vector<QuoteField> vecQuotes1;  // ����10�򣬱����������
		vecQuotes1.push_back(newQuote);

		mapQuotes.insert(map<string,vector<QuoteField>>::value_type(newQuote.InstrumentID, vecQuotes1));
		(--mapQuotes.end())->second.reserve(100000); // Ϊÿһ���º�Լmap�ĵڶ���������vector�����ô�����������Ƶ������
		
		//MapIndicatorsType::iterator iter2 = mapIndicators.end();
		//iter2->second.reserve(100000);


	}

	return;

} 

// ����newQuote������ָ��mapNewQuotes
void FQuote::Update2mapNewQuotes(const QuoteField& newQuote)
{
	//MapQuotesType::iterator iter= mapQuotes.find(newQuote.InstrumentID);
	map<string,QuoteField>::iterator iter= mapNewQuotes.find(newQuote.InstrumentID);

	if(iter != mapNewQuotes.end()) 
	{
		//�ҵ�Key������second����
		memset(&iter->second,0,sizeof(iter->second));
		memcpy(&iter->second, &newQuote, sizeof(iter->second));

	}
	else 
	{
		//û�ҵ�,�����µ�Key
		mapNewQuotes.insert(map<string,QuoteField>::value_type(newQuote.InstrumentID, newQuote));
		
	}
	

	// ����newQuote������ָ��mapQuotes
	Update2mapQuotes(newQuote);

	
	return;

}

// ��ȡĳ��Լ�����¼۸�
double FQuote::GetInstLastPrice(const string instrumentID)
{
	double dblLastPrice = 0;

	map<string,QuoteField>::iterator iter= mapNewQuotes.find(instrumentID);

	if(iter != mapNewQuotes.end()) 
	{
		//�ҵ�Key������second����
		dblLastPrice = iter->second.LastPrice;
	}

	//û�ҵ�,�����µ�Key
	//dblLastPrice = 0;

	return dblLastPrice;

}

// ��ȡĳ��Լ��������һ�۸�
double FQuote::GetInstNewAskPrice1(const string instrumentID)
{
	double dblLastPrice = 0;

	map<string,QuoteField>::iterator iter= mapNewQuotes.find(instrumentID);

	if(iter != mapNewQuotes.end()) 
	{
		//�ҵ�Key������second����
		dblLastPrice = iter->second.AskPrice1;
	}

	//û�ҵ�,�����µ�Key
	//dblLastPrice = 0;

	return dblLastPrice;

}

// ��ȡĳ��Լ��������һ�۸�
double FQuote::GetInstNewBidPrice1(const string instrumentID)
{
	double dblLastPrice = 0;

	map<string,QuoteField>::iterator iter= mapNewQuotes.find(instrumentID);

	if(iter != mapNewQuotes.end()) 
	{
		//�ҵ�Key������second����
		dblLastPrice = iter->second.BidPrice1;
	}

	//û�ҵ�,�����µ�Key
	//dblLastPrice = 0;

	return dblLastPrice;

}


// ������������vector������ָ�꣬����ָ��mapIndicators
void FQuote::CalcIndicators(const QuoteField& newQuote)  
{  

	// ��ʼ�� newIndicator
    InitNewIndicator(newIndicator);

	// �ṹ������ֵ
	newIndicator.TradingTime = newQuote.TradingTime;
	newIndicator.InstrumentID = newQuote.InstrumentID;

	newIndicator.LastPrice = newQuote.LastPrice;

	newIndicator.BidPrice1 = newQuote.BidPrice1;
	newIndicator.BidVolume1 = newQuote.BidVolume1;
	newIndicator.AskPrice1 = newQuote.AskPrice1;
	newIndicator.AskVolume1 = newQuote.AskVolume1;

	// ����ָ��
	string strInstID;
	strInstID = newQuote.InstrumentID;  // ����1�� char[] ת string
	//strInstID.insert(0,newQuote.InstrumentID); // ����2��char[] ת string

	double dblAvgDay = CalcLastPriceAvgDay(strInstID);

	double dblAvg10 = CalcLastPriceAvg(strInstID,10);
	double dblAvg120 = CalcLastPriceAvg(strInstID,120);
	double dblAvg240 = CalcLastPriceAvg(strInstID,240);
	
	double dblMax10 = CalcLastPriceMax(strInstID,10);
	double dblMax120 = CalcLastPriceMax(strInstID,120);

	double dblMin10 = CalcLastPriceMin(strInstID,10);
	double dblMin120 = CalcLastPriceMin(strInstID,120);


	newIndicator.Avg10 = dblAvg10;
	newIndicator.Avg120 = dblAvg120;
	newIndicator.Avg240 = dblAvg240;
	newIndicator.AvgDay = dblAvgDay;
		

	// ���µ��տ�����������߼ۡ���ͼ�
	if(newIndicator.MaxPrice < newQuote.LastPrice)
		newIndicator.MaxPrice = newQuote.LastPrice;
	else 
		if (newIndicator.MinPrice <= 0 || newIndicator.MinPrice > newQuote.LastPrice)
			newIndicator.MinPrice = newQuote.LastPrice;


	newIndicator.Max10 = dblMax10;
	newIndicator.Max120 = dblMax120;

	newIndicator.Min10 = dblMin10;
	newIndicator.Min120 = dblMin120;


	// ������ newIndicator�������� map
	Update2mapIndicators(newIndicator);

	return;

} 

// �������¼۸��ƽ��ֵ
double FQuote::CalcLastPriceAvg(const string instrumentID,const int intNum)  
{  
	unsigned int intN = 0;
	unsigned int intNum2 = intNum;
	double dblAvg = 0;

	map<string,vector<QuoteField>>::iterator iterMap= mapQuotes.find(instrumentID);

	if(iterMap != mapQuotes.end()) 
	{
		//�ҵ�Key������ָ��
	
		// ��� vector ���������㣬�򷵻ؾ�ֵΪ0
		if (iterMap->second.size() < intNum2)
			return 0.0;

		// ��������㹻
		vector<QuoteField>::reverse_iterator iter;
		for (iter = iterMap->second.rbegin(); iter != iterMap->second.rend(); iter++ )
		{
			dblAvg = dblAvg + (*iter).LastPrice; 
			intN += 1;
			
			if (intN >= intNum2)
				break;
		}
	
		dblAvg = dblAvg / intNum;


	}
	else 
	{
		//û�ҵ�,������ָ��
		cerr<<"CalcLastPriceAvg: mapQuotes�в����ڸú�Լ"<<endl;
		
	}

	return dblAvg;

}

//// �������¼۸��ƽ��ֵ
//double CalcLastPriceAvg(TThostFtdcInstrumentIDType instrumentID, unsigned int intNum)  
//{
//	string strInstID(instrumentID); 
//
//	CalcLastPriceAvg(strInstID, intNum);
//
//}

// �������¼۸��ƽ��ֵ
double FQuote::CalcLastPriceAvgDay(const string instrumentID)  
{  
	int intN = 0;
	double dblAvg = 0;

	//MapQuotesType::iterator iterMap= mapQuotes.find(newQuote.InstrumentID);
	map<string,vector<QuoteField>>::iterator iterMap= mapQuotes.find(instrumentID);

	if(iterMap != mapQuotes.end()) 
	{
		//�ҵ�Key������ָ��
		

		vector<QuoteField>::iterator iter;
		for (iter = iterMap->second.begin(); iter != iterMap->second.end(); iter++ )
		{
			dblAvg = dblAvg + (*iter).LastPrice;
			intN += 1;
	
		}
	
		dblAvg = dblAvg / intN;


	}
	else 
	{
		//û�ҵ�,������ָ��
		cerr<<"CalcLastPriceAvgDay: mapQuotes�в����ڸú�Լ"<<endl;	
		
	}

	return dblAvg;

}


// ����۸�����ֵ
double FQuote::CalcLastPriceMax(const string instrumentID,const int intNum)
{
	unsigned int intN = 0;
	unsigned int intNum2 = intNum;
	double dblMax = 0;

	unsigned int intX = 0;
	unsigned int intSize = 0;

	map<string,vector<QuoteField>>::iterator iterMap= mapQuotes.find(instrumentID);

	if(iterMap != mapQuotes.end()) 
	{
		//�ҵ�Key������ָ��
	
		// ��� vector ���������㣬�򷵻ؾ�ֵΪ0
		if (iterMap->second.size() < intNum2)
			return 0.0;

		// ��������㹻


		intSize = iterMap->second.size();
		for(intX = intSize -1; intX >= intSize - intNum; intX-- )
		{
			if ( iterMap->second.at(intX).LastPrice > dblMax )
			   dblMax = iterMap->second.at(intX).LastPrice; 

			intN += 1;
			
			if (intN >= intNum2)
				break;
			
		}

		/*vector<CThostFtdcDepthMarketDataField>::reverse_iterator iter;
		for (iter = iterMap->second.rbegin(); iter != iterMap->second.rend(); ++iter )
		{
			dblAvg = dblAvg + (*iter).LastPrice; 
			intN += 1;
			
			if (intN >= intNum2)
				break;
		}*/
	

	}
	else 
	{
		//û�ҵ�,������ָ��
		cerr<<"CalcLastPriceMax: mapQuotes�в����ڸú�Լ"<<endl;
		
	}

	return dblMax;

}

// ����۸�����ֵ
double FQuote::CalcLastPriceMin(const string instrumentID,const int intNum)
{
	unsigned int intN = 0;
	unsigned int intNum2 = intNum;
	double dblMin = 1000000;

	unsigned int intX = 0;
	unsigned int intSize = 0;

	map<string,vector<QuoteField>>::iterator iterMap= mapQuotes.find(instrumentID);

	if(iterMap != mapQuotes.end()) 
	{
		//�ҵ�Key������ָ��
	
		// ��� vector ���������㣬�򷵻ؾ�ֵΪ0
		if (iterMap->second.size() < intNum2)
			return 0.0;

		// ��������㹻


		intSize = iterMap->second.size();
		for(intX = intSize -1; intX >= intSize - intNum; intX-- )
		{
			if ( iterMap->second.at(intX).LastPrice < dblMin )
			   dblMin = iterMap->second.at(intX).LastPrice; 

			intN += 1;
			
			if (intN >= intNum2)
				break;
			
		}

		/*vector<CThostFtdcDepthMarketDataField>::reverse_iterator iter;
		for (iter = iterMap->second.rbegin(); iter != iterMap->second.rend(); ++iter )
		{
			dblAvg = dblAvg + (*iter).LastPrice; 
			intN += 1;
			
			if (intN >= intNum2)
				break;
		}*/
	

	}
	else 
	{
		//û�ҵ�,������ָ��
		cerr<<"CalcLastPriceMax: mapQuotes�в����ڸú�Լ"<<endl;
		
	}

	if (dblMin >= 1000000)
		dblMin = 0;

	return dblMin;


}




// ����ָ��vector������ָ��mapQuotes
void FQuote::Update2mapIndicators(const IndicatorField& newIndicator)  
{  
  
	if (newIndicator.LastPrice <= 0)
	{
		cerr<<"��ȡ���������������⣬newQuote��������"<<endl;
		return;
	}


	map<string,vector<IndicatorField>>::iterator iter= mapIndicators.find(newIndicator.InstrumentID);

	if(iter != mapIndicators.end()) 
	{
		//�ҵ�Key��������vecQuotes����
		iter->second.push_back(newIndicator);
	}
	else 
	{
		//û�ҵ�,�����µ�Key

		vector<IndicatorField> vecIndicators1; // ����10�򣬱����������
		vecIndicators1.push_back(newIndicator);

		mapIndicators.insert(map<string,vector<IndicatorField>>::value_type(newIndicator.InstrumentID, vecIndicators1));

		(--mapIndicators.end())->second.reserve(100000); // Ϊÿһ���º�Լmap�ĵڶ���������vector�����ô�����������Ƶ������

		//MapIndicatorsType::iterator iter2 = mapIndicators.end();
		//(*iter2).second.reserve();


	}

	return;

} 


// ��ȡTʱ�̵�����ָ�꣨T=0,1,2,3...��
void FQuote::GetInstNewIndicator(const string instrumentID, IndicatorField& newIndicator, const int intT) 
{  
  
	if (instrumentID.length() <= 0)
	{
		cerr<<"GetInstNewIndicator():InstrumentID����"<<endl;
		return;
	}


	map<string,vector<IndicatorField>>::iterator iter= mapIndicators.find(instrumentID);

	if(iter != mapIndicators.end()) 
	{
		//�ҵ�Key����ȡ��T�����µ�ָ������
	
		memset(&newIndicator,0,sizeof(IndicatorField));
		memcpy(&newIndicator,&(iter->second.at(iter->second.size() -1 - intT)),sizeof(newIndicator));

		
	}
	else 
	{
		memset(&newIndicator,0,sizeof(IndicatorField));
	}

	return;

} 




// ��ʼ�� �µ�һ�� ������ ��ָ��
void FQuote::InitNewDayQuotes()
{
	// ��� ����map
	mapQuotes.clear();

	// ��� ָ��map
	mapIndicators.clear();
	
	return;

}

// ��ʼ�� newIndicator �ṹ����
void FQuote::InitNewIndicator(IndicatorField& newIndicator)
{
	//����ʱ��  yyyy-mm-dd hh:mm:ss.000
    newIndicator.TradingTime = "";  

	///��Լ����
	newIndicator.InstrumentID = "";

	///���¼�
	newIndicator.LastPrice = 0;

	///�����һ
	newIndicator.BidPrice1 = 0;
	///������һ
	newIndicator.BidVolume1 = 0;
	///������һ
	newIndicator.AskPrice1 = 0;
	///������һ
	newIndicator.AskVolume1 = 0;

	newIndicator.Avg10 = 0;
	newIndicator.Avg120 = 0;
	newIndicator.Avg240 = 0;
	newIndicator.AvgDay = 0;

	newIndicator.MaxPrice = 0;
	newIndicator.MinPrice = 0;

	newIndicator.BuySell = "";
	
}


