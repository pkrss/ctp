// #include "../build.cc"
#include "../impl/ext.h"
#include "../impl/ctpApiMd.h"
#include "../impl/ctpApiMdUser.h"
#include "../impl/ctpApiTrader.h"
#include "../impl/ctpApiTraderUser.h"
#include "profile.h"
#include "pgsql.h"
#include "character.h"
#include "dll.h"
#include "../impl/utils.h"
#include "ctpSave.h"
#include <iostream>
#include <string.h> // strdup

#if defined(_WINDOWS)
#define My_DLLEXP __declspec(dllimport)
#else
#define My_DLLEXP 
#endif

typedef My_DLLEXP  CThostFtdcTraderApi*(*FunCreateFtdcTraderApi)(const char *pszFlowPath);
typedef My_DLLEXP  CThostFtdcMdApi*(*FunCreateFtdcMdApi)(const char *pszFlowPath, const bool bIsUsingUdp, const bool bIsMulticast);

ctpSave* pCtpSaveObj = 0;

void onRcvRtnDepthMarketData(struct CThostFtdcDepthMarketDataField* pDepthMarketData) {
	// 打印行情，字段较多，截取部分
	std::cout << "=====获得深度行情=====" << std::endl;
	std::cout << "交易日： " << pDepthMarketData->TradingDay << std::endl;
	std::cout << "交易所代码： " << pDepthMarketData->ExchangeID << std::endl;
	std::cout << "合约代码： " << pDepthMarketData->InstrumentID << std::endl;
	std::cout << "合约在交易所的代码： " << pDepthMarketData->ExchangeInstID << std::endl;
	std::cout << "最新价： " << pDepthMarketData->LastPrice << std::endl;
	std::cout << "数量： " << pDepthMarketData->Volume << std::endl;
	std::cout << "更新时间： " << pDepthMarketData->UpdateTime << std::endl;
}

int main() {

	setlocale(LC_ALL, "");

	Profile::getInstance()->init("../../conf/config.json");

	PgSql::getInstance()->init();

	void* soMd = 0;
	void* soTrader = 0;

	std::unique_ptr<ctpSave> ctpSaveObj(new ctpSave());
	ctpSaveObj->SetSaveDataFun([](const char* name, long id, const char* content) {
		PgSql::getInstance()->saveData(name, id, content);
	});
	pCtpSaveObj = ctpSaveObj.get();

	CtpApiMdUser* apiMdUser = 0;
	CtpApiTraderUser* apiTraderUser = 0;

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

		apiMdUser = (CtpApiMdUser*)ctpMdInit(mdApi);

		ctpMdSetCallback_RtnDepthMarketData(apiMdUser, (void*)onRcvRtnDepthMarketData);

		ctpMdRegQuoteStk(apiMdUser, "rb1805,IF1806");

		/*CTraderHandler* traderHandler = apiMdUser->getResponse();
		traderHandler->setSaveDataExchangeCallback([](CThostFtdcExchangeField** p, int n) {
			pCtpSaveObj->saveExchanges(p, n);
		});

		traderHandler->setSaveDataInstrumentCallback([](CThostFtdcInstrumentField** p, int n) {
			pCtpSaveObj->saveInstruments(p, n);
		});

		traderHandler->setSaveDataInstrumentStatusCallback([](CThostFtdcInstrumentStatusField** p, int n) {
			pCtpSaveObj->saveInstrumentsStatus(p, n);
		});*/

	} while (false);

	do {
		break;

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

		apiTraderUser = (CtpApiTraderUser*)ctpTraderInit(traderApi);

		CTraderHandler* traderHandler = apiTraderUser->getResponse();
		traderHandler->setSaveDataExchangeCallback([](CThostFtdcExchangeField** p, int n) {
			pCtpSaveObj->saveExchanges(p, n);
		});

		traderHandler->setSaveDataInstrumentCallback([](CThostFtdcInstrumentField** p, int n) {
			pCtpSaveObj->saveInstruments(p, n);
		});

		traderHandler->setSaveDataInstrumentStatusCallback([](CThostFtdcInstrumentStatusField** p, int n) {
			pCtpSaveObj->saveInstrumentsStatus(p, n);
		});		

	} while (false);

	if (apiTraderUser) {
		ctpTraderWait(apiTraderUser);
		delete apiTraderUser;
		apiTraderUser = 0;
	}

	if (soTrader) {
		so_free(soTrader);
		soTrader = 0;
	}

	if (apiMdUser) {
		ctpMdWait(apiMdUser);
		delete apiMdUser;
		apiMdUser = 0;
	}

	if (soMd) {
		so_free(soMd);
		soMd = 0;
	}

	PgSql::Destroy();
	Profile::Destroy();

	return 0;
}

char* ExtProfileGetString(char* key) {
	return strdup(Profile::getInstance()->getStringCache(key));
}

char* ExtGbkToUtf8(char* gbkStr) {
	return (char*)gbk_to_utf8(gbkStr);
}