#include <structs.h>

#pragma once

class FQuotes
{

public:

	FQuotes(void);
	~FQuotes(void);

	// ��Ա����

	// ������������������
	int InsertQuotes(QuoteDataField quote);  

	// ��ȡ����ȫ������
	vector<QuoteDataField>* GetQuotes();	

	
	//// ���ݳ�Ա
	//vector<quote> vecQuotes			//�������ݣ�tick
	//vector<quote> vecLastQuotes		//�������飬tick
	//vector<quote> vecCloseQuotes	//�������飬tick


private:

	vector<QuoteDataField> vecQuotes;
	QuoteDataField Quote


};
