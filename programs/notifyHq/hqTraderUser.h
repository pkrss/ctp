#if !defined(__Hq_TRADERUSER_H__)
#define __Hq_TRADERUSER_H__

#include "hqTraderUserHandler.h"

class CThostFtdcTraderApi;
class HqTraderHandler;

class HqTraderUser
{
  private:
    CThostFtdcTraderApi *traderApi;
    HqTraderHandler* response;
    
    bool flagOnRspUserLogined;
    char *rememberedRegQuoteStks;
public:
    HqTraderUser(CThostFtdcTraderApi *traderApi, HqTraderHandler* response);

    void RegQuoteStk(const char* stks);

	int waitExit();

	CThostFtdcTraderApi * getTraderApi();

	HqTraderHandler* getResponse();

    void OnRspUserLogin();

    void OnRspUserLogout();

private:
    void handleRegQuoteStk(const char* stkStrings);
};

#endif