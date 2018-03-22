#if !defined(__CTPAPI_MDUSER_H__)
#define __CTPAPI_MDUSER_H__

class CThostFtdcMdApi;
class QuantMdHandler;


class QuantMdUser
{
  private:
    CThostFtdcMdApi *mdapi;
    QuantMdHandler* response;
    
    bool flagOnRspUserLogined;
    char *rememberedRegQuoteStks;

	void* pCallback_RtnDepthMarketData;
public:
    QuantMdUser(CThostFtdcMdApi *mdapi, QuantMdHandler* response);

    void RegQuoteStk(const char* stks);

    void OnRspUserLogin();

    void OnRspUserLogout();

	int waitExit();

	void* getCallback_RtnDepthMarketData();
	void setCallback_RtnDepthMarketData(void *pDepthMarketData);

private:
    void handleRegQuoteStk(const char* stkStrings);
};

#endif