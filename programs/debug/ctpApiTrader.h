#if !defined(__CTPAPI_TRADER_H__)
#define __CTPAPI_TRADER_H__

#include "ext.h"

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

void* ctpTraderInit(void* api);
// void ctpTraderRegQuoteStk(void *p, const char* stks);
int ctpTraderWait(void* p);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif