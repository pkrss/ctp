// ATS_BackTesting.cpp : �������̨Ӧ�ó������ڵ㡣
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
	// ��ʼ����Լ
	myStrategy.InitMapInstruments();

	clock_t tStart,tFinish;
	double tPassed =0;

	//vector<string> vecRowData;  // ÿ�����ݲ�ֺ��������
    string strLine;				// ��ȡÿ������ 
    string strFlag(",");		// �ָ�����ÿ�������У���ָͬ��֮��ķָ���
    long intNum = 0;			// ����ͳ��
	
	TradingDate = "";  // ������ yyyy-mm-dd

	// ---- ��ʼ�� ���� ----------------------------------//
	//InitStrategy();


	tStart = clock();

	// ---- ����Դ�ļ� -----------------------------------// 
	ifstream infile(myStrategy.DataFile, ios::in);  
	vector<string> vecRowData;
	
    if(infile.good())  
    {  
		// ==== ��ȡ�������� �� �ṹ������Ȼ�����д��� ==================================//
        while(!infile.fail())  
        {  
			//if(intNum == 36)
			//	int intY = 0;


			try
			{
				memset(&vecRowData,0,sizeof(vecRowData));

				// vecRowData.clear(); //���� �ڴ�д�����
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
			//----�����ݲ�֡���ϳ�����ṹ����--------------------------------//
			if (vecRowData.size() > 1)    // �������ǿ�ֵʱ���Ŵ��� �� ����
				Convert2NewQuote(vecRowData, newQuote);

			

			//----�ж� �Ƿ��µ�һ��: ���ǣ���ʼ������ȱ���--------------------------//
			if (newQuote.TradingTime.length() > 10)
			{
				if (TradingDate == "" )  // ��������ʱ
				{
					//���ý�������
					TradingDate = newQuote.TradingTime.substr(0,10);

					// ���õ��쿪�̼�
					myQuotes.dayOpenPrice = newQuote.LastPrice;
					//myQuotes.dayOpenPrice = newQuote.LastPrice * 1.01;
					//myQuotes.dayOpenPrice = newQuote.LastPrice * 0.99;

					// ��ʼ�� ���ս��ױ���
					myStrategy.dayTradingNum = 0;

					// ��ʾʱ��
					tFinish = clock();
					tPassed = (double)(tFinish - tStart)/CLOCKS_PER_SEC;
					cerr<<TradingDate<<": passed "<<tPassed<<" seconds"<<endl;
					//system("pause");
				}
				else 
					if(TradingDate < newQuote.TradingTime.substr(0,10))  // ����ѭ�����µ�һ�����ʼ
					{
						// ��ʾʱ��
						tFinish = clock();
						tPassed = (double)(tFinish - tStart)/CLOCKS_PER_SEC;
						cerr<<TradingDate<<": passed "<<tPassed<<" seconds"<<endl;
						//system("pause");


						// ����ӯ��������ͳ��
						myStrategy.DailyStat(TradingDate);

						// �µ�һ�쿪ʼ
						TradingDate = newQuote.TradingTime.substr(0,10);

						// ���õ��쿪�̼�
						myQuotes.dayOpenPrice = newQuote.LastPrice;
						//myQuotes.dayOpenPrice = newQuote.LastPrice * 1.01;
						//myQuotes.dayOpenPrice = newQuote.LastPrice * 0.99;

						// ��ʼ�� ���ս��ױ���
						myStrategy.dayTradingNum = 0;

						// ��ʼ������
						myQuotes.InitNewDayQuotes();

					}

			} // if (newQuote.TradingTime.length() > 10)
			else
			{
				continue; // ��������ѭ��
			}

						

			// ---- ��������ĺ�ԼID������newInstrument 
			if(newQuote.InstrumentID.length() > 1 && newQuote.InstrumentID != myStrategy.newInstrument.InstrumentID)
			{

				myStrategy.GetNewInstrument(newQuote.InstrumentID, myStrategy.newInstrument);

				// ����FStrategy���newInstrument
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

			//----������������µ� mapQuotes ------------------------------------//
			if (newQuote.InstrumentID != "")  // �� ��Լ���� ��Ϊ��ʱ
				myQuotes.Update2mapNewQuotes(newQuote);


			//----����ָ��-------------------------------------------------------//
			if (newQuote.InstrumentID != "")  // �� ��Լ���� ��Ϊ��ʱ
				myQuotes.CalcIndicators(newQuote);

			// ---- ���ù������ -----------------------------------------------//
			// ��FStrategy��ĳ�Աָ�� ָ��������
			myStrategy.SetFAccount(&myAccount);

			// ��FStrategy��ĳ�Աָ�� ָ��������
			myStrategy.SetFQuote(&myQuotes);
						

			// ---- ��������ָ�ִ꣬�н��ײ��� -------------------------------------//
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
		// ==== ��ȡ�������� �� �ṹ���� end ==============================================//
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


// ��ÿһ���������ݲ�ֳɲ�ͬ�ı���������vector
void split(const string& strSrc, const string& strFlag, vector<string>& vecDest)  
{  
    string str = strSrc;  
    string::size_type start = 0, index;  
    string substr;  
  
    index = str.find_first_of(strFlag, start);    //��str�в���(��ʼ��start) delim�������ַ��ĵ�һ�γ��ֵ�λ��  
    while(index != string::npos)  
    {  
        substr = str.substr(start, index-start);  
        vecDest.push_back(substr); 

        start = str.find_first_not_of(strFlag, index);    //��str�в���(��ʼ��index) ��һ��������delim���ַ����ֵ�λ��  

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





// ��������vector����Ӧָ�꣬������������vecQuotes
void Convert2NewQuote(const vector<string>& dest, QuoteField& newQuote)  
{  
	// ��Դ dest �����ݽṹ
	//�г�����,��Լ����,ʱ��,����,�ֲ�,����,�ɽ���,�ɽ���,����,ƽ��,�ɽ�����,����,��һ��,��һ��,��һ��,��һ��
	//sc,rb1407,2014-01-02 10:01:32,3614.0000,230,0,72280.0000,2.0000,1,1,�໻,B,3614.0000,3615.0000,1,1
	//sc,rb1407,2014-01-02 10:01:32,3614.0000,232,2,72280.0000,2.0000,2,0,˫��,B,3594.0000,3628.0000,1,1

  
	if (dest.size() < 16)
	{
		cerr<<"dest ��������,"<<dest.size()<<endl;
		return;
	}

	// ��ʼ������ṹ����
	InitNewQuote(newQuote);

	//���������룺SHFE/DCE/CZCE/CFFEX(������/����/֣��/�н���)
	if (dest[0] == "sc")
	    newQuote.ExchangeID="SHFE";
	else if (dest[0] == "dc")
		newQuote.ExchangeID="DCE";
	else if (dest[0] == "zc")
		newQuote.ExchangeID="CZCE";
	else
	{
		return ;     // ���û�л�ȡ���������룬������������飬�ܿ����Ǳ�����
		newQuote.ExchangeID="CFFEX";
	}

	// ��Լ����
	newQuote.InstrumentID=dest[1];

	// ʱ��
	newQuote.TradingTime = dest[2];

	// ����
	newQuote.LastPrice = atof(dest[3].c_str());

	// �ֲ�
	newQuote.OpenInterest = atof(dest[4].c_str());

	// ���֣�
	newQuote.AddPosition = atof(dest[5].c_str());

	// �ɽ���
	newQuote.Turnover = atof(dest[6].c_str());

	//�ɽ���
	newQuote.Volume = atoi(dest[7].c_str());

	//����
	newQuote.OpenPosition = atof(dest[8].c_str());

	//ƽ��
	newQuote.ClosePosition = atof(dest[9].c_str());

	//�ɽ�����
	newQuote.TradingType = dest[10];

	//���� 
	newQuote.BuySell = dest[11];

	//��һ��
	newQuote.BidPrice1 = atof(dest[12].c_str());
	//��һ��
	newQuote.AskPrice1 = atof(dest[13].c_str());
	//��һ��
	newQuote.BidVolume1 = atoi(dest[14].c_str());
	//��һ��
	newQuote.AskVolume1 = atoi(dest[15].c_str());
	
	return ;

} 


//// ��������vector����Ӧָ�꣬������������vecQuotes
//void Update2vecQuotes(const vector<string>& dest)  
//{  
//	// ��Դ dest �����ݽṹ
//	//�г�����,��Լ����,ʱ��,����,�ֲ�,����,�ɽ���,�ɽ���,����,ƽ��,�ɽ�����,����,��һ��,��һ��,��һ��,��һ��
//	//sc,rb1407,2014-01-02 10:01:32,3614.0000,230,0,72280.0000,2.0000,1,1,�໻,B,3614.0000,3615.0000,1,1
//	//sc,rb1407,2014-01-02 10:01:32,3614.0000,232,2,72280.0000,2.0000,2,0,˫��,B,3594.0000,3628.0000,1,1
//
//  
//	if (dest.size() < 16)
//	{
//		cerr<<"dest ��������,"<<dest.size()<<endl;
//		return;
//	}
//
//	// ��ʼ������ṹ����
//	InitNewQuote(newQuote);
//
//	//���������룺SHFE/DCE/CZCE/CFFEX(������/����/֣��/�н���)
//	if (dest[0] == "sc")
//	    strcpy_s(newQuote.ExchangeID,"SHFE");
//	else if (dest[0] == "dc")
//		strcpy_s(newQuote.ExchangeID,"DCE");
//	else if (dest[0] == "zc")
//		strcpy_s(newQuote.ExchangeID,"CZCE");
//	else
//	{
//		return ;     // ���û�л�ȡ���������룬������������飬�ܿ����Ǳ�����
//		strcpy_s(newQuote.ExchangeID,"CFFEX");
//	}
//
//	// ��Լ����
//	string strX = dest[1];
//
//	strcpy_s(newQuote.InstrumentID,dest[1].c_str());
//
//	// ʱ��
//	newQuote.TradingTime = dest[2];
//
//	// ����
//	newQuote.LastPrice = atof(dest[3].c_str());
//
//	// �ֲ�
//	newQuote.OpenInterest = atof(dest[4].c_str());
//
//	// ���֣�
//	newQuote.AddPosition = atof(dest[5].c_str());
//
//	// �ɽ���
//	newQuote.Turnover = atof(dest[6].c_str());
//
//	//�ɽ���
//	newQuote.Volume = atoi(dest[7].c_str());
//
//	//����
//	newQuote.OpenPosition = atof(dest[8].c_str());
//
//	//ƽ��
//	newQuote.ClosePosition = atof(dest[9].c_str());
//
//	//�ɽ�����
//	newQuote.TradingType = dest[10];
//
//	//���� 
//	newQuote.BuySell = dest[11];
//
//	//��һ��
//	newQuote.BidPrice1 = atof(dest[12].c_str());
//	//��һ��
//	newQuote.AskPrice1 = atof(dest[13].c_str());
//	//��һ��
//	newQuote.BidVolume1 = atoi(dest[14].c_str());
//	//��һ��
//	newQuote.AskVolume1 = atoi(dest[15].c_str());
//
//
//	// ��������
//	vecQuotes.push_back(newQuote);
//
// 
//} 








//// ��ʼ������
//void InitStrategy()
//{
//	// ��ʼ�� ��Լ
//	InitInstruments();
//
//	
//	// ��ʼ�� ���Բ���
//	//myStrategy.Strategy.Money = 100000; // ��ʼ�ʽ�
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
//	// ��ʼ�� �˻� 
//	
//	/////Ͷ���ߴ���
//	//myAccount.Account.InvestorID = "Kevin2002";
//
//	/////����ID
//	//myAccount.Account.StrategyID ="R-BreakerM1";
//		
//	///�����
//	myAccount.Account.Deposit = myStrategy.Strategy.Money;   // ��ʼ�ʽ�
//
//	///����ı�֤��
//	myAccount.Account.FrozenMargin = 0;
//
//	///�����������
//	myAccount.Account.FrozenCommission = 0;
//		
//	///��ǰ��֤���ܶ�
//	myAccount.Account.CurrMargin = 0;
//	
//	///������
//	myAccount.Account.Commission = 0;
//
//	///ƽ��ӯ��
//	myAccount.Account.CloseProfit = 0;
//	///�ֲ�ӯ��
//	myAccount.Account.PositionProfit = 0;
//		
//	///�����ʽ�
//	myAccount.Account.Available = myAccount.Account.Deposit;  
//	
//	///�䶯ʱ��
//	myAccount.Account.UpdateTime = "";
//
//
//
//
//
//
//}




// ��ʼ�� newQuote �ṹ����
void InitNewQuote(QuoteField& newQuote)
{
	//���������룺SHFE/DCE/CZCE/CFFEX(������/����/֣��/�н���)
	newQuote.ExchangeID = "";

	// ��Լ����
	newQuote.InstrumentID = "";

	// ʱ��
	newQuote.TradingTime = "";

	// ����
	newQuote.LastPrice = 0;

	// �ֲ�
	newQuote.OpenInterest = 0;

	// ���֣�
	newQuote.AddPosition = 0;

	// �ɽ���
	newQuote.Turnover = 0;

	//�ɽ���
	newQuote.Volume = 0;

	//����
	newQuote.OpenPosition = 0;

	//ƽ��
	newQuote.ClosePosition = 0;

	//�ɽ�����
	newQuote.TradingType = "";

	//���� 
	newQuote.BuySell = "";

	//��һ��
	newQuote.BidPrice1 = 0;
	//��һ��
	newQuote.AskPrice1 = 0;
	//��һ��
	newQuote.BidVolume1 = 0;
	//��һ��
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
//		 // ����ҵ���
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


//	//.csv���ݸ�ʽ
//	//�г�����,��Լ����,ʱ��,����,�ֲ�,����,�ɽ���,�ɽ���,����,ƽ��,�ɽ�����,����,��һ��,��һ��,��һ��,��һ��
//	//sc,rb1407,2014-01-02 10:01:32,3614.0000,230,0,72280.0000,2.0000,1,1,�໻,B,3614.0000,3615.0000,1,1
//
//	char chrLine[300];
//	string strLine;
//	bool ifGotten = false;
//
//	// ��Ҫ��ȡ�ı���
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
//		// ��������ˡ�,�����������
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
//	vector<string> vecRowData;  // ÿ�����ݲ�ֺ��������
//    string strLine;				// ��ȡÿ������ 
//    string strFlag(",");		// �ָ�����ÿ�������У���ָͬ��֮��ķָ���
//	char strLineFlag('\n');
//    long intNum = 0;			// ����ͳ��
//	
//	TradingDate = "";  // ������ yyyy-mm-dd
//
//	// ---- ��ʼ�� ���� ----------------------------------//
//	InitStrategy();
//
//	tStart = clock();
//
//	// ---- ����Դ�ļ� -----------------------------------// 
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
//		// ���
//		memset(&vecRowData,0,sizeof(vecRowData));
//
//		// ���ж�ȡ
//		strLine = strBuf.substr(start,intIndex - start);
//
//		// ���в��������
//		split(strLine,strFlag,vecRowData);
//
//
//		//----�����ݲ�֡���ϳ�����ṹ����--------------------------------//
//		if (vecRowData.size() > 1)    // �������ǿ�ֵʱ���Ŵ��� �� ����
//			Convert2NewQuote(vecRowData, newQuote);
//
//
//
//		//----�ж� �Ƿ��µ�һ��: ���ǣ���ʼ������ȱ���--------------------------//
//		if (newQuote.TradingTime.length() > 10)
//		{
//				if (TradingDate == "" )  // ��������ʱ
//					TradingDate = newQuote.TradingTime.substr(0,10);
//				else 
//					if(TradingDate < newQuote.TradingTime.substr(0,10))  // ����ѭ�����µ�һ�����ʼ
//					{
//						// �µ�һ�쿪ʼ
//						TradingDate = newQuote.TradingTime.substr(0,10);
//
//						// ��ʼ������
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
//		//----������������µ� mapQuotes ------------------------------------//
//		if (newQuote.InstrumentID != "")  // �� ��Լ���� ��Ϊ��ʱ
//				Update2mapQuotes(newQuote);
//
//
//		//----����ָ��-------------------------------------------------------//
//		if (newQuote.InstrumentID != "")  // �� ��Լ���� ��Ϊ��ʱ
//				CalcIndicators(newQuote,newIndicator);
//
//
//		//----������ָ����µ� mapIndicators ------------------------------------//
//		if (newQuote.InstrumentID != "")  // �� ��Լ���� ��Ϊ��ʱ
//				Update2mapIndicators(newIndicator);
//
//		// ---- ִ�н��ײ��� ------------------------------------------------------//
//		ExecStrategy();
//
//
//
//
//
//		intNum +=1;
//		cerr<<intNum<<endl;
//
//		// ѭ���� ��һ��
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

