#include "ctpApiMdUserHandler.h"
#include "ctpApiMdUser.h"
#include "ctpApiMd.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>


void* ctpMdInit(void *mdapi)
{
    // const char* pszFlowPath = ProfileGetString("data.path");
    // CThostFtdcMdApi *cMdapi = CThostFtdcMdApi::CreateFtdcMdApi(pszFlowPath);

    char *pszFrontAddress = profileGetString("hq.frontAddress");

    CThostFtdcMdApi *cMdapi = (CThostFtdcMdApi *)mdapi;

    CMdHandler *spiResponse = new CMdHandler(cMdapi);

    CtpApiMdUser* ret = new CtpApiMdUser(cMdapi, spiResponse);
    spiResponse->setApi(ret);

    cMdapi->RegisterSpi(spiResponse);

    cMdapi->RegisterFront(pszFrontAddress);

    cMdapi->Init();

    profileFreeString(pszFrontAddress);

    return ret;
}

void ctpMdSetCallback_RtnDepthMarketData(void *p, void* cb) {
	((CtpApiMdUser*)p)->setCallback_RtnDepthMarketData(cb);
}

void ctpMdRegQuoteStk(void *p, const char* stks){
    CtpApiMdUser* api = (CtpApiMdUser*)p;
    api->RegQuoteStk(stks);
}

int ctpMdWait(void* p) {
	CtpApiMdUser* api = (CtpApiMdUser*)p;
	return api->waitExit();
}