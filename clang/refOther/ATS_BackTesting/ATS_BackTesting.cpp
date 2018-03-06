// ATS_BackTesting.cpp : 定义控制台应用程序的入口点。
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h> 
#include <vector>
#include <map>
#include <time.h>

#include "ATS_BackTesting.h"


using namespace std;


int main()
{
	// 初始化合约
	myStrategy.InitMapInstruments();

	clock_t tStart,tFinish;
	double tPassed =0;

	//vector<string> vecRowData;  // 每行数据拆分后存入向量
    string strLine;				// 读取每行数据 
    string strFlag(",");		// 分隔符，每行数据中，不同指标之间的分隔符
    long intNum = 0;			// 行数统计
	
	TradingDate = "";  // 交易日 yyyy-mm-dd

	// ---- 初始化 策略 ----------------------------------//
	//InitStrategy();


	tStart = clock();

	// ---- 数据源文件 -----------------------------------// 
	ifstream infile(myStrategy.DataFile, ios::in);  
	vector<string> vecRowData;
	
    if(infile.good())  
    {  
		// ==== 读取行情数据 至 结构向量，然后逐行处理 ==================================//
        while(!infile.fail())  
        {  
			//if(intNum == 36)
			//	int intY = 0;


			try
			{
				memset(&vecRowData,0,sizeof(vecRowData));

				// vecRowData.clear(); //报错 内存写入错误
				//vecRowData.erase(vecRowData.begin(),vecRowData.end());

				getline(infile, strLine);  
				split(strLine, strFlag, vecRowData);  

				//cerr<<textline<<endl;
			
			}
			catch(exception e)
			{
				cerr<<"split Error: "<<e.what()<<endl;

			}


	  try
	  {
			//----行数据拆分、组合成行情结构变量--------------------------------//
			if (vecRowData.size() > 1)    // 向量不是空值时，才传递 给 向量
				Convert2NewQuote(vecRowData, newQuote);

			

			//----判断 是否新的一天: 若是，初始化行情等变量--------------------------//
			if (newQuote.TradingTime.length() > 10)
			{
				if (TradingDate == "" )  // 程序启动时
				{
					//设置交易日期
					TradingDate = newQuote.TradingTime.substr(0,10);

					// 设置当天开盘价
					myQuotes.dayOpenPrice = newQuote.LastPrice;
					//myQuotes.dayOpenPrice = newQuote.LastPrice * 1.01;
					//myQuotes.dayOpenPrice = newQuote.LastPrice * 0.99;

					// 初始化 当日交易笔数
					myStrategy.dayTradingNum = 0;

					// 显示时间
					tFinish = clock();
					tPassed = (double)(tFinish - tStart)/CLOCKS_PER_SEC;
					cerr<<TradingDate<<": passed "<<tPassed<<" seconds"<<endl;
					//system("pause");
				}
				else 
					if(TradingDate < newQuote.TradingTime.substr(0,10))  // 行情循环到新的一天的起始
					{
						// 显示时间
						tFinish = clock();
						tPassed = (double)(tFinish - tStart)/CLOCKS_PER_SEC;
						cerr<<TradingDate<<": passed "<<tPassed<<" seconds"<<endl;
						//system("pause");


						// 日终盈亏及交易统计
						myStrategy.DailyStat(TradingDate);

						// 新的一天开始
						TradingDate = newQuote.TradingTime.substr(0,10);

						// 设置当天开盘价
						myQuotes.dayOpenPrice = newQuote.LastPrice;
						//myQuotes.dayOpenPrice = newQuote.LastPrice * 1.01;
						//myQuotes.dayOpenPrice = newQuote.LastPrice * 0.99;

						// 初始化 当日交易笔数
						myStrategy.dayTradingNum = 0;

						// 初始化变量
						myQuotes.InitNewDayQuotes();

					}

			} // if (newQuote.TradingTime.length() > 10)
			else
			{
				continue; // 跳出本次循环
			}

						

			// ---- 根据行情的合约ID，更新newInstrument 
			if(newQuote.InstrumentID.length() > 1 && newQuote.InstrumentID != myStrategy.newInstrument.InstrumentID)
			{

				myStrategy.GetNewInstrument(newQuote.InstrumentID, myStrategy.newInstrument);

				// 更新FStrategy类的newInstrument
				//myStrategy.UpdateNewInstrument(newInstrument);  

			}
			



			//if (intNum == 3500)
			//{
			//	tFinish = clock();
			//	tPassed = (double)(tFinish - tStart)/CLOCKS_PER_SEC;
			//	cerr<<"1 passed "<<tPassed<<" seconds"<<endl;
			//	system("pause");
			//}


		/*	if (TradingDate == "2014-01-03")
			{
				tFinish = clock();
				tPassed = (double)(tFinish - tStart)/CLOCKS_PER_SEC;
				cerr<<"1 passed "<<tPassed<<" seconds"<<endl;

				system("pause");
			}*/

			if(myStrategy.vecOrders.size()>=10)
				int intY = myStrategy.vecOrders.size();

			//----将最新行情更新到 mapQuotes ------------------------------------//
			if (newQuote.InstrumentID != "")  // 当 合约代码 不为空时
				myQuotes.Update2mapNewQuotes(newQuote);


			//----计算指标-------------------------------------------------------//
			if (newQuote.InstrumentID != "")  // 当 合约代码 不为空时
				myQuotes.CalcIndicators(newQuote);

			// ---- 设置共享的类 -----------------------------------------------//
			// 令FStrategy类的成员指针 指向行情类
			myStrategy.SetFAccount(&myAccount);

			// 令FStrategy类的成员指针 指向行情类
			myStrategy.SetFQuote(&myQuotes);
						

			// ---- 根据最新指标，执行交易策略 -------------------------------------//
			myStrategy.ExecStrategy(myQuotes.newIndicator);


			
	  }
	  catch(exception e)
	  {
		  cerr<<"ExecStrategy Error: "<<e.what()<<endl;

	  }



			intNum += 1;
			
			

			//long intNum2 = myQuotes.mapQuotes.size();
			//long intNum3 = mapIndicators.size();
			long intNum4 = 0;

			cerr<<intNum<<endl;

        } // while(!infile.fail()) 
		// ==== 读取行情数据 至 结构向量 end ==============================================//
		myAccount.PrintAccount();
		myStrategy.PrintVecOrders();
		myStrategy.PrintVecPositionDetails();

		cerr<<"...End..."<<endl;

		

    }  //if(infile.good())  

    infile.close();  
  
	tFinish = clock();
	tPassed = (double)(tFinish - tStart)/CLOCKS_PER_SEC;
	cerr<<"Strategy passed "<<tPassed<<" seconds"<<endl;
	//system("pause");


  //  vector<QuoteField>::iterator iter = vecQuotes.begin();  
  //  while(iter != vecQuotes.end())  
  //  {  
		//cout<<(*iter).InstrumentID<<":"<<(*iter++).TradingTime<<endl;  
  //  }  
  




	return 0;
}


