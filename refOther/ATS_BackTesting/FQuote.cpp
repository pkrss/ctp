
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



// 根据行情vector，对应指标，然后存入行情mapQuotes
void FQuote::Update2mapQuotes(const QuoteField& newQuote)  
{  
  
	if (newQuote.LastPrice <= 0)
	{
		cerr<<"读取的行情数据有问题，newQuote数据有误！"<<endl;
		return;
	}


	//MapQuotesType::iterator iter= mapQuotes.find(newQuote.InstrumentID);
	map<string,vector<QuoteField>>::iterator iter= mapQuotes.find(newQuote.InstrumentID);

	if(iter != mapQuotes.end()) 
	{
		//找到Key，插入其vecQuotes向量
		iter->second.push_back(newQuote);

	}
	else 
	{
		//没找到,插入新的Key

		vector<QuoteField> vecQuotes1;  // 容量10万，避免后续重设
		vecQuotes1.push_back(newQuote);

		mapQuotes.insert(map<string,vector<QuoteField>>::value_type(newQuote.InstrumentID, vecQuotes1));
		(--mapQuotes.end())->second.reserve(100000); // 为每一个新合约map的第二个变量（vector）设置大容量，避免频繁扩容
		
		//MapIndicatorsType::iterator iter2 = mapIndicators.end();
		//iter2->second.reserve(100000);


	}

	return;

} 

// 根据newQuote，存入指标mapNewQuotes
void FQuote::Update2mapNewQuotes(const QuoteField& newQuote)
{
	//MapQuotesType::iterator iter= mapQuotes.find(newQuote.InstrumentID);
	map<string,QuoteField>::iterator iter= mapNewQuotes.find(newQuote.InstrumentID);

	if(iter != mapNewQuotes.end()) 
	{
		//找到Key，更新second向量
		memset(&iter->second,0,sizeof(iter->second));
		memcpy(&iter->second, &newQuote, sizeof(iter->second));

	}
	else 
	{
		//没找到,插入新的Key
		mapNewQuotes.insert(map<string,QuoteField>::value_type(newQuote.InstrumentID, newQuote));
		
	}
	

	// 根据newQuote，存入指标mapQuotes
	Update2mapQuotes(newQuote);

	
	return;

}

// 获取某合约的最新价格
double FQuote::GetInstLastPrice(const string instrumentID)
{
	double dblLastPrice = 0;

	map<string,QuoteField>::iterator iter= mapNewQuotes.find(instrumentID);

	if(iter != mapNewQuotes.end()) 
	{
		//找到Key，更新second向量
		dblLastPrice = iter->second.LastPrice;
	}

	//没找到,插入新的Key
	//dblLastPrice = 0;

	return dblLastPrice;

}

// 获取某合约的最新卖一价格
double FQuote::GetInstNewAskPrice1(const string instrumentID)
{
	double dblLastPrice = 0;

	map<string,QuoteField>::iterator iter= mapNewQuotes.find(instrumentID);

	if(iter != mapNewQuotes.end()) 
	{
		//找到Key，更新second向量
		dblLastPrice = iter->second.AskPrice1;
	}

	//没找到,插入新的Key
	//dblLastPrice = 0;

	return dblLastPrice;

}

// 获取某合约的最新买一价格
double FQuote::GetInstNewBidPrice1(const string instrumentID)
{
	double dblLastPrice = 0;

	map<string,QuoteField>::iterator iter= mapNewQuotes.find(instrumentID);

	if(iter != mapNewQuotes.end()) 
	{
		//找到Key，更新second向量
		dblLastPrice = iter->second.BidPrice1;
	}

	//没找到,插入新的Key
	//dblLastPrice = 0;

	return dblLastPrice;

}


// 根据最新行情vector，计算指标，存入指标mapIndicators
void FQuote::CalcIndicators(const QuoteField& newQuote)  
{  

	// 初始化 newIndicator
    InitNewIndicator(newIndicator);

	// 结构变量赋值
	newIndicator.TradingTime = newQuote.TradingTime;
	newIndicator.InstrumentID = newQuote.InstrumentID;

	newIndicator.LastPrice = newQuote.LastPrice;

	newIndicator.BidPrice1 = newQuote.BidPrice1;
	newIndicator.BidVolume1 = newQuote.BidVolume1;
	newIndicator.AskPrice1 = newQuote.AskPrice1;
	newIndicator.AskVolume1 = newQuote.AskVolume1;

	// 计算指标
	string strInstID;
	strInstID = newQuote.InstrumentID;  // 方法1： char[] 转 string
	//strInstID.insert(0,newQuote.InstrumentID); // 方法2：char[] 转 string

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
		

	// 更新当日开盘以来的最高价、最低价
	if(newIndicator.MaxPrice < newQuote.LastPrice)
		newIndicator.MaxPrice = newQuote.LastPrice;
	else 
		if (newIndicator.MinPrice <= 0 || newIndicator.MinPrice > newQuote.LastPrice)
			newIndicator.MinPrice = newQuote.LastPrice;


	newIndicator.Max10 = dblMax10;
	newIndicator.Max120 = dblMax120;

	newIndicator.Min10 = dblMin10;
	newIndicator.Min120 = dblMin120;


	// 将最新 newIndicator，更新至 map
	Update2mapIndicators(newIndicator);

	return;

} 

