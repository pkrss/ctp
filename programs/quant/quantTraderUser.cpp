#include "quantTraderUserHandler.h"
#include "quantTraderUser.h"
#include "../../utils.h"
#include <stdio.h>
#include <string.h>

QuantTraderUser::QuantTraderUser(CThostFtdcTraderApi *traderApi, QuantTraderHandler* response){
    this->traderApi = traderApi;
    this->response = response;

    flagOnRspUserLogined = false;
    rememberedRegQuoteStks = nullptr;
}

// 登录结果响应
void QuantTraderUser::OnRspUserLogin()
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
void QuantTraderUser::OnRspUserLogout()
{
    flagOnRspUserLogined = false;
}

void QuantTraderUser::RegQuoteStk(const char *stks)
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

CThostFtdcTraderApi * QuantTraderUser::getTraderApi() {
	return traderApi;
}

QuantTraderHandler* QuantTraderUser::getResponse() {
	return response;
}

int QuantTraderUser::waitExit() {
	if (!traderApi)
		return -9;
	int ret = traderApi->Join();

	traderApi->Release();

	return ret;
}

void QuantTraderUser::handleRegQuoteStk(const char *stkStrings)
{

    if ((stkStrings == 0) || (stkStrings[0] == 0))
    {
        return;
    }

    std::list<std::string> stks = SplitString(stkStrings, ",");
    int l = stks.size();

    if (!traderApi || !l)
    {
        return;
    }

    char **ppInstrumentID = (char **)malloc(l * sizeof(char *));

    int i = 0;
    for (std::list<std::string>::iterator b = stks.begin(), e = stks.end(); b != e; ++b, ++i)
    {
        strcpy(ppInstrumentID[i], (*b).c_str());
    }

	// requestID++;
    // traderApi->ReqQryInstrument(ppInstrumentID, i);
    // printf("CThostFtdcMdApi_SubscribeForQuoteRsp\n");


    free(ppInstrumentID);
}
