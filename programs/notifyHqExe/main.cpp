#include "hqRedis.h"
#include "../notifyHq/dll.h"

typedef My_DLLEXP  int(*DllFun)();

extern HqMdUser* apiMdUser;
HqRedis *hqRedis = 0;

int main() {

    do {
		my_ctp_init();

		const char *redisIp = Profile::getInstance()->getStringCache("MY_REDIS_IP", "127.0.0.1");
		const char *redisPort = Profile::getInstance()->getStringCache("MY_REDIS_PORT", "6379");
		const char *redisPassword = Profile::getInstance()->getStringCache("MY_REDIS_PASSWORD", "");
		const char *redisDbNum = Profile::getInstance()->getStringCache("MY_REDIS_DBNUM", "0");

		hqRedis = new HqRedis(apiMdUser);

		hqRedis->start(redisIp, atoi(redisPort), redisPassword, atoi(redisDbNum));

		my_ctp_loop();
	} while (false);

	my_ctp_uninit();

    return 0;
}