// 计算最新价格的平均值
double FQuote::CalcLastPriceAvg(const string instrumentID,const int intNum)  
{  
	unsigned int intN = 0;
	unsigned int intNum2 = intNum;
	double dblAvg = 0;

	map<string,vector<QuoteField>>::iterator iterMap= mapQuotes.find(instrumentID);

	if(iterMap != mapQuotes.end()) 
	{
		//找到Key，计算指标
	
		// 如果 vector 的数量不足，则返回均值为0
		if (iterMap->second.size() < intNum2)
			return 0.0;

		// 如果数量足够
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
		//没找到,不计算指标
		cerr<<"CalcLastPriceAvg: mapQuotes中不存在该合约"<<endl;
		
	}

	return dblAvg;

}

//// 计算最新价格的平均值
//double CalcLastPriceAvg(TThostFtdcInstrumentIDType instrumentID, unsigned int intNum)  
//{
//	string strInstID(instrumentID); 
//
//	CalcLastPriceAvg(strInstID, intNum);
//
//}

// 计算最新价格的平均值
double FQuote::CalcLastPriceAvgDay(const string instrumentID)  
{  
	int intN = 0;
	double dblAvg = 0;

	//MapQuotesType::iterator iterMap= mapQuotes.find(newQuote.InstrumentID);
	map<string,vector<QuoteField>>::iterator iterMap= mapQuotes.find(instrumentID);

	if(iterMap != mapQuotes.end()) 
	{
		//找到Key，计算指标
		

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
		//没找到,不计算指标
		cerr<<"CalcLastPriceAvgDay: mapQuotes中不存在该合约"<<endl;	
		
	}

	return dblAvg;

}


// 计算价格的最高值
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
		//找到Key，计算指标
	
		// 如果 vector 的数量不足，则返回均值为0
		if (iterMap->second.size() < intNum2)
			return 0.0;

		// 如果数量足够


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
		//没找到,不计算指标
		cerr<<"CalcLastPriceMax: mapQuotes中不存在该合约"<<endl;
		
	}

	return dblMax;

}

// 计算价格的最低值
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
		//找到Key，计算指标
	
		// 如果 vector 的数量不足，则返回均值为0
		if (iterMap->second.size() < intNum2)
			return 0.0;

		// 如果数量足够


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
		//没找到,不计算指标
		cerr<<"CalcLastPriceMax: mapQuotes中不存在该合约"<<endl;
		
	}

	if (dblMin >= 1000000)
		dblMin = 0;

	return dblMin;


}




// 根据指标vector，存入指标mapQuotes
void FQuote::Update2mapIndicators(const IndicatorField& newIndicator)  
{  
  
	if (newIndicator.LastPrice <= 0)
	{
		cerr<<"读取的行情数据有问题，newQuote数据有误！"<<endl;
		return;
	}


	map<string,vector<IndicatorField>>::iterator iter= mapIndicators.find(newIndicator.InstrumentID);

	if(iter != mapIndicators.end()) 
	{
		//找到Key，插入其vecQuotes向量
		iter->second.push_back(newIndicator);
	}
	else 
	{
		//没找到,插入新的Key

		vector<IndicatorField> vecIndicators1; // 容量10万，避免后续重设
		vecIndicators1.push_back(newIndicator);

		mapIndicators.insert(map<string,vector<IndicatorField>>::value_type(newIndicator.InstrumentID, vecIndicators1));

		(--mapIndicators.end())->second.reserve(100000); // 为每一个新合约map的第二个变量（vector）设置大容量，避免频繁扩容

		//MapIndicatorsType::iterator iter2 = mapIndicators.end();
		//(*iter2).second.reserve();


	}

	return;

} 


// 获取T时刻的最新指标（T=0,1,2,3...）
void FQuote::GetInstNewIndicator(const string instrumentID, IndicatorField& newIndicator, const int intT) 
{  
  
	if (instrumentID.length() <= 0)
	{
		cerr<<"GetInstNewIndicator():InstrumentID有误！"<<endl;
		return;
	}


	map<string,vector<IndicatorField>>::iterator iter= mapIndicators.find(instrumentID);

	if(iter != mapIndicators.end()) 
	{
		//找到Key，获取第T个最新的指标向量
	
		memset(&newIndicator,0,sizeof(IndicatorField));
		memcpy(&newIndicator,&(iter->second.at(iter->second.size() -1 - intT)),sizeof(newIndicator));

		
	}
	else 
	{
		memset(&newIndicator,0,sizeof(IndicatorField));
	}

	return;

} 




// 初始化 新的一天 的行情 、指标
void FQuote::InitNewDayQuotes()
{
	// 清空 行情map
	mapQuotes.clear();

	// 清空 指标map
	mapIndicators.clear();
	
	return;

}

// 初始化 newIndicator 结构向量
void FQuote::InitNewIndicator(IndicatorField& newIndicator)
{
	//交易时间  yyyy-mm-dd hh:mm:ss.000
    newIndicator.TradingTime = "";  

	///合约代码
	newIndicator.InstrumentID = "";

	///最新价
	newIndicator.LastPrice = 0;

	///申买价一
	newIndicator.BidPrice1 = 0;
	///申买量一
	newIndicator.BidVolume1 = 0;
	///申卖价一
	newIndicator.AskPrice1 = 0;
	///申卖量一
	newIndicator.AskVolume1 = 0;

	newIndicator.Avg10 = 0;
	newIndicator.Avg120 = 0;
	newIndicator.Avg240 = 0;
	newIndicator.AvgDay = 0;

	newIndicator.MaxPrice = 0;
	newIndicator.MinPrice = 0;

	newIndicator.BuySell = "";
	
}


