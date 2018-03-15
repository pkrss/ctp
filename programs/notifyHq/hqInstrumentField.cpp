#include "hqTrader.h"
#include "hqTraderUserHandler.h"
#include "../../utils.h"
#include <stdio.h>
#include <string.h>

HqTraderUser* hqTraderInit(CThostFtdcTraderApi *cTraderapi)
{
    char *pszFrontAddress = profileGetString("trader.frontAddress");

    HqTraderHandler *spiResponse = new HqTraderHandler(cTraderapi);

    HqTraderUser* ret = new HqTraderUser(cTraderapi, spiResponse);
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

// void hqTraderRegQuoteStk(void *p, const char* stks){
//     CtpApiTraderUser* api = (CtpApiTraderUser*)p;
//     api->RegQuoteStk(stks);
// }

int hqTraderWait(HqTraderUser* api) {
	return api->waitExit();
}