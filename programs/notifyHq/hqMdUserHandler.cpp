#include "HqMdUserHandler.h"
#include "HqMdUser.h"
#include "hqMd.h"
#include "../../profile.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../third/jsoncpp/json.hpp"

HqMdHandler::HqMdHandler(CThostFtdcMdApi *mdapi)
{
    requestID = 0;
    this->mdapi = mdapi;
}

void HqMdHandler::setApi(HqMdUser* api){
    this->api = api;
}

// 允许登录事件
void HqMdHandler::OnFrontConnected()
{
    printf("OnFrontConnected()...\n");

    CThostFtdcReqUserLoginField reqUserLoginField;
    memset(&reqUserLoginField, 0, sizeof(reqUserLoginField));

    const char *BrokerID = Profile::getInstance()->getStringCache("user.brokerID");
    strcpy(reqUserLoginField.BrokerID, BrokerID);

    const char *UserID = Profile::getInstance()->getStringCache("user.userID");
    strcpy(reqUserLoginField.UserID, UserID);

    const char *Password = Profile::getInstance()->getStringCache("user.password");
    strcpy(reqUserLoginField.Password, Password);

    ++requestID;
    mdapi->ReqUserLogin(&reqUserLoginField, requestID);
    printf("CThostFtdcMdApi_ReqUserLogin BrokerID=%s, UserID=%s\n", BrokerID, UserID);

}

///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
///@param nReason 错误原因
///        0x1001 网络读失败
///        0x1002 网络写失败
///        0x2001 接收心跳超时
///        0x2002 发送心跳失败
///        0x2003 收到错误报文
void HqMdHandler::OnFrontDisconnected(int nReason)
{
    printf("OnFrontDisconnected %d\n", nReason);
}

///心跳超时警告。当长时间未收到报文时，该方法被调用。
///@param nTimeLapse 距离上次接收报文的时间
void HqMdHandler::OnHeartBeatWarning(int nTimeLapse)
{
    printf("OnHeartBeatWarning %d\n", nTimeLapse);
}

// 登录结果响应
void HqMdHandler::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
                                CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    char *msg = ExtGbkToUtf8(pRspInfo->ErrorMsg);
    printf("OnRspUserLogin nRequestID=%d bIsLast=%d errorId=%d errorMsg=%s\n", nRequestID, (int)bIsLast, pRspInfo->ErrorID, msg);
    free(msg);

    if(!pRspInfo->ErrorID){
        if(api){
            api->OnRspUserLogin();
        }
    }
}

// 登出结果响应
void HqMdHandler::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout,
                                 CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    printf("OnRspUserLogout nRequestID:%d bIsLast:%d\n", nRequestID, (int)bIsLast);
    if(!pRspInfo->ErrorID){
        if(api){
            api->OnRspUserLogout();
        }
    }
}

// 错误信息响应方法
void HqMdHandler::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    char *msg = ExtGbkToUtf8(pRspInfo->ErrorMsg);
    printf("OnRspError nRequestID=%d bIsLast=%d errorId=%d errorMsg=%s\n", nRequestID, (int)bIsLast, pRspInfo->ErrorID, msg);
    free(msg);
}

// 行情详情通知
void HqMdHandler::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	if (!this->api)
		return;
	void* cb = this->api->getCallback_RtnDepthMarketData();
	if (!cb)
		return;

    nlohmann::json quote;
	quote["id"] = pDepthMarketData->InstrumentID;
	// quote["name"] = rspRowCols[0]
	quote["open"] = pDepthMarketData->OpenPrice;
	quote["high"] = pDepthMarketData->HighestPrice;
	quote["low"] = pDepthMarketData->LowestPrice;
	quote["prevClose"] = pDepthMarketData->PreClosePrice;
	quote["buy1"] = pDepthMarketData->BidPrice1;
	quote["sell1"] = pDepthMarketData->AskPrice1;
	quote["price"] = pDepthMarketData->LastPrice;
	quote["settlement"] = pDepthMarketData->SettlementPrice;
	quote["prevSettlement"] = pDepthMarketData->PreSettlementPrice;
	quote["buy1vol"] = pDepthMarketData->BidVolume1;
	quote["sell1vol"] = pDepthMarketData->AskVolume1;

	// quote["exSName"] = pDepthMarketData->;
	// quote["sName"] = pDepthMarketData->;

	// quote["time"] = pDepthMarketData->UpdateTime + pDepthMarketData->UpdateMillisec;
	int timeH = 0, timeM = 0, timeS = 0;
	if (3 ==sscanf(pDepthMarketData->UpdateTime,"%02d:%02d:%02d", &timeH, &timeM, &timeS)){
		time_t now = time(0);
		// now += timeH * 60 * 60 + timeM * 60 + timeS;

		tm* tm = gmtime(&now);
		tm->tm_hour = timeH;
		tm->tm_min = timeM;
		tm->tm_sec = timeS;
		now = mktime(tm);

		quote["time"] = now;  //		+pDepthMarketData->UpdateMillisec;
	}

	double prevSettlement = pDepthMarketData->PreSettlementPrice;
	double price = pDepthMarketData->LastPrice;

	if(prevSettlement == 0)
	{
		prevSettlement = pDepthMarketData->OpenPrice;
	}

	quote["updnPrice"] = price - prevSettlement;
	if (prevSettlement > 0)
	{
		quote["updnPricePer"] = ((price-prevSettlement)*100.0f/prevSettlement)+0.0000005;
	}
	else
	{
		// quote["updnPricePer"] = nil
	}
	
	nlohmann::json jsonRoot;
	jsonRoot["cat"] = "quote";
	jsonRoot["oper"] = "realtime";
	jsonRoot["data"] = quote;

	std::string s = jsonRoot.dump();
	((void(*)(const char*))cb)(s.c_str());
}