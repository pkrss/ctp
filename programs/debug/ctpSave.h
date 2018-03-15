#ifndef __MY_CTP_SAVE__H__
#define __MY_CTP_SAVE__H__

#include "../../third/ctp/ThostFtdcUserApiStruct.h"

class ctpSave
{
public:
	typedef void (*SaveDataFun)(const char* name, long id, const char* content);
	typedef const char* (*ReadDataFun)(const char* name, long id);

	void SetSaveDataFun(SaveDataFun saveDataFun);
	void SetReadDataFun(ReadDataFun readDataFun);

	void saveExchanges(CThostFtdcExchangeField** exchanges, int count);
	CThostFtdcInstrumentField** readInstruments(int* outCount);
	void saveInstruments(CThostFtdcInstrumentField** instaruments, int count);
	void saveInstrumentsStatus(CThostFtdcInstrumentStatusField** instarumentsStatus, int count);
private:
	SaveDataFun saveDataFun;
	ReadDataFun readDataFun;
};

#endif

