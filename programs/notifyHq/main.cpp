// #include "../build.cc"
#include "../../ext.h"
#include "hqMd.h"
#include "hqMdUser.h"
#include "hqTrader.h"
#include "hqTraderUser.h"
#include "../../profile.h"
#include "../../character.h"
#include "../../dll.h"
#include "../../utils.h"
#include <iostream>
#include <string.h> // strdup

#include "hqRedis.h"

#if defined(_MSC_VER)
#define My_DLLEXP __declspec(dllimport)
#else
#define My_DLLEXP 
#endif

typedef My_DLLEXP  CThostFtdcTraderApi*(*FunCreateFtdcTraderApi)(const char *pszFlowPath);
typedef My_DLLEXP  CThostFtdcMdApi*(*FunCreateFtdcMdApi)(const char *pszFlowPath, const bool bIsUsingUdp, const bool bIsMulticast);

int main() {

// #if defined(_WIN32) || defined(_WINDOWS)
//   //! Windows netword DLL init
//   WORD version = MAKEWORD(2, 2);
//   WSADATA data;

//   if (WSAStartup(version, &data) != 0) {
//     std::cerr << "WSAStartup() failure" << std::endl;
//     return -1;
//   }
// #endif /* _WIN32 */

	Profile::getInstance()->init("go/conf/config.json");
	
	void* soMd = 0;
	void* soTrader = 0;

	HqMdUser* apiMdUser = 0;
	HqTraderUser* apiTraderUser = 0;

	HqRedis *hqRedis = 0;

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

		apiMdUser = hqMdInit(mdApi);

		const char *redisIp = Profile::getInstance()->getStringCache("MY_REDIS_IP", "127.0.0.1");
		const char *redisPort = Profile::getInstance()->getStringCache("MY_REDIS_PORT", "6379");
		const char *redisPassword = Profile::getInstance()->getStringCache("MY_REDIS_PASSWORD", "");
		const char *redisDbNum = Profile::getInstance()->getStringCache("MY_REDIS_DBNUM", "0");

		hqRedis = new HqRedis(apiMdUser);

		hqRedis->start(redisIp, atoi(redisPort), redisPassword, atoi(redisDbNum));


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

		
		apiTraderUser = hqTraderInit(traderApi);

		HqTraderHandler* traderHandler = apiTraderUser->getResponse();
		traderHandler->setSaveDataExchangeCallback([](CThostFtdcExchangeField** p, int n) {
			// pCtpSaveObj->saveExchanges(p, n);
		});

		traderHandler->setSaveDataInstrumentCallback([](CThostFtdcInstrumentField** p, int n) {
			// pCtpSaveObj->saveInstruments(p, n);
		});

		traderHandler->setSaveDataInstrumentStatusCallback([](CThostFtdcInstrumentStatusField** p, int n) {
			// pCtpSaveObj->saveInstrumentsStatus(p, n);
		});		

	} while (false);

	if(apiMdUser)
		hqMdWait(apiMdUser);

	if (hqRedis) {
		hqRedis->stop();
		delete hqRedis;
		hqRedis = 0;
	}

	if (apiTraderUser) {
		hqTraderWait(apiTraderUser);
		delete apiTraderUser;
		apiTraderUser = 0;
	}

	if (soTrader) {
		so_free(soTrader);
		soTrader = 0;
	}

	if (apiMdUser) {
		hqMdWait(apiMdUser);
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