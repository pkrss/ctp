#include "hqTrader.h"
#include "hqTraderUserHandler.h"
#include "../../profile.h"
#include <stdio.h>
#include <string.h>

HqTraderUser* hqTraderInit(CThostFtdcTraderApi *cTraderapi)
{
    const char *pszFrontAddress = Profile::getInstance()->getStringCache("trader.frontAddress");

    HqTraderHandler *spiResponse = new HqTraderHandler(cTraderapi);

    HqTraderUser* ret = new HqTraderUser(cTraderapi, spiResponse);
    spiResponse->setApi(ret);

    cTraderapi->RegisterSpi(spiResponse);

	printf("Start connect Trader Server:%s\n", pszFrontAddress);

    cTraderapi->RegisterFront((char*)pszFrontAddress);

	cTraderapi->SubscribePublicTopic(THOST_TERT_RESTART);    // 订阅公共流
	cTraderapi->SubscribePrivateTopic(THOST_TERT_RESTART);   // 订阅私有流

    cTraderapi->Init();

    return ret;
}

// void hqTraderRegQuoteStk(void *p, const char* stks){
//     CtpApiTraderUser* api = (CtpApiTraderUser*)p;
//     api->RegQuoteStk(stks);
// }

int hqTraderWait(HqTraderUser* api) {
	return api->waitExit();
}