// 将每一行行情数据拆分成不同的变量，存入vector
void split(const string& strSrc, const string& strFlag, vector<string>& vecDest)  
{  
    string str = strSrc;  
    string::size_type start = 0, index;  
    string substr;  
  
    index = str.find_first_of(strFlag, start);    //在str中查找(起始：start) delim的任意字符的第一次出现的位置  
    while(index != string::npos)  
    {  
        substr = str.substr(start, index-start);  
        vecDest.push_back(substr); 

        start = str.find_first_not_of(strFlag, index);    //在str中查找(起始：index) 第一个不属于delim的字符出现的位置  

        if(start == string::npos) 
		{
			return;  
		 }
  
        index = str.find_first_of(strFlag, start);  

		if(index == string::npos) 
		{
			substr = str.substr(start, str.size() - start); 
			vecDest.push_back(substr);
		 }


    }  
} 





// 根据行情vector，对应指标，存入行情向量vecQuotes
void Convert2NewQuote(const vector<string>& dest, QuoteField& newQuote)  
{  
	// 来源 dest 的数据结构
	//市场代码,合约代码,时间,最新,持仓,增仓,成交额,成交量,开仓,平仓,成交类型,方向,买一价,卖一价,买一量,卖一量
	//sc,rb1407,2014-01-02 10:01:32,3614.0000,230,0,72280.0000,2.0000,1,1,多换,B,3614.0000,3615.0000,1,1
	//sc,rb1407,2014-01-02 10:01:32,3614.0000,232,2,72280.0000,2.0000,2,0,双开,B,3594.0000,3628.0000,1,1

  
	if (dest.size() < 16)
	{
		cerr<<"dest 数据有误,"<<dest.size()<<endl;
		return;
	}

	// 初始化行情结构变量
	InitNewQuote(newQuote);

	//交易所代码：SHFE/DCE/CZCE/CFFEX(上期所/大连/郑州/中金所)
	if (dest[0] == "sc")
	    newQuote.ExchangeID="SHFE";
	else if (dest[0] == "dc")
		newQuote.ExchangeID="DCE";
	else if (dest[0] == "zc")
		newQuote.ExchangeID="CZCE";
	else
	{
		return ;     // 如果没有获取交易所代码，则放弃本笔行情，很可能是标题行
		newQuote.ExchangeID="CFFEX";
	}

	// 合约代码
	newQuote.InstrumentID=dest[1];

	// 时间
	newQuote.TradingTime = dest[2];

	// 最新
	newQuote.LastPrice = atof(dest[3].c_str());

	// 持仓
	newQuote.OpenInterest = atof(dest[4].c_str());

	// 增仓：
	newQuote.AddPosition = atof(dest[5].c_str());

	// 成交额
	newQuote.Turnover = atof(dest[6].c_str());

	//成交量
	newQuote.Volume = atoi(dest[7].c_str());

	//开仓
	newQuote.OpenPosition = atof(dest[8].c_str());

	//平仓
	newQuote.ClosePosition = atof(dest[9].c_str());

	//成交类型
	newQuote.TradingType = dest[10];

	//方向 
	newQuote.BuySell = dest[11];

	//买一价
	newQuote.BidPrice1 = atof(dest[12].c_str());
	//卖一价
	newQuote.AskPrice1 = atof(dest[13].c_str());
	//买一量
	newQuote.BidVolume1 = atoi(dest[14].c_str());
	//卖一量
	newQuote.AskVolume1 = atoi(dest[15].c_str());
	
	return ;

} 


