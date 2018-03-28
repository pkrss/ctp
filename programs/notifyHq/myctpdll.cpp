// #include "../build.cc"
#include "myctpdll.h"
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

#include "../../fileSave.h"
#include "../../recordsMem.h"
#include "ctpSave.h"

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
HqMdUser* apiMdUser = 0;
HqTraderUser* apiTraderUser = 0;

int my_ctp_init(){

// #if defined(_WIN32) || defined(_WINDOWS)
//   //! Windows netword DLL init
//   WORD version = MAKEWORD(2, 2);
//   WSADATA data;

//   if (WSAStartup(version, &data) != 0) {
//     std::cerr << "WSAStartup() failure" << std::endl;
//     return -1;
//   }
// #endif /* _WIN32 */

	Profile::getInstance()->init("conf/config.json");
	
	ctpSaveObj.reset(new ctpSave());
	pCtpSaveObj = ctpSaveObj.get();
	pCtpSaveObj->SetSaveDataFun([](const char* name, long id, const char* content) {
		if(!name || !*name || !(*content))
			return;

		// PgSql::getInstance()->saveData(name, id, content);
		std::string path = Profile::getInstance()->getStringCache("data.save", "../data/save");
		char s[32];
		ltoa(id, s, 10);
		path += std::string() + "/" + name;

		mkdir_r(path.c_str());

		path += std::string() + "/" + s + ".json";

		myFileWrite(path.c_str(), content);
	});

	pCtpSaveObj->SetReadDataFun([](const char *name, long id) -> const char* {
		if(!name || !*name)
			return 0;

		// PgSql::getInstance()->saveData(name, id, content);
		std::string path = Profile::getInstance()->getStringCache("data.save", "../data/save");
		char s[32];
		ltoa(id, s, 10);
		path += std::string() + "/" + name;

		mkdir_r(path.c_str());

		path += std::string() + "/" + s + ".json";

		return myFileRead(path.c_str());
	});

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

		auto ppInstrumentField = pCtpSaveObj->readInstruments();
		if(ppInstrumentField.get())
			RecordsMem<CThostFtdcInstrumentField>::getInstance()->resetAll(ppInstrumentField->begin(), ppInstrumentField->end());

		apiTraderUser = hqTraderInit(traderApi);

		HqTraderHandler* traderHandler = apiTraderUser->getResponse();
		// traderHandler->setSaveDataExchangeCallback([](CThostFtdcExchangeField** p, int n) {
			// pCtpSaveObj->saveExchanges(p, n);
		// });

		traderHandler->setSaveDataInstrumentCallback([](const std::list<CThostFtdcInstrumentField>& p) {
			RecordsMem<CThostFtdcInstrumentField>::getInstance()->resetAll(p.begin(),p.end());
			pCtpSaveObj->saveInstruments(p);
		});

		// traderHandler->setSaveDataInstrumentStatusCallback([](CThostFtdcInstrumentStatusField** p, int n) {
			// pCtpSaveObj->saveInstrumentsStatus(p, n);
		// });		

	} while (false);

	return 0;
}

int my_ctp_loop(){
	if(apiMdUser)
		hqMdWait(apiMdUser);
	return 0;
}

int my_ctp_uninit(){


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


NotifyStringCallback realtimeQuoteNotifyCallback = 0;
void onDllRecvDepthMarketDataField(const char *pDepthMarketData) {	
	
	if (!pDepthMarketData || !(*pDepthMarketData) || !realtimeQuoteNotifyCallback)
		return;

	realtimeQuoteNotifyCallback(pDepthMarketData);
}

void setRealtimeQuoteNotifyCallback(NotifyStringCallback cb){
	realtimeQuoteNotifyCallback = cb;
	apiMdUser->setCallback_RtnDepthMarketData((void*)onDllRecvDepthMarketDataField);
}

char* ExtProfileGetString(char* key) {
	return strdup(Profile::getInstance()->getStringCache(key));
}

char* ExtGbkToUtf8(char* gbkStr) {
	return (char*)gbk_to_utf8(gbkStr);
}