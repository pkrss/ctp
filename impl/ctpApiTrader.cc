#include "ctpApiTraderUserHandler.h"
#include "ctpApiTraderUser.h"
#include "ctpApiTrader.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>


void* ctpTraderInit(void *api)
{
    char *pszFrontAddress = profileGetString("trader.frontAddress");

    CThostFtdcTraderApi *cTraderapi = (CThostFtdcTraderApi *)api;

    CTraderHandler *spiResponse = new CTraderHandler(cTraderapi);

    CtpApiTraderUser* ret = new CtpApiTraderUser(cTraderapi, spiResponse);
    spiResponse->setApi(ret);

    cTraderapi->RegisterSpi(spiResponse);

	printf("Start connect Trader Server:%s\n", pszFrontAddress);

    cTraderapi->RegisterFront(pszFrontAddress);

	cTraderapi->SubscribePublicTopic(THOST_TERT_RESTART);    // 订阅公共流
	cTraderapi->SubscribePrivateTopic(THOST_TERT_RESTART);   // 订阅私有流

    cTraderapi->Init();

    profileFreeString(pszFrontAddress);

    return ret;
}

void ctpTraderRegQuoteStk(void *p, const char* stks){
    CtpApiTraderUser* api = (CtpApiTraderUser*)p;
    api->RegQuoteStk(stks);
}

int ctpTraderWait(void* p) {
	CtpApiTraderUser* api = (CtpApiTraderUser*)p;
	return api->waitExit();
}