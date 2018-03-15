#include "hqRedis.h"

#include "../../third/jsoncpp/json.hpp"
#include "../../third/ctp/ThostFtdcUserApiStruct.h"

#include "../../impl/ctpApiMdUser.h"

#include "../../third/cpp_redis/includes/cpp_redis/cpp_redis"

#include "../../third/jsoncpp/json.hpp"
using json = nlohmann::json;

#include "../../impl/ctpApiMd.h"

HqRedis* hqRedis = 0;

void onRecvDepthMarketDataField(CThostFtdcDepthMarketDataField *pDepthMarketData) {	
	if (!hqRedis || !hqRedis->apiMdUser)
		return;

	json quote;
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
	
	json jsonRoot;
	jsonRoot["cat"] = "quote";
	jsonRoot["oper"] = "realtime";
	jsonRoot["data"] = quote;

	std::string s = jsonRoot.dump();
	hqRedis->my_redis_tool_publish("stk_quote_changed", s.c_str());
};

 void onRecvHqStkChanged(const std::string& chan, const std::string& msg) {

	if (!hqRedis || !hqRedis->apiMdUser)
		return;

	ctpMdRegQuoteStk(hqRedis->apiMdUser, msg.c_str());
}

HqRedis::HqRedis(CtpApiMdUser* apiMdUser){
	p_redis_client = new cpp_redis::client();
  	p_redis_async_client = new cpp_redis::subscriber();
	this->apiMdUser = apiMdUser;
	hqRedis = this;
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
	// std::uint32_t timeOut = 10000;
	p_redis_client->connect(sIp, uPort, [](const std::string& host, std::size_t port, cpp_redis::client::connect_state status){
		const char *s = "unknwon";
		switch(status){
			case cpp_redis::client::connect_state::dropped:
				s = "dropped";
				break;
			case cpp_redis::client::connect_state::start:
				s = "start";
				break;
			case cpp_redis::client::connect_state::sleeping:
				s = "sleeping";
				break;
			case cpp_redis::client::connect_state::ok:
				s = "ok";
				break;
			case cpp_redis::client::connect_state::failed:
				s = "failed";
				break;
			case cpp_redis::client::connect_state::lookup_failed:
				s = "lookup_failed";
				break;
			case cpp_redis::client::connect_state::stopped:
				s = "stopped";
				break;
		}

		printf("redis client connect %s:%d, %s\n", host.c_str(), (int)port, s);
	});

	if(psw && *psw){
		p_redis_client->auth(psw, [](const cpp_redis::reply& reply) {
			if (reply.is_error()) { 
				printf("redis client Authentication failed: %s\n", reply.as_string().c_str());
			} else {
				printf("redis client successful authentication\n");
			}
		});
	}

	p_redis_async_client->connect(sIp, uPort, [](const std::string& host, std::size_t port, cpp_redis::subscriber::connect_state status){
		const char *s = "unknwon";
		switch(status){
			case cpp_redis::subscriber::connect_state::dropped:
				s = "dropped";
				break;
			case cpp_redis::subscriber::connect_state::start:
				s = "start";
				break;
			case cpp_redis::subscriber::connect_state::sleeping:
				s = "sleeping";
				break;
			case cpp_redis::subscriber::connect_state::ok:
				s = "ok";
				break;
			case cpp_redis::subscriber::connect_state::failed:
				s = "failed";
				break;
			case cpp_redis::subscriber::connect_state::lookup_failed:
				s = "lookup_failed";
				break;
			case cpp_redis::subscriber::connect_state::stopped:
				s = "stopped";
				break;
		}

		printf("redis subscribe connect %s:%d, %s\n", host.c_str(), (int)port, s);
	});
	
	if(psw && *psw){
		p_redis_async_client->auth(psw, [](const cpp_redis::reply& reply) {
			if (reply.is_error()) { 
				printf("redis subscribe Authentication failed: %s\n", reply.as_string().c_str());
			} else {
				printf("redis subscribe successful authentication\n");
			}
		});
	}

	p_redis_async_client->subscribe("stk_changed", onRecvHqStkChanged);

	p_redis_async_client->commit();

	my_redis_tool_publish("resend_stk_changed", "");

	apiMdUser->setCallback_RtnDepthMarketData((void*)onRecvDepthMarketDataField);
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