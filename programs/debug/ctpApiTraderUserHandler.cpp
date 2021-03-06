#include "ctpApiTraderUserHandler.h"
#include "ctpApiTraderUser.h"
#include "ctpApiTrader.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

CTraderHandler::CTraderHandler(CThostFtdcTraderApi *traderApi)
{
    requestID = 0;
    this->traderApi = traderApi;
    tradingDay = 0;
}

void CTraderHandler::setApi(CtpApiTraderUser* api){
    this->api = api;
}

void CTraderHandler::setSaveDataExchangeCallback(CSaveDataHelper<CThostFtdcExchangeField>::FUNSAVEDATA callback) {
	if(!saveDataExchangeHelper.get())
		saveDataExchangeHelper.reset(new CSaveDataHelper<CThostFtdcExchangeField>());
	saveDataExchangeHelper.get()->setSaveChangeCallback(callback);
}


void CTraderHandler::setSaveDataInstrumentCallback(CSaveDataHelper<CThostFtdcInstrumentField>::FUNSAVEDATA callback) {
	if (!saveDataInstrumentHelper.get())
		saveDataInstrumentHelper.reset(new CSaveDataHelper<CThostFtdcInstrumentField>());
	saveDataInstrumentHelper.get()->setSaveChangeCallback(callback);
}

void CTraderHandler::setSaveDataInstrumentStatusCallback(CSaveDataHelper<CThostFtdcInstrumentStatusField>::FUNSAVEDATA callback) {
	if (!saveDataInstrumentStatusHelper.get())
		saveDataInstrumentStatusHelper.reset(new CSaveDataHelper<CThostFtdcInstrumentStatusField>());
	saveDataInstrumentStatusHelper.get()->setSaveChangeCallback(callback);
}

int CTraderHandler::plusRequestId(){
    return ++requestID;
}

    ///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
void CTraderHandler::OnFrontConnected(){
    printf("OnFrontConnected()...\n");

    CThostFtdcReqUserLoginField reqUserLoginField;
    memset(&reqUserLoginField, 0, sizeof(reqUserLoginField));

    char *BrokerID = profileGetString("user.brokerID");
    strcpy(reqUserLoginField.BrokerID, BrokerID);

    char *UserID = profileGetString("user.userID");
    strcpy(reqUserLoginField.UserID, UserID);

    char *Password = profileGetString("user.password");
    strcpy(reqUserLoginField.Password, Password);
	/*
	strcpy(reqUserLoginField.UserProductInfo, "Q7V3 3456");
	strcpy(reqUserLoginField.InterfaceProductInfo, "THOST User");
	strcpy(reqUserLoginField.ProtocolInfo, "FTDC 0");*/
	

    ++requestID;
    traderApi->ReqUserLogin(&reqUserLoginField, requestID);
    printf("CThostFtdcTraderApi::ReqUserLogin BrokerID=%s, UserID=%s\n", BrokerID, UserID);

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
void CTraderHandler::OnFrontDisconnected(int nReason){
    printf("OnFrontDisconnected %d\n", nReason);
}
		
	///心跳超时警告。当长时间未收到报文时，该方法被调用。
	///@param nTimeLapse 距离上次接收报文的时间
void CTraderHandler::OnHeartBeatWarning(int nTimeLapse){
    printf("OnHeartBeatWarning %d\n", nTimeLapse);
}

    ///客户端认证响应
void CTraderHandler::OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspAuthenticate\n");
}
	

	///登录请求响应
