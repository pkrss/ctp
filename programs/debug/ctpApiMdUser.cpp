#include "ctpApiMdUserHandler.h"
#include "ctpApiMdUser.h"
#include "../../impl/utils.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <algorithm>

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

    // time_t now;
    // time(&now);
    // struct tm * nowTm = localtime(&now);

    // char curYYMM[32];
    // sprintf(curYYMM, "%02d%02d", nowTm->tm_year % 100, nowTm->tm_mon + 1);

    int i = 0;
    for (std::list<std::string>::iterator b = stks.begin(), e = stks.end(); b != e; ++b)
    {
        // if(((*b).length()==3) && (0==(*b).substr(2,1).compare("0"))){
        //     b->resize(7);
        //     strcpy((char *)(b->c_str() + 2), curYYMM);

        //     if(stks.end() != std::find(stks.begin(),stks.end(), *b))
        //         continue;
        // }
		ppInstrumentID[i] = strdup((*b).c_str()); // strlwr(
        ++i;
    }
    l = i;

    // mdapi->SubscribeForQuoteRsp(ppInstrumentID, l);
    // printf("CThostFtdcMdApi_SubscribeForQuoteRsp\n");

	// 订阅行情
    mdapi->SubscribeMarketData(ppInstrumentID, l);
    printf("CThostFtdcMdApi_SubscribeMarketData,$d\n", l);

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