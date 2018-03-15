#include "hqMd.h"
#include "../../utils.h"
#include <stdio.h>
#include <string.h>


HqMdUser* hqMdInit(CThostFtdcMdApi *cMdapi)
{
    // const char* pszFlowPath = ProfileGetString("data.path");
    // CThostFtdcMdApi *cMdapi = CThostFtdcMdApi::CreateFtdcMdApi(pszFlowPath);

    char *pszFrontAddress = profileGetString("hq.frontAddress");

    HqMdHandler *spiResponse = new HqMdHandler(cMdapi);

    HqMdUser* ret = new HqMdUser(cMdapi, spiResponse);
    spiResponse->setApi(ret);

    cMdapi->RegisterSpi(spiResponse);

    cMdapi->RegisterFront(pszFrontAddress);

    cMdapi->Init();

    profileFreeString(pszFrontAddress);

    return ret;
}

// void hqMdSetCallback_RtnDepthMarketData(void *p, void* cb) {
// 	((CtpApiMdUser*)p)->setCallback_RtnDepthMarketData(cb);
// }

// void hqMdRegQuoteStk(void *p, const char* stks){
//     CtpApiMdUser* api = (CtpApiMdUser*)p;
//     api->RegQuoteStk(stks);
// }

int hqMdWait(HqMdUser* p) {
	return p->waitExit();
}