//// 根据行情vector，对应指标，存入行情向量vecQuotes
//void Update2vecQuotes(const vector<string>& dest)  
//{  
//	// 来源 dest 的数据结构
//	//市场代码,合约代码,时间,最新,持仓,增仓,成交额,成交量,开仓,平仓,成交类型,方向,买一价,卖一价,买一量,卖一量
//	//sc,rb1407,2014-01-02 10:01:32,3614.0000,230,0,72280.0000,2.0000,1,1,多换,B,3614.0000,3615.0000,1,1
//	//sc,rb1407,2014-01-02 10:01:32,3614.0000,232,2,72280.0000,2.0000,2,0,双开,B,3594.0000,3628.0000,1,1
//
//  
//	if (dest.size() < 16)
//	{
//		cerr<<"dest 数据有误,"<<dest.size()<<endl;
//		return;
//	}
//
//	// 初始化行情结构变量
//	InitNewQuote(newQuote);
//
//	//交易所代码：SHFE/DCE/CZCE/CFFEX(上期所/大连/郑州/中金所)
//	if (dest[0] == "sc")
//	    strcpy_s(newQuote.ExchangeID,"SHFE");
//	else if (dest[0] == "dc")
//		strcpy_s(newQuote.ExchangeID,"DCE");
//	else if (dest[0] == "zc")
//		strcpy_s(newQuote.ExchangeID,"CZCE");
//	else
//	{
//		return ;     // 如果没有获取交易所代码，则放弃本笔行情，很可能是标题行
//		strcpy_s(newQuote.ExchangeID,"CFFEX");
//	}
//
//	// 合约代码
//	string strX = dest[1];
//
//	strcpy_s(newQuote.InstrumentID,dest[1].c_str());
//
//	// 时间
//	newQuote.TradingTime = dest[2];
//
//	// 最新
//	newQuote.LastPrice = atof(dest[3].c_str());
//
//	// 持仓
//	newQuote.OpenInterest = atof(dest[4].c_str());
//
//	// 增仓：
//	newQuote.AddPosition = atof(dest[5].c_str());
//
//	// 成交额
//	newQuote.Turnover = atof(dest[6].c_str());
//
//	//成交量
//	newQuote.Volume = atoi(dest[7].c_str());
//
//	//开仓
//	newQuote.OpenPosition = atof(dest[8].c_str());
//
//	//平仓
//	newQuote.ClosePosition = atof(dest[9].c_str());
//
//	//成交类型
//	newQuote.TradingType = dest[10];
//
//	//方向 
//	newQuote.BuySell = dest[11];
//
//	//买一价
//	newQuote.BidPrice1 = atof(dest[12].c_str());
//	//卖一价
//	newQuote.AskPrice1 = atof(dest[13].c_str());
//	//买一量
//	newQuote.BidVolume1 = atoi(dest[14].c_str());
//	//卖一量
//	newQuote.AskVolume1 = atoi(dest[15].c_str());
//
//
//	// 插入向量
//	vecQuotes.push_back(newQuote);
//
// 
//} 








