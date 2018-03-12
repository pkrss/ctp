
#include "redis_client.h"
#include "../../third/redis_win/deps/hiredis/win32_hiredis.h"

struct my_redis_client {
	redisContext* client;
	char* ip;
	int port;
	char* psw;
};

//void my_redis_async_client_onDisConnectedCallback(const struct redisAsyncContext* client, int status) {
//
//	if (_my_redis_sys_enable_log)
//		printf("Redis disConnection status:%d error: %s\n", status, (client != 0 ? client->errstr : "unknown error"));
//
//	struct my_redis_async_client* async_client = client->data;
//
//	if ((async_client == 0) || (async_client->client == 0))
//		return;
//
//	Sleep(10 * 1000);
//
//	async_client->client = 0;
//
//	my_redis_client_reconnect_client(async_client);
//}

int my_redis_client_reconnect_client(my_redis_client* client) {
	if (client == 0)
		return -1;

	printf("redis client prepare connect [%s:%d] password:%s\n", client->ip, client->port, (client->psw != 0 ? client->psw : ""));


	if (client->client != 0) {
		redisFree(client->client);
		// redisAsyncFree(g_my_redis_client->client);
		client->client = 0;
	}

	struct timeval timeout = { 10, 500000 }; // 10.5 seconds
	client->client = redisConnectWithTimeout(client->ip, client->port, timeout);
	if (client->client == 0) {
		// my_redis_client_onDisConnectedCallback(client->client, -1);
		return -1;
	}

	printf("Redis connection status:%d error: %s\n", client->client->err, client->client->errstr);

	if (client->client->err) {
		// my_redis_async_client_onDisConnectedCallback(client->client, client->client->err);
		return -1;
	}

	if (client->psw != 0)
		return my_redis_client_send_command(client, "AUTH %s", client->psw);

	return 0;
}

my_redis_client* my_redis_client_connect_client(const char* ip, int port, const char* psw) {
	my_redis_client* client = (my_redis_client*)malloc(sizeof(*client));
	// my_redis_client_free();

	client->client = 0;
	client->ip = (char*)malloc(strlen(ip) + 1);
	strcpy(client->ip, ip);
	client->port = port;

	if (psw) {
		client->psw = (char*)malloc(strlen(psw) + 1);
		strcpy(client->psw, psw);
	}
	else {
		client->psw = 0;
	}

	if (0 != my_redis_client_reconnect_client(client)) {
		my_redis_client_free(client);
		client = 0;
	}

	return client;
}

int my_redis_client_send_command(my_redis_client* client, const char* format, ...) {

	if ((client == 0) || (client->client == 0))
		return 0;

	if ((format == 0) || (format[0] == 0))
		return 0;

	va_list ap;
	redisReply *reply;

	va_start(ap, format);
	reply = (redisReply *)redisvCommand(client->client, format, ap);
	va_end(ap);

	va_start(ap, format);

	char* s = new char[1024];
	strcpy(s, "redis send: ");
	int len = vsnprintf(&s[12], 1000, format, ap);
	if (len > 0) {
		s[len++] = '\0';
		printf(s);
	}
	va_end(ap);

	freeReplyObject(reply);

	return reply != 0 ? 0 : -1;
}

int my_redis_client_publish(my_redis_client* client, const char* channel, const char* message) {
	return my_redis_client_send_command(client, "publish %s %s", channel, message);
}

void my_redis_client_free(my_redis_client* client) {
	if (client == 0)
		return;

	if (client->client != 0) {
		redisFree(client->client);
		// redisAsyncFree(g_my_redis_client->client);
		client->client = 0;
	}
	if (client->ip != 0) {
		free(client->ip);
		client->ip = 0;
	}
	if (client->psw != 0) {
		free(client->psw);
		client->psw = 0;
	}

	free(client);
}