void CTraderHandler::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    char *msg = ExtGbkToUtf8(pRspInfo->ErrorMsg);
    printf("OnRspUserLogin nRequestID=%d bIsLast=%d errorId=%d errorMsg=%s\n", nRequestID, (int)bIsLast, pRspInfo->ErrorID, pRspInfo->ErrorMsg);
    free(msg);

    if(0){
        CThostFtdcQryInstrumentField requestInstrumentField;
        memset(&requestInstrumentField, 0, sizeof(CThostFtdcQryInstrumentField));
        // strcpy(requestInstrumentField.ExchangeID, "CZCE");
        requestID++;
        traderApi->ReqQryInstrument(&requestInstrumentField, requestID);
        printf("CThostFtdcTraderApi::ReqQryInstrument\n");
    }

    // 20180122
    tradingDay = traderApi->GetTradingDay();
    printf("CThostFtdcTraderApi::GetTradingDay()=%s\n", tradingDay);
    

    if(0){
        traderApi->SubscribePrivateTopic(THOST_TERT_RESTART);
        printf("CThostFtdcTraderApi::SubscribePrivateTopic(THOST_TERT_RESTART)\n");
    }

    if(0){
        traderApi->SubscribePublicTopic(THOST_TERT_RESTART);
        printf("CThostFtdcTraderApi::SubscribePublicTopic(THOST_TERT_RESTART)\n");
    }

    if(1){
        CThostFtdcQryExchangeField qryExchangeField;
        memset(&qryExchangeField, 0, sizeof(CThostFtdcQryExchangeField));
        requestID++;
        traderApi->ReqQryExchange(&qryExchangeField, requestID);
        printf("CThostFtdcTraderApi::ReqQryExchange(ExchangeID=%s)\n", qryExchangeField.ExchangeID);
    }    

    if(!pRspInfo->ErrorID){
        if(api){
            api->OnRspUserLogin();
        }
    }
}

	///登出请求响应
void CTraderHandler::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspUserLogout nRequestID:%d bIsLast:%d\n", nRequestID, (int)bIsLast);
    if(!pRspInfo->ErrorID){
        if(api){
            api->OnRspUserLogout();
        }
    }
}

	///用户口令更新请求响应
void CTraderHandler::OnRspUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspUserPasswordUpdate\n");
}

	///资金账户口令更新请求响应
void CTraderHandler::OnRspTradingAccountPasswordUpdate(CThostFtdcTradingAccountPasswordUpdateField *pTradingAccountPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspTradingAccountPasswordUpdate\n");
}

	///报单录入请求响应
void CTraderHandler::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspOrderInsert\n");
}

	///预埋单录入请求响应
void CTraderHandler::OnRspParkedOrderInsert(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspParkedOrderInsert\n");
}

	///预埋撤单录入请求响应
void CTraderHandler::OnRspParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspParkedOrderAction\n");
}

	///报单操作请求响应
void CTraderHandler::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspOrderAction\n");
}

	///查询最大报单数量响应
void CTraderHandler::OnRspQueryMaxOrderVolume(CThostFtdcQueryMaxOrderVolumeField *pQueryMaxOrderVolume, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspQueryMaxOrderVolume\n");
}

	///投资者结算结果确认响应
void CTraderHandler::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspSettlementInfoConfirm\n");
}

	///删除预埋单响应
void CTraderHandler::OnRspRemoveParkedOrder(CThostFtdcRemoveParkedOrderField *pRemoveParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspRemoveParkedOrder\n");
}

	///删除预埋撤单响应
void CTraderHandler::OnRspRemoveParkedOrderAction(CThostFtdcRemoveParkedOrderActionField *pRemoveParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspRemoveParkedOrderAction\n");
}

	///执行宣告录入请求响应
void CTraderHandler::OnRspExecOrderInsert(CThostFtdcInputExecOrderField *pInputExecOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspExecOrderInsert\n");
}

	///执行宣告操作请求响应
void CTraderHandler::OnRspExecOrderAction(CThostFtdcInputExecOrderActionField *pInputExecOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspExecOrderAction\n");
}

	///询价录入请求响应
void CTraderHandler::OnRspForQuoteInsert(CThostFtdcInputForQuoteField *pInputForQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspForQuoteInsert\n");
}

	///报价录入请求响应
