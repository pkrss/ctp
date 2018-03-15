#include "ctpApiMdUserHandler.h"
#include "ctpApiMdUser.h"
#include "../../impl/ctpApiMd.h"
#include "../../impl/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

CMdHandler::CMdHandler(CThostFtdcMdApi *mdapi)
{
    requestID = 0;
    this->mdapi = mdapi;
}

void CMdHandler::setApi(CtpApiMdUser* api){
    this->api = api;
}

// 允许登录事件
void CMdHandler::OnFrontConnected()
{
    printf("OnFrontConnected()...\n");

    CThostFtdcReqUserLoginField reqUserLoginField;
    memset(&reqUserLoginField, 0, sizeof(reqUserLoginField));

    char *BrokerID = profileGetString("user.brokerID");
    strcpy(reqUserLoginField.BrokerID, BrokerID);

    char *UserID = profileGetString("user.userID");
    strcpy(reqUserLoginField.UserID, UserID);

    char *Password = profileGetString("user.password");
    strcpy(reqUserLoginField.Password, Password);

    ++requestID;
    mdapi->ReqUserLogin(&reqUserLoginField, requestID);
    printf("CThostFtdcMdApi_ReqUserLogin BrokerID=%s, UserID=%s\n", BrokerID, UserID);

    free(BrokerID);
    free(UserID);
    free(Password);
}

///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
///@param nReason 错误原因
///        0x1001 网络读失败
///        0x1002 网络写失败
///        0x2001 接收心跳超时
///        0x2002 发送心跳失败
///        0x2003 收到错误报文
void CMdHandler::OnFrontDisconnected(int nReason)
{
    printf("OnFrontDisconnected %d\n", nReason);
}

///心跳超时警告。当长时间未收到报文时，该方法被调用。
///@param nTimeLapse 距离上次接收报文的时间
void CMdHandler::OnHeartBeatWarning(int nTimeLapse)
{
    printf("OnHeartBeatWarning %d\n", nTimeLapse);
}

// 登录结果响应
void CMdHandler::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
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
void CMdHandler::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout,
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
void CMdHandler::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    char *msg = ExtGbkToUtf8(pRspInfo->ErrorMsg);
    printf("OnRspError nRequestID=%d bIsLast=%d errorId=%d errorMsg=%s\n", nRequestID, (int)bIsLast, pRspInfo->ErrorID, msg);
    free(msg);
}

// 行情详情通知
void CMdHandler::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	if (!this->api)
		return;
	void* cb = this->api->getCallback_RtnDepthMarketData();
	if (!cb)
		return;
	((void(*)(struct CThostFtdcDepthMarketDataField*))cb)(pDepthMarketData);
}