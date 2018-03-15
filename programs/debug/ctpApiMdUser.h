#if !defined(__CTPAPI_MDUSER_H__)
#define __CTPAPI_MDUSER_H__

class CThostFtdcMdApi;
class CMdHandler;


class CtpApiMdUser
{
  private:
    CThostFtdcMdApi *mdapi;
    CMdHandler* response;
    
    bool flagOnRspUserLogined;
    char *rememberedRegQuoteStks;

	void* pCallback_RtnDepthMarketData;
public:
    CtpApiMdUser(CThostFtdcMdApi *mdapi, CMdHandler* response);

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