void CTraderHandler::OnRspQuoteInsert(CThostFtdcInputQuoteField *pInputQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspQuoteInsert\n");
}

	///报价操作请求响应
void CTraderHandler::OnRspQuoteAction(CThostFtdcInputQuoteActionField *pInputQuoteAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspQuoteAction\n");
}

	///批量报单操作请求响应
void CTraderHandler::OnRspBatchOrderAction(CThostFtdcInputBatchOrderActionField *pInputBatchOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspBatchOrderAction\n");
}

	///期权自对冲录入请求响应
void CTraderHandler::OnRspOptionSelfCloseInsert(CThostFtdcInputOptionSelfCloseField *pInputOptionSelfClose, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspOptionSelfCloseInsert\n");
}

	///期权自对冲操作请求响应
void CTraderHandler::OnRspOptionSelfCloseAction(CThostFtdcInputOptionSelfCloseActionField *pInputOptionSelfCloseAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspOptionSelfCloseAction\n");
}

	///申请组合录入请求响应
void CTraderHandler::OnRspCombActionInsert(CThostFtdcInputCombActionField *pInputCombAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspCombActionInsert\n");
}

	///请求查询报单响应
void CTraderHandler::OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspQryOrder\n");
}

	///请求查询成交响应
void CTraderHandler::OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspQryTrade\n");
}

	///请求查询投资者持仓响应
void CTraderHandler::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspQryInvestorPosition\n");
}

	///请求查询资金账户响应
void CTraderHandler::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspQryTradingAccount\n");
}

	///请求查询投资者响应
void CTraderHandler::OnRspQryInvestor(CThostFtdcInvestorField *pInvestor, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspQryInvestor\n");
}

	///请求查询交易编码响应
void CTraderHandler::OnRspQryTradingCode(CThostFtdcTradingCodeField *pTradingCode, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspQryTradingCode\n");
}

	///请求查询合约保证金率响应
void CTraderHandler::OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspQryInstrumentMarginRate\n");
}

	///请求查询合约手续费率响应
void CTraderHandler::OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspQryInstrumentCommissionRate\n");
}

	///请求查询交易所响应
void CTraderHandler::OnRspQryExchange(CThostFtdcExchangeField *pExchange, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){

	printf("CTraderHandler::OnRspQryExchange ExchangeID=%s, ExchangeName=%s, ExchangeProperty=%d, IsLast=%d\n", pExchange->ExchangeID, pExchange->ExchangeName, (int)pExchange->ExchangeProperty, (int)bIsLast);

	//if (!pRspInfo || pRspInfo->ErrorID) {
	//	return;
	//}

	if (!saveDataExchangeHelper.get()) {
		printf("WARN: CTraderHandler::OnRspQryExchange can not save data, bacause saveDataExchangeHelper is null\n");
		return;
	}

	saveDataExchangeHelper.get()->addItem(pExchange);
	if (bIsLast) {
		saveDataExchangeHelper.get()->saveAll();
	}
}

	///请求查询产品响应
void CTraderHandler::OnRspQryProduct(CThostFtdcProductField *pProduct, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspQryProduct\n");
}

	///请求查询合约响应
void CTraderHandler::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspQryInstrument InstrumentID=%s ExchangeID=%s InstrumentName=%s, IsLast=%d\n", pInstrument->InstrumentID, pInstrument->ExchangeID, pInstrument->InstrumentName, (int)bIsLast);

	//if (!pRspInfo || pRspInfo->ErrorID) {
	//	return;
	//}

	if (!saveDataInstrumentHelper.get()) {
		printf("WARN: CTraderHandler::OnRspQryInstrument can not save data, bacause saveDataInstrumentHelper is null\n");
		return;
	}

	saveDataInstrumentHelper.get()->addItem(pInstrument);
	if (bIsLast) {
		saveDataInstrumentHelper.get()->saveAll();
	}
}

	///请求查询行情响应
