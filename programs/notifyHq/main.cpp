// #include "../build.cc"
#include "../../impl/ext.h"
#include "../../impl/ctpApiMd.h"
#include "../../impl/ctpApiMdUser.h"
#include "../../impl/ctpApiTrader.h"
#include "../../impl/ctpApiTraderUser.h"
#include "../../profile.h"
#include "../../pgsql.h"
#include "../../character.h"
#include "../../dll.h"
#include "../../impl/utils.h"
#include <iostream>
#include <string.h> // strdup

#if defined(_MSC_VER)
#define My_DLLEXP __declspec(dllimport)
#else
#define My_DLLEXP 
#endif

typedef My_DLLEXP  CThostFtdcMdApi*(*FunCreateFtdcMdApi)(const char *pszFlowPath, const bool bIsUsingUdp, const bool bIsMulticast);

ctpSave* pCtpSaveObj = 0;

void onRcvRtnDepthMarketData(struct CThostFtdcDepthMarketDataField* pDepthMarketData) {
	// 打印行情，字段较多，截取部分
	std::cout << "=====DepthMarketData=====" << std::endl;
	std::cout << "TradingDay: " << pDepthMarketData->TradingDay << std::endl;
	std::cout << "ExchangeID: " << pDepthMarketData->ExchangeID << std::endl;
	std::cout << "InstrumentID: " << pDepthMarketData->InstrumentID << std::endl;
	std::cout << "ExchangeInstID: " << pDepthMarketData->ExchangeInstID << std::endl;
	std::cout << "LastPrice: " << pDepthMarketData->LastPrice << std::endl;
	std::cout << "Volume: " << pDepthMarketData->Volume << std::endl;
	std::cout << "UpdateTime: " << pDepthMarketData->UpdateTime << std::endl;
}

int main() {

	Profile::getInstance()->init("go/conf/config.json");
	
	void* soMd = 0;

	CtpApiMdUser* apiMdUser = 0;

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

	} while (false);

	if (apiMdUser) {
		ctpMdWait(apiMdUser);
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

char* ExtProfileGetString(char* key) {
	return strdup(Profile::getInstance()->getStringCache(key));
}

char* ExtGbkToUtf8(char* gbkStr) {
	return (char*)gbk_to_utf8(gbkStr);
}