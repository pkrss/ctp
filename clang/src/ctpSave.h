#ifndef __MY_CTP_SAVE__H__
#define __MY_CTP_SAVE__H__

#include "../h/ThostFtdcUserApiStruct.h"

class ctpSave
{
public:
	typedef void (*SaveDataFun)(const char* name, long id, const char* content);

	void SetSaveDataFun(SaveDataFun saveDataFun);

	void saveExchanges(CThostFtdcExchangeField** exchanges, int count);
	void saveInstruments(CThostFtdcInstrumentField** instaruments, int count);
	void saveInstrumentsStatus(CThostFtdcInstrumentStatusField** instarumentsStatus, int count);
private:
	SaveDataFun saveDataFun;
};

#endif

