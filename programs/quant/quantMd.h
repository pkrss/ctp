#if !defined(__CTPAPI_MD_H__)
#define __CTPAPI_MD_H__

#include "../../ext.h"
#include "quantMdUser.h"
#include "quantMdUserHandler.h"

QuantMdUser* QuantMdInit(CThostFtdcMdApi* mdapi);

// typedef void(*Cb_RtnDepthMarketData)(struct CThostFtdcDepthMarketDataField *pDepthMarketData);
// void QuantMdSetCallback_RtnDepthMarketData(void *p, void* cb);

int QuantMdWait(QuantMdUser* p);

#endif