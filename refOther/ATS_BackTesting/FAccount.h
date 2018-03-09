#pragma once
#include <iostream>
#include <fstream>
#include "structs.h"


class FAccount
{

public:

	// ���캯������ʼ���˻���Ϣ
	FAccount(string investorId,double money);

	//������������ʲô
	~FAccount(void);

	// ��ʼ�� �ʽ𡢲���ID
	void InitAccount(string strategyId, double initMoney);

	// ����ί�б����������˻��ʽ�
	void UpdateAccountByOrder(const OrderField& order);
	
	// ��ȡ��ǰ�����ʽ�
	double GetAvailableCash();

	// ���ʽ�䶯��������log�ļ�
	void PrintAccount();

	// ��ȡϵͳ��ǰʱ��ĺ�����
	string GetNowMillisecond(void);

public:
	
	// �˻�����
	string InvestorID;

private:
	// �˻���Ϣ
	AccountField Account;




};