void CTraderHandler::OnRspQryDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("CTraderHandler::OnRspQryDepthMarketData InstrumentID=%s, LastPrice=%f, IsLast=%d\n", pDepthMarketData->InstrumentID, pDepthMarketData->LastPrice, (int)bIsLast);
}

	///请求查询投资者结算结果响应
void CTraderHandler::OnRspQrySettlementInfo(CThostFtdcSettlementInfoField *pSettlementInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspQrySettlementInfo\n");
}

	///请求查询转帐银行响应
void CTraderHandler::OnRspQryTransferBank(CThostFtdcTransferBankField *pTransferBank, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspQryTransferBank\n");
}

	///请求查询投资者持仓明细响应
void CTraderHandler::OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspQryInvestorPositionDetail\n");
}

	///请求查询客户通知响应
void CTraderHandler::OnRspQryNotice(CThostFtdcNoticeField *pNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspQryNotice\n");
}

	///请求查询结算信息确认响应
void CTraderHandler::OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspQrySettlementInfoConfirm\n");
}

	///请求查询投资者持仓明细响应
void CTraderHandler::OnRspQryInvestorPositionCombineDetail(CThostFtdcInvestorPositionCombineDetailField *pInvestorPositionCombineDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspQryInvestorPositionCombineDetail\n");
}

	///查询保证金监管系统经纪公司资金账户密钥响应
void CTraderHandler::OnRspQryCFMMCTradingAccountKey(CThostFtdcCFMMCTradingAccountKeyField *pCFMMCTradingAccountKey, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspQryCFMMCTradingAccountKey\n");
}

	///请求查询仓单折抵信息响应
void CTraderHandler::OnRspQryEWarrantOffset(CThostFtdcEWarrantOffsetField *pEWarrantOffset, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspQryEWarrantOffset\n");
}

	///请求查询投资者品种/跨品种保证金响应
void CTraderHandler::OnRspQryInvestorProductGroupMargin(CThostFtdcInvestorProductGroupMarginField *pInvestorProductGroupMargin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspQryInvestorProductGroupMargin\n");
}

	///请求查询交易所保证金率响应
void CTraderHandler::OnRspQryExchangeMarginRate(CThostFtdcExchangeMarginRateField *pExchangeMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspQryExchangeMarginRate\n");
}

	///请求查询交易所调整保证金率响应
void CTraderHandler::OnRspQryExchangeMarginRateAdjust(CThostFtdcExchangeMarginRateAdjustField *pExchangeMarginRateAdjust, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspQryExchangeMarginRateAdjust\n");
}

	///请求查询汇率响应
void CTraderHandler::OnRspQryExchangeRate(CThostFtdcExchangeRateField *pExchangeRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspQryExchangeRate\n");
}

	///请求查询二级代理操作员银期权限响应
void CTraderHandler::OnRspQrySecAgentACIDMap(CThostFtdcSecAgentACIDMapField *pSecAgentACIDMap, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspQrySecAgentACIDMap\n");
}

	///请求查询产品报价汇率
void CTraderHandler::OnRspQryProductExchRate(CThostFtdcProductExchRateField *pProductExchRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspQryProductExchRate\n");
}

	///请求查询产品组
void CTraderHandler::OnRspQryProductGroup(CThostFtdcProductGroupField *pProductGroup, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspQryProductGroup\n");
}

	///请求查询做市商合约手续费率响应
void CTraderHandler::OnRspQryMMInstrumentCommissionRate(CThostFtdcMMInstrumentCommissionRateField *pMMInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspQryMMInstrumentCommissionRate\n");
}

	///请求查询做市商期权合约手续费响应
void CTraderHandler::OnRspQryMMOptionInstrCommRate(CThostFtdcMMOptionInstrCommRateField *pMMOptionInstrCommRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspQryMMOptionInstrCommRate\n");
}

	///请求查询报单手续费响应
