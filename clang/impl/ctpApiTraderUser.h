#if !defined(__CTPAPI_TRADERUSER_H__)
#define __CTPAPI_TRADERUSER_H__

#include "ctpApiTraderUserHandler.h"

class CThostFtdcTraderApi;
class CTraderHandler;

class CtpApiTraderUser
{
  private:
    CThostFtdcTraderApi *traderApi;
    CTraderHandler* response;
    
    bool flagOnRspUserLogined;
    char *rememberedRegQuoteStks;
public:
    CtpApiTraderUser(CThostFtdcTraderApi *traderApi, CTraderHandler* response);

    void RegQuoteStk(const char* stks);

	int waitExit();

	CThostFtdcTraderApi * getTraderApi();

	CTraderHandler* getResponse();

    void OnRspUserLogin();

    void OnRspUserLogout();

private:
    void handleRegQuoteStk(const char* stkStrings);
};

#endif