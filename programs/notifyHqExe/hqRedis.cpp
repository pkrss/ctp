#include "hqRedis.h"

#include "../../third/jsoncpp/json.hpp"
#include "../../third/ctp/ThostFtdcUserApiStruct.h"

#include "../notifyHq/hqMdUser.h"

#include "../../third/cpp_redis/includes/cpp_redis/cpp_redis"


extern HqRedis* hqRedis;

void onRecvDepthMarketDataField(const char *pDepthMarketData) {	
	if (!pDepthMarketData || !(*pDepthMarketData) || !hqRedis || !hqRedis->apiMdUser)
		return;

	hqRedis->my_redis_tool_publish("stk_quote_changed", pDepthMarketData);
};

 void onRecvHqStkChanged(const std::string& chan, const std::string& msg) {

	if (!hqRedis || !hqRedis->apiMdUser)
		return;

	// RecordsMem<CThostFtdcInstrumentField>::getInstance()->resetAll(p, n);

	hqRedis->apiMdUser->RegQuoteStk(msg.c_str());
}

HqRedis::HqRedis(HqMdUser* apiMdUser){
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