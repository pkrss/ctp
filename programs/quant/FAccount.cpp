#include <Windows.h>
#include "FAccount.h"
#include <iomanip>

// ���캯������ʼ���˻���Ϣ
FAccount::FAccount(string investorId,double money)
{

	/////Ͷ���ߴ���
	InvestorID = investorId;
		
	///�����
	Account.Deposit = money;   // ��ʼ�ʽ�

	/// ��ǰ�ʽ��ܶ�
	Account.NowCash = money;

	///����ı�֤��
	Account.FrozenMargin = 0;

	///�����������
	Account.FrozenCommission = 0;
		
	///��ǰ��֤���ܶ�
	Account.CurrMargin = 0;
	
	///������
	Account.Commission = 0;

	///ƽ��ӯ��
	Account.CloseProfit = 0;
	///�ֲ�ӯ��
	Account.PositionProfit = 0;
		
	///�����ʽ�
	Account.Available = money;  
	
	///�䶯ʱ��
	Account.UpdateTime = "";


}

//������������ʲô
FAccount::~FAccount(void)
{
	
}


// ���ݱ����������˻��ʽ�
void FAccount::UpdateAccountByOrder(const OrderField& order)
{
	// ���ֱ���
	if(order.CombOffsetFlag == "OPEN")
	{
		
		if(order.OrderStatus == 1)
		{
			// ���ύ�� ���ᱣ֤��������;
			Account.FrozenMargin += order.FrozenMargin;
			Account.FrozenCommission += order.FrozenCommission;
			
			// �����ʽ�
			// -- ���+ƽ��ӯ��+�ֲ�ӯ��-������ = ��̬Ȩ�棻 
			// -- ��̬Ȩ�� -ռ�ñ�֤�� -���ᱣ֤�� -���������� = �����ʽ�
			Account.Available = Account.Available - order.FrozenMargin - order.FrozenCommission;
			Account.UpdateTime = order.InsertTime;

		}
		else if (order.OrderStatus == 0)  
		{
			// �ɽ��� �ⶳ����ı�֤�������ѣ���ʽ�ۼ���֤�������ѣ�

			// �ⶳ
			Account.FrozenMargin -= order.FrozenMargin;
			Account.FrozenCommission -= order.FrozenCommission;
			
			// �����ʽ�
			// -- ���+ƽ��ӯ��+�ֲ�ӯ��-������ = ��̬Ȩ�棻 
			// -- ��̬Ȩ�� -ռ�ñ�֤�� -���ᱣ֤�� -���������� = �����ʽ�
			Account.Available = Account.Available + order.FrozenMargin + order.FrozenCommission;

			// �ۼ�
			Account.CurrMargin += order.Margin;
			Account.Commission += order.Commission;

			Account.NowCash -= order.Commission;
			Account.Available = Account.Available - order.Margin - order.Commission;

			Account.UpdateTime = order.UpdateTime;
		}
		else // = -1
		{
			// ������ �ⶳ��֤��������;
			// �ⶳ
			Account.FrozenMargin -= order.FrozenMargin;
			Account.FrozenCommission -= order.FrozenCommission;
			
			// �����ʽ�
			// -- ���+ƽ��ӯ��+�ֲ�ӯ��-������ = ��̬Ȩ�棻 
			// -- ��̬Ȩ�� -ռ�ñ�֤�� -���ᱣ֤�� -���������� = �����ʽ�
			Account.Available = Account.Available + order.FrozenMargin + order.FrozenCommission;

			Account.UpdateTime = order.CancelTime;

		} 

	} // if(order.CombOffsetFlag == "OPEN")
	
	else // ƽ�ֱ���
	{
		//CLOSE��
		if(order.OrderStatus == 1)
		{
			// ���ύ�� ����������,�����ᱣ֤��
			//Account.FrozenMargin += order.FrozenMargin;
			Account.FrozenCommission += order.FrozenCommission;
			
			// �����ʽ�
			// -- ���+ƽ��ӯ��+�ֲ�ӯ��-������ = ��̬Ȩ�棻 
			// -- ��̬Ȩ�� -ռ�ñ�֤�� -���ᱣ֤�� -���������� = �����ʽ�
			Account.Available = Account.Available - order.FrozenCommission;

			Account.UpdateTime = order.InsertTime;
		}
		else if (order.OrderStatus == 0)
		{
			// �ɽ��� �ͷű�֤�𣬿ۼ�������;
			// �ⶳ
			//Account.FrozenMargin = 0 ; //-= order.Margin;  ƽ�ֵ������ᱣ֤�� 
			Account.FrozenCommission -= order.FrozenCommission;
			
			// �����ʽ�
			// -- ���+ƽ��ӯ��+�ֲ�ӯ��-������ = ��̬Ȩ�棻 
			// -- ��̬Ȩ�� -ռ�ñ�֤�� -���ᱣ֤�� -���������� = �����ʽ�
			Account.Available = Account.Available  + order.FrozenCommission;

			// �ۼ�
			Account.CurrMargin -= order.Margin;
			Account.Commission += order.Commission;

			Account.CloseProfit += order.CloseProfit;
			Account.PositionProfit = 0;               // ��TODO�� �ݲ�����

			Account.NowCash = Account.NowCash - order.Commission + order.CloseProfit;
			Account.Available = Account.Available + order.Margin - order.Commission + order.CloseProfit;

			Account.UpdateTime = order.UpdateTime;
		}
		else if (order.OrderStatus == -1) // = -1
		{
			// ������ �ⶳ������;
			// �ⶳ
			//Account.FrozenMargin -= order.FrozenMargin;
			Account.FrozenCommission -= order.FrozenCommission;
			
			// �����ʽ�
			// -- ���+ƽ��ӯ��+�ֲ�ӯ��-������ = ��̬Ȩ�棻 
			// -- ��̬Ȩ�� -ռ�ñ�֤�� -���ᱣ֤�� -���������� = �����ʽ�
			Account.Available = Account.Available + order.FrozenMargin ;

			Account.UpdateTime = order.CancelTime;
		}
		else
		{
			cerr<<"UpdateAccountByOrder Error:"<<endl;
			system("pause");
		}

	}
	
	// ��ӡAccount��log
	PrintAccount();

	return;

}


