#ifndef __MY_CTP_SAVE__H__
#define __MY_CTP_SAVE__H__

#include "../../third/ctp/ThostFtdcUserApiStruct.h"
#include <list>
#include <vector>
#include <memory>

class ctpSave
{
public:
	typedef void (*SaveDataFun)(const char* name, long id, const char* content);
	typedef const char* (*ReadDataFun)(const char* name, long id);

	void SetSaveDataFun(SaveDataFun saveDataFun);
	void SetReadDataFun(ReadDataFun readDataFun);

	void saveExchanges(CThostFtdcExchangeField** exchanges, int count);
	std::shared_ptr<std::vector<CThostFtdcInstrumentField>> readInstruments();
	void saveInstruments(const std::list<CThostFtdcInstrumentField>& instaruments);
	void saveInstrumentsStatus(const std::list<CThostFtdcInstrumentStatusField>& instaruments);
private:
	SaveDataFun saveDataFun;
	ReadDataFun readDataFun;
};

#endif