//// 初始化策略
//void InitStrategy()
//{
//	// 初始化 合约
//	InitInstruments();
//
//	
//	// 初始化 策略参数
//	//myStrategy.Strategy.Money = 100000; // 初始资金
//	myStrategy.Strategy.BSQty = 1;
//	myStrategy.Strategy.ModifyRange = 4;
//	myStrategy.Strategy.ModifyRatio = 0.25;
//	myStrategy.Strategy.Rongrendu = 1.5;
//	myStrategy.Strategy.TradeBegTime = "9:00:02";
//	myStrategy.Strategy.ForceCloseTime = "14:58:00";
//	myStrategy.Strategy.InitOrderRef = 1 ;
//
//	NextOrderRef = myStrategy.Strategy.InitOrderRef ;
//
//	// 初始化 账户 
//	
//	/////投资者代码
//	//myAccount.Account.InvestorID = "Kevin2002";
//
//	/////策略ID
//	//myAccount.Account.StrategyID ="R-BreakerM1";
//		
//	///入金金额
//	myAccount.Account.Deposit = myStrategy.Strategy.Money;   // 初始资金
//
//	///冻结的保证金
//	myAccount.Account.FrozenMargin = 0;
//
//	///冻结的手续费
//	myAccount.Account.FrozenCommission = 0;
//		
//	///当前保证金总额
//	myAccount.Account.CurrMargin = 0;
//	
//	///手续费
//	myAccount.Account.Commission = 0;
//
//	///平仓盈亏
//	myAccount.Account.CloseProfit = 0;
//	///持仓盈亏
//	myAccount.Account.PositionProfit = 0;
//		
//	///可用资金
//	myAccount.Account.Available = myAccount.Account.Deposit;  
//	
//	///变动时间
//	myAccount.Account.UpdateTime = "";
//
//
//
//
//
//
//}




// 初始化 newQuote 结构向量
void InitNewQuote(QuoteField& newQuote)
{
	//交易所代码：SHFE/DCE/CZCE/CFFEX(上期所/大连/郑州/中金所)
	newQuote.ExchangeID = "";

	// 合约代码
	newQuote.InstrumentID = "";

	// 时间
	newQuote.TradingTime = "";

	// 最新
	newQuote.LastPrice = 0;

	// 持仓
	newQuote.OpenInterest = 0;

	// 增仓：
	newQuote.AddPosition = 0;

	// 成交额
	newQuote.Turnover = 0;

	//成交量
	newQuote.Volume = 0;

	//开仓
	newQuote.OpenPosition = 0;

	//平仓
	newQuote.ClosePosition = 0;

	//成交类型
	newQuote.TradingType = "";

	//方向 
	newQuote.BuySell = "";

	//买一价
	newQuote.BidPrice1 = 0;
	//卖一价
	newQuote.AskPrice1 = 0;
	//买一量
	newQuote.BidVolume1 = 0;
	//卖一量
	newQuote.AskVolume1 = 0;


}