// ��ȡ��ǰ�����ʽ�
double FAccount::GetAvailableCash()
{
	return Account.Available;
}


// ���ʽ�䶯��������log�ļ�
void FAccount::PrintAccount()
{
	// ��ȡ��ǰϵͳʱ�䣬��ȷ������
		string strNewTime=GetNowMillisecond(); 

		//ofstream log_md("ATS_vecOrders_log.csv",ios::app);
		ofstream log_md("AATS_Account_log.csv",ios::app);

		log_md<<strNewTime;
					
		///�����
		log_md<<","<<Account.Deposit;   // ��ʼ�ʽ�

		///��ǰ���
		log_md<<","<<Account.NowCash;

		///����ı�֤��
		log_md<<","<<Account.FrozenMargin;
		///�����������
		log_md<<","<<Account.FrozenCommission;
		
		///��ǰ��֤���ܶ�
		log_md<<","<<Account.CurrMargin;
	
		///������
		log_md<<","<<Account.Commission;

		///ƽ��ӯ��
		log_md<<","<<Account.CloseProfit;
		///�ֲ�ӯ��
		log_md<<","<<Account.PositionProfit;
		
		///�����ʽ�
		log_md<<","<<Account.Available;  
	
		///�䶯ʱ��
		log_md<<","<<Account.UpdateTime;
								
		log_md<<endl;


		//log_md<<"---,Account,end,--- "<<endl;
		log_md.close();

}

// ��ȡϵͳ��ǰʱ��ĺ�����
string FAccount::GetNowMillisecond(void)
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
