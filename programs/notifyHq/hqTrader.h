#if !defined(__CTPAPI_TRADER_H__)
#define __CTPAPI_TRADER_H__

#include "../../ext.h"

#include "hqTraderUser.h"
#include "hqTraderUserHandler.h"

HqTraderUser* hqTraderInit(CThostFtdcTraderApi *cTraderapi);
// void hqTraderRegQuoteStk(void *p, const char* stks);
int hqTraderWait(HqTraderUser* api);


#endif