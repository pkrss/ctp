#if !defined(__CTPAPI_TRADER_H__)
#define __CTPAPI_TRADER_H__

#include "../../ext.h"

#include "quantTraderUser.h"
#include "quantTraderUserHandler.h"

QuantTraderUser* QuantTraderInit(CThostFtdcTraderApi *cTraderapi);
// void QuantTraderRegQuoteStk(void *p, const char* stks);
int QuantTraderWait(QuantTraderUser* api);


#endif