void CTraderHandler::OnRspQryInstrumentOrderCommRate(CThostFtdcInstrumentOrderCommRateField *pInstrumentOrderCommRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspQryInstrumentOrderCommRate\n");
}

	///请求查询资金账户响应
void CTraderHandler::OnRspQrySecAgentTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspQrySecAgentTradingAccount\n");
}

	///请求查询二级代理商资金校验模式响应
void CTraderHandler::OnRspQrySecAgentCheckMode(CThostFtdcSecAgentCheckModeField *pSecAgentCheckMode, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspQrySecAgentCheckMode\n");
}

	///请求查询期权交易成本响应
void CTraderHandler::OnRspQryOptionInstrTradeCost(CThostFtdcOptionInstrTradeCostField *pOptionInstrTradeCost, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspQryOptionInstrTradeCost\n");
}

	///请求查询期权合约手续费响应
void CTraderHandler::OnRspQryOptionInstrCommRate(CThostFtdcOptionInstrCommRateField *pOptionInstrCommRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspQryOptionInstrCommRate\n");
}

	///请求查询执行宣告响应
void CTraderHandler::OnRspQryExecOrder(CThostFtdcExecOrderField *pExecOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspQryExecOrder\n");
}

	///请求查询询价响应
void CTraderHandler::OnRspQryForQuote(CThostFtdcForQuoteField *pForQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspQryForQuote\n");
}

	///请求查询报价响应
void CTraderHandler::OnRspQryQuote(CThostFtdcQuoteField *pQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspQryQuote\n");
}

	///请求查询期权自对冲响应
void CTraderHandler::OnRspQryOptionSelfClose(CThostFtdcOptionSelfCloseField *pOptionSelfClose, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspQryOptionSelfClose\n");
}

	///请求查询投资单元响应
void CTraderHandler::OnRspQryInvestUnit(CThostFtdcInvestUnitField *pInvestUnit, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspQryInvestUnit\n");
}

	///请求查询组合合约安全系数响应
void CTraderHandler::OnRspQryCombInstrumentGuard(CThostFtdcCombInstrumentGuardField *pCombInstrumentGuard, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspQryCombInstrumentGuard\n");
}

	///请求查询申请组合响应
void CTraderHandler::OnRspQryCombAction(CThostFtdcCombActionField *pCombAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspQryCombAction\n");
}

	///请求查询转帐流水响应
void CTraderHandler::OnRspQryTransferSerial(CThostFtdcTransferSerialField *pTransferSerial, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspQryTransferSerial\n");
}

	///请求查询银期签约关系响应
void CTraderHandler::OnRspQryAccountregister(CThostFtdcAccountregisterField *pAccountregister, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspQryAccountregister\n");
}

	///错误应答
void CTraderHandler::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    char *msg = ExtGbkToUtf8(pRspInfo->ErrorMsg);
    printf("OnRspError nRequestID=%d bIsLast=%d errorId=%d errorMsg=%s\n", nRequestID, (int)bIsLast, pRspInfo->ErrorID, msg);
    free(msg);
}

	///报单通知
void CTraderHandler::OnRtnOrder(CThostFtdcOrderField *pOrder){
    printf("OnRtnOrder\n");
}

	///成交通知
void CTraderHandler::OnRtnTrade(CThostFtdcTradeField *pTrade){
    printf("OnRtnTrade\n");
}

	///报单录入错误回报
void CTraderHandler::OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo){
    printf("OnErrRtnOrderInsert\n");
}

	///报单操作错误回报
void CTraderHandler::OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo){
    printf("OnErrRtnOrderAction\n");
}

	///合约交易状态通知
