#if !defined(__Hq_TRADERUSER_H__)
#define __Hq_TRADERUSER_H__

#include "quantTraderUserHandler.h"

class CThostFtdcTraderApi;
class QuantTraderHandler;

class QuantTraderUser
{
  private:
    CThostFtdcTraderApi *traderApi;
    QuantTraderHandler* response;
    
    bool flagOnRspUserLogined;
    char *rememberedRegQuoteStks;
public:
    QuantTraderUser(CThostFtdcTraderApi *traderApi, QuantTraderHandler* response);

    void RegQuoteStk(const char* stks);

	int waitExit();

	CThostFtdcTraderApi * getTraderApi();

	QuantTraderHandler* getResponse();

    void OnRspUserLogin();

    void OnRspUserLogout();

private:
    void handleRegQuoteStk(const char* stkStrings);
};

#endif