#if !defined(__CTPAPI_MD_H__)
#define __CTPAPI_MD_H__

#include "../../ext.h"
#include "hqMdUser.h"
#include "hqMdUserHandler.h"

HqMdUser* hqMdInit(CThostFtdcMdApi* mdapi);

// typedef void(*Cb_RtnDepthMarketData)(struct CThostFtdcDepthMarketDataField *pDepthMarketData);
// void hqMdSetCallback_RtnDepthMarketData(void *p, void* cb);

int hqMdWait(HqMdUser* p);

#endif