void CTraderHandler::OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus){

	// OnRtnInstrumentStatus ExchangeID = SHFE InstrumentID = au
	// OnRtnInstrumentStatus ExchangeID = SHFE InstrumentID = hc
    printf("OnRtnInstrumentStatus ExchangeID=%s InstrumentID=%s\n", pInstrumentStatus->ExchangeID, pInstrumentStatus->InstrumentID);

	/*if (!saveDataInstrumentStatusHelper.get()) {
		printf("WARN: CTraderHandler::OnRtnInstrumentStatus can not save data, bacause saveDataInstrumentStatusHelper is null\n");
		return;
	}

	saveDataInstrumentStatusHelper.get()->addItem(pInstrumentStatus);
	if (bIsLast) {
		saveDataInstrumentStatusHelper.get()->saveAll();
	}*/

}

	///交易所公告通知
void CTraderHandler::OnRtnBulletin(CThostFtdcBulletinField *pBulletin){
    printf("OnRtnBulletin\n");
}

	///交易通知
void CTraderHandler::OnRtnTradingNotice(CThostFtdcTradingNoticeInfoField *pTradingNoticeInfo){
    printf("OnRtnTradingNotice\n");
}

	///提示条件单校验错误
void CTraderHandler::OnRtnErrorConditionalOrder(CThostFtdcErrorConditionalOrderField *pErrorConditionalOrder){
    printf("OnRtnErrorConditionalOrder\n");
}

	///执行宣告通知
void CTraderHandler::OnRtnExecOrder(CThostFtdcExecOrderField *pExecOrder){
    printf("OnRtnExecOrder\n");
}

	///执行宣告录入错误回报
void CTraderHandler::OnErrRtnExecOrderInsert(CThostFtdcInputExecOrderField *pInputExecOrder, CThostFtdcRspInfoField *pRspInfo){
    printf("OnErrRtnExecOrderInsert\n");
}

	///执行宣告操作错误回报
void CTraderHandler::OnErrRtnExecOrderAction(CThostFtdcExecOrderActionField *pExecOrderAction, CThostFtdcRspInfoField *pRspInfo){
    printf("OnErrRtnExecOrderAction\n");
}

	///询价录入错误回报
void CTraderHandler::OnErrRtnForQuoteInsert(CThostFtdcInputForQuoteField *pInputForQuote, CThostFtdcRspInfoField *pRspInfo){
    printf("OnErrRtnForQuoteInsert\n");
}

	///报价通知
void CTraderHandler::OnRtnQuote(CThostFtdcQuoteField *pQuote){
    printf("OnRtnQuote\n");
}

	///报价录入错误回报
void CTraderHandler::OnErrRtnQuoteInsert(CThostFtdcInputQuoteField *pInputQuote, CThostFtdcRspInfoField *pRspInfo){
    printf("OnErrRtnQuoteInsert\n");
}

	///报价操作错误回报
void CTraderHandler::OnErrRtnQuoteAction(CThostFtdcQuoteActionField *pQuoteAction, CThostFtdcRspInfoField *pRspInfo){
    printf("OnErrRtnQuoteAction\n");
}

	///询价通知
void CTraderHandler::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp){
    printf("OnRtnForQuoteRsp\n");
}

	///保证金监控中心用户令牌
void CTraderHandler::OnRtnCFMMCTradingAccountToken(CThostFtdcCFMMCTradingAccountTokenField *pCFMMCTradingAccountToken){
    printf("OnRtnCFMMCTradingAccountToken\n");
}

	///批量报单操作错误回报
void CTraderHandler::OnErrRtnBatchOrderAction(CThostFtdcBatchOrderActionField *pBatchOrderAction, CThostFtdcRspInfoField *pRspInfo){
    printf("OnErrRtnBatchOrderAction\n");
}

	///期权自对冲通知
void CTraderHandler::OnRtnOptionSelfClose(CThostFtdcOptionSelfCloseField *pOptionSelfClose){
    printf("OnRtnOptionSelfClose\n");
}

	///期权自对冲录入错误回报