//string readIniFile(ifstream &T,string strKey)
//{
//	char chr[300];
//	string strValue;
//	bool ifGotten = false;
//
//	while(!T.eof())
//	{
//		//T.get(ch);
//		T.getline(chr,300);
//
//		int intX = strlen(chr);
//
//		if (strlen(chr) < strKey.length())
//			break;
//
//		ifGotten = false;
//		unsigned int intJ = 0;
//
//        for(unsigned int i=0; i<strlen(chr); ++i)
//		{
//			
//           if(chr[0] == ';' || chr[0] == '[')
//		   {
//			   break;
//		   }
//
//		   int intY = strKey.length();
//
//		   if(i < strKey.length())
//		   {
//			   if( strKey[i] != chr[i])
//			   {
//			     break;
//			   }
//		   }
//		   else
//		   {
//			   
//			   if (chr[i] == '=' && ifGotten == false)
//			   {
//				   ifGotten = true;
//			   }
//			   else if(ifGotten)
//			   {
//				   strValue = strValue + chr[i];
//				   intJ += 1;
//			   }
//			   else
//			   {
//				   break;
//			   }
//
//		   }
//
//		  
//
//
//		} // for
//
//		 // 如果找到了
//		   if (ifGotten)  
//		   {
//			   strValue = strValue + '\0';
//			   return strValue;
//		   }
//
//
//		//cout << chr;
//
//	}  // while
//
//	return "";
//}

//string readCsvFile(ifstream &iFile)
//{

	/*char strKey[100] = "instB";
	ifstream ifCsvFile("SCrb1407_201401.csv");

	string str = "";
	str = readCsvFile(ifCsvFile);
	
	int intX = 0;
	intX = 1;*/


//	//.csv数据格式
//	//市场代码,合约代码,时间,最新,持仓,增仓,成交额,成交量,开仓,平仓,成交类型,方向,买一价,卖一价,买一量,卖一量
//	//sc,rb1407,2014-01-02 10:01:32,3614.0000,230,0,72280.0000,2.0000,1,1,多换,B,3614.0000,3615.0000,1,1
//
//	char chrLine[300];
//	string strLine;
//	bool ifGotten = false;
//
//	// 需要获取的变量
//	QuoteField newQuotes;
//
//
//	while(!iFile.eof())
//	{
//		//T.get(ch);
//		iFile.getline(chrLine,300);
//		strLine = chrLine;
//
//
//		if (strLine.size() < 1)
//			continue;
//
//		int intPosi1 = 0;
//		int intPosi2 = strLine.find_first_of(',',intPosi1+1);
//
//		ifGotten = false;
//		unsigned int intNum = 0;
//		string strValue = "";
//
//		// 如果发现了‘,’则继续处理
//		while (intPosi2 >=0)
//		{
//			intNum += 1;
//			strValue = "";
//
//			strValue = strLine.substr(intPosi1, intPosi2 - intPosi1);
//			//if(strValue = "")
//			switch(intNum)
//			{
//
//				case 1:
//						strcpy_s(newQuotes.ExchangeID,strValue.c_str());
//						break;
//				case 2:
//						strcpy_s(newQuotes.InstrumentID,strValue.c_str());
//						break;
//				case 3:
//						newQuotes.TradingTime = strValue;
//						break;
//				case 4:
//						newQuotes.LastPrice = atof(strValue.c_str());
//						break;
//
//				default:
//					cerr<<"Error case."<<endl;
//
//			}
//			
//
//		  intPosi1 = intPosi2 + 1;
//		  intPosi2 = strLine.find_first_of(',', intPosi1);
//
//
//		} // while 
//
//       
//
//		//cout << chr;
//
//	}  // while
//
//	return "";
//}


//int main2()
//{
//	const int MAXS = 60*1024*1024;
//	char buf[MAXS];

