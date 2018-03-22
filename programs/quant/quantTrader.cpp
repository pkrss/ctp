#include "quantTrader.h"
#include "quantTraderUserHandler.h"
#include "../../utils.h"
#include <stdio.h>
#include <string.h>

QuantTraderUser* QuantTraderInit(CThostFtdcTraderApi *cTraderapi)
{
    char *pszFrontAddress = profileGetString("trader.frontAddress");

    QuantTraderHandler *spiResponse = new QuantTraderHandler(cTraderapi);

    QuantTraderUser* ret = new QuantTraderUser(cTraderapi, spiResponse);
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

// void QuantTraderRegQuoteStk(void *p, const char* stks){
//     CtpApiTraderUser* api = (CtpApiTraderUser*)p;
//     api->RegQuoteStk(stks);
// }

int QuantTraderWait(QuantTraderUser* api) {
	return api->waitExit();
}