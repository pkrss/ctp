#include "quantMd.h"
#include "../../utils.h"
#include <stdio.h>
#include <string.h>


QuantMdUser* QuantMdInit(CThostFtdcMdApi *cMdapi)
{
    // const char* pszFlowPath = ProfileGetString("data.path");
    // CThostFtdcMdApi *cMdapi = CThostFtdcMdApi::CreateFtdcMdApi(pszFlowPath);

    char *pszFrontAddress = profileGetString("quant.frontAddress");

    QuantMdHandler *spiResponse = new QuantMdHandler(cMdapi);

    QuantMdUser* ret = new QuantMdUser(cMdapi, spiResponse);
    spiResponse->setApi(ret);

    cMdapi->RegisterSpi(spiResponse);

    cMdapi->RegisterFront(pszFrontAddress);

    cMdapi->Init();

    profileFreeString(pszFrontAddress);

    return ret;
}

// void QuantMdSetCallback_RtnDepthMarketData(void *p, void* cb) {
// 	((CtpApiMdUser*)p)->setCallback_RtnDepthMarketData(cb);
// }

// void QuantMdRegQuoteStk(void *p, const char* stks){
//     CtpApiMdUser* api = (CtpApiMdUser*)p;
//     api->RegQuoteStk(stks);
// }

int QuantMdWait(QuantMdUser* p) {
	return p->waitExit();
}