//	clock_t tStart,tFinish;
//	double tPassed;
//
//	vector<string> vecRowData;  // 每行数据拆分后存入向量
//    string strLine;				// 读取每行数据 
//    string strFlag(",");		// 分隔符，每行数据中，不同指标之间的分隔符
//	char strLineFlag('\n');
//    long intNum = 0;			// 行数统计
//	
//	TradingDate = "";  // 交易日 yyyy-mm-dd
//
//	// ---- 初始化 策略 ----------------------------------//
//	InitStrategy();
//
//	tStart = clock();
//
//	// ---- 数据源文件 -----------------------------------// 
//	freopen("SCrb1405_201401.csv","rb",stdin);
//    int len = fread(buf,1,MAXS,stdin);
//
//    buf[len] = '\0';
//    
//	string strBuf;
//	strBuf = buf;
//
//	int start = 0;
//	int intIndex = strBuf.find_first_of('\n',start);
//
//	while (intIndex != -1)
//	{
//		// 清空
//		memset(&vecRowData,0,sizeof(vecRowData));
//
//		// 逐行读取
//		strLine = strBuf.substr(start,intIndex - start);
//
//		// 逐行拆分至向量
//		split(strLine,strFlag,vecRowData);
//
//
//		//----行数据拆分、组合成行情结构变量--------------------------------//
//		if (vecRowData.size() > 1)    // 向量不是空值时，才传递 给 向量
//			Convert2NewQuote(vecRowData, newQuote);
//
//
//
//		//----判断 是否新的一天: 若是，初始化行情等变量--------------------------//
//		if (newQuote.TradingTime.length() > 10)
//		{
//				if (TradingDate == "" )  // 程序启动时
//					TradingDate = newQuote.TradingTime.substr(0,10);
//				else 
//					if(TradingDate < newQuote.TradingTime.substr(0,10))  // 行情循环到新的一天的起始
//					{
//						// 新的一天开始
//						TradingDate = newQuote.TradingTime.substr(0,10);
//
//						// 初始化变量
//						InitNewDay();
//
//					}
//
//		} // if (newQuote.TradingTime.length() > 10)
//
//		if (intNum == 10000)
//			{
//
//				tFinish = clock();
//
//				tPassed = (double)(tFinish - tStart)/CLOCKS_PER_SEC;
//				cerr<<"1 passed "<<tPassed<<" seconds"<<endl;
//				system("pause");
//			}
//
//
//		if (TradingDate == "2014-01-03")
//			{
//
//				tFinish = clock();
//
//				tPassed = (double)(tFinish - tStart)/CLOCKS_PER_SEC;
//				cerr<<"2 passed "<<tPassed<<" seconds"<<endl;
//				system("pause");
//			}
//
//
//
//		//----将最新行情更新到 mapQuotes ------------------------------------//
//		if (newQuote.InstrumentID != "")  // 当 合约代码 不为空时
//				Update2mapQuotes(newQuote);
//
//
//		//----计算指标-------------------------------------------------------//
//		if (newQuote.InstrumentID != "")  // 当 合约代码 不为空时
//				CalcIndicators(newQuote,newIndicator);
//
//
//		//----将最新指标更新到 mapIndicators ------------------------------------//
//		if (newQuote.InstrumentID != "")  // 当 合约代码 不为空时
//				Update2mapIndicators(newIndicator);
//
//		// ---- 执行交易策略 ------------------------------------------------------//
//		ExecStrategy();
//
//
//
//
//
//		intNum +=1;
//		cerr<<intNum<<endl;
//
//		// 循环至 下一行
//		start = intIndex+1;
//		intIndex = strBuf.find_first_of('\n',start);
//
//
//	} //while (intIndex != -1)
//	
//  
//	cerr<<"...End..."<<endl;
//
//
//
//  //  vector<QuoteField>::iterator iter = vecQuotes.begin();  
//  //  while(iter != vecQuotes.end())  
//  //  {  
//		//cout<<(*iter).InstrumentID<<":"<<(*iter++).TradingTime<<endl;  
//  //  }  
//  
//		
//
//	return 0;
//}

