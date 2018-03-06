#include "ctpApiMdUserHandler.h"
#include "ctpApiMdUser.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>

CtpApiMdUser::CtpApiMdUser(CThostFtdcMdApi *mdapi, CMdHandler* response){
    this->mdapi = mdapi;
    this->response = response;

    flagOnRspUserLogined = false;
    rememberedRegQuoteStks = nullptr;
	pCallback_RtnDepthMarketData = nullptr;
}

// 登录结果响应
void CtpApiMdUser::OnRspUserLogin()
{
    flagOnRspUserLogined = true;
    if (rememberedRegQuoteStks)
    {
        char* tmp = rememberedRegQuoteStks;
        rememberedRegQuoteStks = nullptr;

        handleRegQuoteStk(tmp);
        free(tmp);
    }
}

// 登出结果响应
void CtpApiMdUser::OnRspUserLogout()
{
    flagOnRspUserLogined = false;
}

void CtpApiMdUser::RegQuoteStk(const char *stks)
{

    if ((stks == 0) || (stks[0] == 0))
    {
        return;
    }

    if (flagOnRspUserLogined == false)
    {
        rememberedRegQuoteStks = strdup(stks);
        return;
    }

    handleRegQuoteStk(stks);
}

void CtpApiMdUser::handleRegQuoteStk(const char *stkStrings)
{

    if ((stkStrings == 0) || (stkStrings[0] == 0))
    {
        return;
    }

    std::list<std::string> stks = SplitString(stkStrings, ",");
    int l = stks.size();

    if (!mdapi || !l)
    {
        return;
    }

    char **ppInstrumentID = (char **)malloc(l * sizeof(char *));

    int i = 0;
    for (std::list<std::string>::iterator b = stks.begin(), e = stks.end(); b != e; ++b, ++i)
    {
		ppInstrumentID[i] = strdup((*b).c_str()); // strlwr(
    }

    // mdapi->SubscribeForQuoteRsp(ppInstrumentID, l);
    // printf("CThostFtdcMdApi_SubscribeForQuoteRsp\n");

	// 订阅行情
    mdapi->SubscribeMarketData(ppInstrumentID, l);
    printf("CThostFtdcMdApi_SubscribeMarketData\n");

	for (i = 0; i < l; i++)
		free(ppInstrumentID[i]);
    free(ppInstrumentID);

}

int CtpApiMdUser::waitExit() {
	if (!mdapi)
		return -9;
	int ret = mdapi->Join();

	mdapi->Release();

	return ret;
}

void* CtpApiMdUser::getCallback_RtnDepthMarketData() {
	return this->pCallback_RtnDepthMarketData;
}
void CtpApiMdUser::setCallback_RtnDepthMarketData(void *cb) {
	this->pCallback_RtnDepthMarketData = cb;
}