#include "hqRedis.h"

#include "../../third/cpp_redis/includes/cpp_redis/cpp_redis"
#include "../../third/ctp/ThostFtdcUserApiStruct.h"

#include "../../impl/ctpApiMdUser.h"

#include "../../third/jsoncpp/json.hpp"

void ctpMdRegQuoteStk(void *p, const char* stks);

HqRedis::HqRedis(CtpApiMdUser* apiMdUser){
	p_redis_client = new cpp_redis::client();
  	p_redis_async_client = new cpp_redis::subscriber();
	this->apiMdUser = apiMdUser;
}

// void my_redis_async_connect_callback(struct my_redis_async_client* async_client) {
// 	if (async_client == nullptr)
// 		return;

// 	my_redis_async_client_subscribe(async_client, "stk_changed", [](struct my_redis_async_client* async_client, const char* channel, const char* message) {
		
// 	});
// }

void HqRedis::start(const char* ip, int port, const  char* psw, int dbNum){
	std::string sIp = ip;
	std::uint32_t uPort = (std::uint32_t)port;
	std::uint32_t timeOut = 10000;
	p_redis_client->connect(sIp, uPort, timeOut);
	p_redis_async_client->connect(sIp, uPort, timeOut);
	p_redis_async_client->subscribe("stk_changed", [this](const std::string& chan, const std::string& msg) {

		if (!apiMdUser)
			return;

		ctpMdRegQuoteStk(apiMdUser, msg.c_str());
	});

	p_redis_async_client->commit();

	my_redis_tool_publish("resend_stk_changed", "");

	apiMdUser->setCallback_RtnDepthMarketData([this](CThostFtdcDepthMarketDataField* pDepthMarketData){
		Json::Value quote;
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

		double prevSettlement = pDepthMarketData->PreSettlementPrice;
		double price = pDepthMarketData->LastPrice;

		if prevSettlement == 0 {
			prevSettlement = pDepthMarketData->OpenPrice;
		}

		quote["updnPrice"] = price - prevSettlement;
		if (prevSettlement > 0) {
			quote["updnPricePer"] = ((price-prevSettlement)*100.0f/prevSettlement)+0.0000005;
		} else {
			// quote["updnPricePer"] = nil
		}
		
		Json::Value jsonRoot;
		jsonRoot["cat"] = "quote";
		jsonRoot["oper"] = "realtime";
		jsonRoot["data"] = quote;
		my_redis_tool_publish("stk_quote_changed", jsonRoot.toStyledString().c_str());
	});
}

void HqRedis::stop(){
	if(apiMdUser){
		apiMdUser->setCallback_RtnDepthMarketData(0);
		apiMdUser = 0;
	}

	if (p_redis_async_client != 0) {
		p_redis_async_client->clear_sentinels();
		delete p_redis_async_client;
		p_redis_async_client = 0;
	}
	if (p_redis_client != 0) {
		p_redis_client->clear_sentinels();
		delete p_redis_client;
		p_redis_client = 0;
	}	
}

int HqRedis::my_redis_tool_publish(const char* channel, const char* message) {
	if (p_redis_client == 0)
		return -1;
	p_redis_client->publish(channel, message);
	p_redis_client->commit();
	return 0;
}