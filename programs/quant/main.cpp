// #include "../build.cc"
#include "dll.h"
#include "../../ext.h"
#include "quantMd.h"
#include "quantMdUser.h"
#include "quantTrader.h"
#include "quantTraderUser.h"
#include "../../profile.h"
#include "../../character.h"
#include "../../dll.h"
#include "../../utils.h"
#include <iostream>
#include <string.h> // strdup

#include "fileSave.h"
#include "../../recordsMem.h"
#include "../../ctpSave.h"

#if defined(_MSC_VER)
#define My_DLLEXP __declspec(dllimport)
#else
#define My_DLLEXP 
#endif

typedef My_DLLEXP  CThostFtdcTraderApi*(*FunCreateFtdcTraderApi)(const char *pszFlowPath);
typedef My_DLLEXP  CThostFtdcMdApi*(*FunCreateFtdcMdApi)(const char *pszFlowPath, const bool bIsUsingUdp, const bool bIsMulticast);

ctpSave* pCtpSaveObj = 0;
void* soMd = 0;
void* soTrader = 0;
std::unique_ptr<ctpSave> ctpSaveObj;
QuantMdUser* apiMdUser = 0;
QuantTraderUser* apiTraderUser = 0;

int main(){

	Profile::getInstance()->init("go/conf/config.json");
	



	do {
		soMd = so_open("thostmduserapi");

		if (!soMd)
			break;

		const char*funcName = "CThostFtdcMdApi::CreateFtdcMdApi";

#ifdef _WINDOWS
		funcName = "?CreateFtdcMdApi@CThostFtdcMdApi@@SAPEAV1@PEBD_N1@Z";
#else
		funcName = "_ZN15CThostFtdcMdApi15CreateFtdcMdApiEPKcbb";
#endif

		void* createApi = so_find(soMd, funcName);
		if (!createApi) {
			printf("FindProc %s not exist\n", funcName);
			break;
		}

		FunCreateFtdcMdApi funCreateApi = (FunCreateFtdcMdApi)createApi;

		CThostFtdcMdApi * mdApi = 0;

		const char* pszFlowPath = Profile::getInstance()->getStringCache("data.path");

		mkdir_r(pszFlowPath);

		// pszFlowPath = "";
		mdApi = (*funCreateApi)(pszFlowPath, 0, 0);

		if (!mdApi) {
			printf("CreateFtdcMdApi return null\n");
			break;
		}

		apiMdUser = QuantMdInit(mdApi);
	} while (false);
	

	do {
		soTrader = so_open("thosttraderapi");

		if (!soTrader)
			break;

		const char*funcName = "CThostFtdcTraderApi::CreateFtdcTraderApi";

#ifdef _WINDOWS
		funcName = "?CreateFtdcTraderApi@CThostFtdcTraderApi@@SAPEAV1@PEBD@Z";
#else
		funcName = "_ZN19CThostFtdcTraderApi19CreateFtdcTraderApiEPKc";
#endif
		void* createApi = so_find(soTrader, funcName);
		if (!createApi) {
			printf("FindProc %s not exist\n", funcName);
			break;
		}

		FunCreateFtdcTraderApi funCreateApi = (FunCreateFtdcTraderApi)createApi;

		CThostFtdcTraderApi * traderApi = 0;

		const char* pszFlowPath = Profile::getInstance()->getStringCache("data.path");

		mkdir_r(pszFlowPath);

		// pszFlowPath = "";
		traderApi = (*funCreateApi)(pszFlowPath);

		if (!traderApi) {
			printf("CreateFtdcTraderApi return null\n");
			break;
		}


		int instrumentsCount = 0;
		CThostFtdcInstrumentField** ppInstrumentField = pCtpSaveObj->readInstruments(&instrumentsCount);
		RecordsMem<CThostFtdcInstrumentField>::getInstance()->resetAll(ppInstrumentField, instrumentsCount);

		apiTraderUser = QuantTraderInit(traderApi);

		QuantTraderHandler* traderHandler = apiTraderUser->getResponse();
		// traderHandler->setSaveDataExchangeCallback([](CThostFtdcExchangeField** p, int n) {
			// pCtpSaveObj->saveExchanges(p, n);
		// });

		traderHandler->setSaveDataInstrumentCallback(const std::list<CThostFtdcInstrumentField>& p) {
			RecordsMem<CThostFtdcInstrumentField>::getInstance()->resetAll(p);
			pCtpSaveObj->saveInstruments(p);
		});

		// traderHandler->setSaveDataInstrumentStatusCallback([](CThostFtdcInstrumentStatusField** p, int n) {
			// pCtpSaveObj->saveInstrumentsStatus(p, n);
		// });		

	} while (false);
	
	if(apiMdUser)
		QuantMdWait(apiMdUser);	

	if (hqRedis) {
		hqRedis->stop();
		delete hqRedis;
		hqRedis = 0;
	}

	if (apiTraderUser) {
		QuantTraderWait(apiTraderUser);
		delete apiTraderUser;
		apiTraderUser = 0;
	}

	if (soTrader) {
		so_free(soTrader);
		soTrader = 0;
	}

	if (apiMdUser) {
		QuantMdWait(apiMdUser);
		delete apiMdUser;
		apiMdUser = 0;
	}

	if (soMd) {
		so_free(soMd);
		soMd = 0;
	}

	Profile::Destroy();

	return 0;
}


RealtimeQuoteNotifyCallback realtimeQuoteNotifyCallback = 0;
void onRecvDepthMarketDataField(const char *pDepthMarketData) {	
	
	if (!pDepthMarketData || !(*pDepthMarketData) || !realtimeQuoteNotifyCallback)
		return;

	realtimeQuoteNotifyCallback(pDepthMarketData);
}

void setRealtimeQuoteNotifyCallback(NotifyStringCallback cb){
	apiMdUser->setCallback_RtnDepthMarketData((void*)onRecvDepthMarketDataField);
}

char* ExtProfileGetString(char* key) {
	return strdup(Profile::getInstance()->getStringCache(key));
}

char* ExtGbkToUtf8(char* gbkStr) {
	return (char*)gbk_to_utf8(gbkStr);
}