void CTraderHandler::OnErrRtnOptionSelfCloseInsert(CThostFtdcInputOptionSelfCloseField *pInputOptionSelfClose, CThostFtdcRspInfoField *pRspInfo){
    printf("OnErrRtnOptionSelfCloseInsert\n");
}

	///期权自对冲操作错误回报
void CTraderHandler::OnErrRtnOptionSelfCloseAction(CThostFtdcOptionSelfCloseActionField *pOptionSelfCloseAction, CThostFtdcRspInfoField *pRspInfo){
    printf("OnErrRtnOptionSelfCloseAction\n");
}

	///申请组合通知
void CTraderHandler::OnRtnCombAction(CThostFtdcCombActionField *pCombAction){
    printf("OnRtnCombAction\n");
}

	///申请组合录入错误回报
void CTraderHandler::OnErrRtnCombActionInsert(CThostFtdcInputCombActionField *pInputCombAction, CThostFtdcRspInfoField *pRspInfo){
    printf("OnErrRtnCombActionInsert\n");
}

	///请求查询签约银行响应
void CTraderHandler::OnRspQryContractBank(CThostFtdcContractBankField *pContractBank, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspQryContractBank\n");
}

	///请求查询预埋单响应
void CTraderHandler::OnRspQryParkedOrder(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspQryParkedOrder\n");
}

	///请求查询预埋撤单响应
void CTraderHandler::OnRspQryParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspQryParkedOrderAction\n");
}

	///请求查询交易通知响应
void CTraderHandler::OnRspQryTradingNotice(CThostFtdcTradingNoticeField *pTradingNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspQryTradingNotice\n");
}

	///请求查询经纪公司交易参数响应
void CTraderHandler::OnRspQryBrokerTradingParams(CThostFtdcBrokerTradingParamsField *pBrokerTradingParams, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspQryBrokerTradingParams\n");
}

	///请求查询经纪公司交易算法响应
void CTraderHandler::OnRspQryBrokerTradingAlgos(CThostFtdcBrokerTradingAlgosField *pBrokerTradingAlgos, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspQryBrokerTradingAlgos\n");
}

	///请求查询监控中心用户令牌
void CTraderHandler::OnRspQueryCFMMCTradingAccountToken(CThostFtdcQueryCFMMCTradingAccountTokenField *pQueryCFMMCTradingAccountToken, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspQueryCFMMCTradingAccountToken\n");
}

	///银行发起银行资金转期货通知
void CTraderHandler::OnRtnFromBankToFutureByBank(CThostFtdcRspTransferField *pRspTransfer){
    printf("OnRtnFromBankToFutureByBank\n");
}

	///银行发起期货资金转银行通知
void CTraderHandler::OnRtnFromFutureToBankByBank(CThostFtdcRspTransferField *pRspTransfer){
    printf("OnRtnFromFutureToBankByBank\n");
}

	///银行发起冲正银行转期货通知
void CTraderHandler::OnRtnRepealFromBankToFutureByBank(CThostFtdcRspRepealField *pRspRepeal){
    printf("OnRtnRepealFromBankToFutureByBank\n");
}

	///银行发起冲正期货转银行通知
void CTraderHandler::OnRtnRepealFromFutureToBankByBank(CThostFtdcRspRepealField *pRspRepeal){
    printf("OnRtnRepealFromFutureToBankByBank\n");
}

	///期货发起银行资金转期货通知
void CTraderHandler::OnRtnFromBankToFutureByFuture(CThostFtdcRspTransferField *pRspTransfer){
    printf("OnRtnFromBankToFutureByFuture\n");
}

	///期货发起期货资金转银行通知
void CTraderHandler::OnRtnFromFutureToBankByFuture(CThostFtdcRspTransferField *pRspTransfer){
    printf("OnRtnFromFutureToBankByFuture\n");
}

	///系统运行时期货端手工发起冲正银行转期货请求，银行处理完毕后报盘发回的通知
void CTraderHandler::OnRtnRepealFromBankToFutureByFutureManual(CThostFtdcRspRepealField *pRspRepeal){
    printf("OnRtnRepealFromBankToFutureByFutureManual\n");
}

	///系统运行时期货端手工发起冲正期货转银行请求，银行处理完毕后报盘发回的通知
void CTraderHandler::OnRtnRepealFromFutureToBankByFutureManual(CThostFtdcRspRepealField *pRspRepeal){
    printf("OnRtnRepealFromFutureToBankByFutureManual\n");
}

	///期货发起查询银行余额通知
void CTraderHandler::OnRtnQueryBankBalanceByFuture(CThostFtdcNotifyQueryAccountField *pNotifyQueryAccount){
    printf("OnRtnQueryBankBalanceByFuture\n");
}

	///期货发起银行资金转期货错误回报
void CTraderHandler::OnErrRtnBankToFutureByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo){
    printf("OnErrRtnBankToFutureByFuture\n");
}

	///期货发起期货资金转银行错误回报
void CTraderHandler::OnErrRtnFutureToBankByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo){
    printf("OnErrRtnFutureToBankByFuture\n");
}

	///系统运行时期货端手工发起冲正银行转期货错误回报
void CTraderHandler::OnErrRtnRepealBankToFutureByFutureManual(CThostFtdcReqRepealField *pReqRepeal, CThostFtdcRspInfoField *pRspInfo){
    printf("OnErrRtnRepealBankToFutureByFutureManual\n");
}

	///系统运行时期货端手工发起冲正期货转银行错误回报
void CTraderHandler::OnErrRtnRepealFutureToBankByFutureManual(CThostFtdcReqRepealField *pReqRepeal, CThostFtdcRspInfoField *pRspInfo){
    printf("OnErrRtnRepealFutureToBankByFutureManual\n");
}

	///期货发起查询银行余额错误回报
void CTraderHandler::OnErrRtnQueryBankBalanceByFuture(CThostFtdcReqQueryAccountField *pReqQueryAccount, CThostFtdcRspInfoField *pRspInfo){
    printf("OnErrRtnQueryBankBalanceByFuture\n");
}

	///期货发起冲正银行转期货请求，银行处理完毕后报盘发回的通知
void CTraderHandler::OnRtnRepealFromBankToFutureByFuture(CThostFtdcRspRepealField *pRspRepeal){
    printf("OnRtnRepealFromBankToFutureByFuture\n");
}

	///期货发起冲正期货转银行请求，银行处理完毕后报盘发回的通知
void CTraderHandler::OnRtnRepealFromFutureToBankByFuture(CThostFtdcRspRepealField *pRspRepeal){
    printf("OnRtnRepealFromFutureToBankByFuture\n");
}

	///期货发起银行资金转期货应答
void CTraderHandler::OnRspFromBankToFutureByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspFromBankToFutureByFuture\n");
}

	///期货发起期货资金转银行应答
void CTraderHandler::OnRspFromFutureToBankByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspFromFutureToBankByFuture\n");
}

	///期货发起查询银行余额应答
void CTraderHandler::OnRspQueryBankAccountMoneyByFuture(CThostFtdcReqQueryAccountField *pReqQueryAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    printf("OnRspQueryBankAccountMoneyByFuture\n");
}

	///银行发起银期开户通知
void CTraderHandler::OnRtnOpenAccountByBank(CThostFtdcOpenAccountField *pOpenAccount){
    printf("OnRtnOpenAccountByBank\n");
}

	///银行发起银期销户通知
void CTraderHandler::OnRtnCancelAccountByBank(CThostFtdcCancelAccountField *pCancelAccount){
    printf("OnRtnCancelAccountByBank\n");
}

	///银行发起变更银行账号通知
void CTraderHandler::OnRtnChangeAccountByBank(CThostFtdcChangeAccountField *pChangeAccount){
    printf("OnRtnChangeAccountByBank\n");
}
