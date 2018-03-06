#if !defined(__CTPAPI_MD_H__)
#define __CTPAPI_MD_H__

#include "ext.h"

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif



void* ctpMdInit(void* mdapi);

// typedef void(*Cb_RtnDepthMarketData)(struct CThostFtdcDepthMarketDataField *pDepthMarketData);
void ctpMdSetCallback_RtnDepthMarketData(void *p, void* cb);

void ctpMdRegQuoteStk(void *p, const char* stks);

int ctpMdWait(void* p);


#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif