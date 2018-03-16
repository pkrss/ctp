#include <structs.h>

#pragma once

class FQuotes
{

public:

	FQuotes(void);
	~FQuotes(void);

	// 成员函数

	// 插入最新行情至向量
	int InsertQuotes(QuoteDataField quote);  

	// 获取向量全部数据
	vector<QuoteDataField>* GetQuotes();	

	
	//// 数据成员
	//vector<quote> vecQuotes			//行情数据，tick
	//vector<quote> vecLastQuotes		//最新行情，tick
	//vector<quote> vecCloseQuotes	//收盘行情，tick


private:

	vector<QuoteDataField> vecQuotes;
	QuoteDataField Quote


};
