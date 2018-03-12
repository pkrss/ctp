
#include "redis_async_client.h"

extern "C" {
#include "../../third/redis_win/deps/hiredis/win32_hiredis.h"
#include "../../third/redis_win/deps/hiredis/async.h"
#include "../../third/redis_win/deps/hiredis/adapters/ae.h"

}

#include "my_list.h"

extern struct aeEventLoop* _my_redis_event_loop;


struct my_redis_async_client {
	redisAsyncContext* client;
	char* ip;
	int port;
	char* psw;
	my_redis_async_connected_callback* connected_callback;
	int connected_callback_called;
	struct my_list_struct* subscribe_list;
};

struct my_redis_subscribe_data {
	char* channel;
	my_redis_subscribe_callback* callback;
};

REDIS_DLL_API void my_redis_async_client_free(my_redis_async_client* async_client) {
	if (async_client == 0)
		return;

	if (async_client->client != 0) {
		redisAsyncDisconnect(async_client->client);
		// redisAsyncFree(g_my_redis_client->client);
		async_client->client = 0;
	}
	if (async_client->ip != 0) {
		free(async_client->ip);
		async_client->ip = 0;
	}
	if (async_client->psw != 0) {
		free(async_client->psw);
		async_client->psw = 0;
	}
	if (async_client->subscribe_list != 0) {
		my_list_struct_destroy(async_client->subscribe_list);
		async_client->subscribe_list = 0;
	}

	free(async_client);
}

void my_redis_async_client_onMessageCallback(redisAsyncContext* redisAsyncClient, void* reply, void* privdata) {
	my_redis_async_client* client = (my_redis_async_client*)privdata;
	if (client == 0)
		return;

	redisReply *r = (redisReply *)reply;
	if (reply == 0) return;

	if (my_log_get_enabled()) {
		const char* typeStr = "unknown";
		switch (r->type) {
		case REDIS_REPLY_STRING:
			typeStr = "string";
			break;
		case REDIS_REPLY_ARRAY:
			typeStr = "array";
			break;
		case REDIS_REPLY_NIL:
			typeStr = "nil";
			break;
		case REDIS_REPLY_STATUS:
			typeStr = "status";
			break;
		case REDIS_REPLY_ERROR:
			typeStr = "error";
			break;
		}

		my_log("redis async reply type:%d %s\n", r->type, typeStr);
	}

	if (r->type == REDIS_REPLY_ARRAY) {
		if (r->elements > 2) {
			if (0 == stricmp("subscribe", r->element[0]->str)) {

			}
			else if (0 == stricmp("message", r->element[0]->str)) {
				if ((r->element[2]->str) && (client->subscribe_list != 0)) {
					struct my_list_node* node = my_list_struct_first(client->subscribe_list);
					while (node != 0) {
						struct my_redis_subscribe_data* node_data = (struct my_redis_subscribe_data*)my_list_struct_val(node);
						if (0 == stricmp(node_data->channel, r->element[1]->str))
							(*(node_data->callback))(client, r->element[1]->str, r->element[2]->str);
						node = my_list_struct_next(node);
					}
				}
			}
		}

		if (my_log_get_enabled()) {
			for (int j = 0; j < (int)r->elements; j++) {
				my_log("redis async reply array %u) %s\n", j, r->element[j]->str);
			}
		}
	}
}


REDIS_DLL_API int my_redis_client_async_send_command(my_redis_async_client* async_client, const char* format, ...) {
	if ((async_client == 0) || (async_client->client == 0))
		return 0;

	if ((format == 0) || (format[0] == 0))
		return 0;

	va_list ap;
	int status;
	va_start(ap, format);
	status = redisvAsyncCommand(async_client->client, my_redis_async_client_onMessageCallback, async_client, format, ap);
	va_end(ap);

	if (my_log_get_enabled()) {
		my_log("redis async send: ");
		va_start(ap, format);
		_vfprintf_l(stdout, format, NULL, ap);
		va_end(ap);
		my_log("\r\n");
	}

	return status;
}

void my_redis_async_client_onConnectedCallback(const struct redisAsyncContext* client, int status) {
	if (my_log_get_enabled())
		my_log("Redis async connection status:%d error: %s\n", status, client->errstr);

	my_redis_async_client* async_client = (my_redis_async_client*)client->data;

	if ((async_client == 0) || (async_client->client == 0))
		return;

	if (async_client->connected_callback_called == 0) {
		async_client->connected_callback_called = 1;
		if (async_client->connected_callback != 0)
			async_client->connected_callback(async_client);
	}
}

void my_redis_async_client_reconnect_client(my_redis_async_client* async_client);

void my_redis_async_client_onDisConnectedCallback(const struct redisAsyncContext* client, int status) {

	if (my_log_get_enabled())
		my_log("Redis async disConnection status:%d error: %s\n", status, (client != 0 ? client->errstr : "unknown error"));

	my_redis_async_client* async_client = (my_redis_async_client* )client->data;

	if ((async_client == 0) || (async_client->client == 0))
		return;

	Sleep(10 * 1000);

	async_client->client = 0;

	my_redis_async_client_reconnect_client(async_client);
}


void my_redis_async_client_reconnect_client(my_redis_async_client* async_client) {
	if (async_client == 0)
		return;

	if (my_log_get_enabled())
		my_log("redis async client prepare connect [%s:%d] password:%s\n", async_client->ip, async_client->port, (async_client->psw != 0 ? async_client->psw : ""));


	if (async_client->client != 0) {
		redisAsyncDisconnect(async_client->client);
		// redisAsyncFree(g_my_redis_client->client);
		async_client->client = 0;
	}

	async_client->connected_callback_called = 0;

	async_client->client = redisAsyncConnect(async_client->ip, async_client->port);
	if (async_client->client == 0) {
		my_redis_async_client_onDisConnectedCallback(async_client->client, -1);
		return;
	}
	if (async_client->client->err) {
		my_redis_async_client_onDisConnectedCallback(async_client->client, async_client->client->err);
		return;
	}

	async_client->client->data = async_client;

	redisAsyncSetConnectCallback(async_client->client, my_redis_async_client_onConnectedCallback);
	redisAsyncSetDisconnectCallback(async_client->client, my_redis_async_client_onDisConnectedCallback);

	if (_my_redis_event_loop != 0)
		redisAeAttach(_my_redis_event_loop, async_client->client);

	if (async_client->psw != 0)
		my_redis_client_async_send_command(async_client, "AUTH %s", async_client->psw);

	// call now
	my_redis_async_client_onConnectedCallback(async_client->client, async_client->client->err);

}

REDIS_DLL_API my_redis_async_client* my_redis_async_client_connect_client(const char* ip, int port, const char* psw, my_redis_async_connected_callback* connected_callback) {
	my_redis_async_client* async_client = (my_redis_async_client*)malloc(sizeof(*async_client));
	// my_redis_client_free();

	async_client->client = 0;
	async_client->ip = (char*)malloc(strlen(ip) + 1);
	strcpy(async_client->ip, ip);
	async_client->port = port;
	async_client->connected_callback_called = 0;

	if (psw) {
		async_client->psw = (char*)malloc(strlen(psw) + 1);
		strcpy(async_client->psw, psw);
	}
	else {
		async_client->psw = 0;
	}
	async_client->connected_callback = connected_callback;

	async_client->subscribe_list = 0;

	my_redis_async_client_reconnect_client(async_client);

	return async_client;
}

void _my_redis_subscribe_list_node_free(void* data) {
	struct my_redis_subscribe_data* node_data = (struct my_redis_subscribe_data*)data;
	free(node_data->channel);
	free(data);
}

REDIS_DLL_API int my_redis_async_client_subscribe(my_redis_async_client* async_client, const char* channel, my_redis_subscribe_callback* callback) {
	if (async_client == 0)
		return -1;

	if (async_client->subscribe_list == 0)
		async_client->subscribe_list = my_list_struct_create(&_my_redis_subscribe_list_node_free);

	int found = 0;

	struct my_list_node* node = my_list_struct_first(async_client->subscribe_list);
	while (node != 0) {
		struct my_redis_subscribe_data* node_data = (struct my_redis_subscribe_data*)my_list_struct_val(node);
		if (0 == stricmp(node_data->channel, channel)) {
			node_data->callback = callback;
			found = 1;
			break;
		}
		node = my_list_struct_next(node);
	}

	if (!found) {
		struct my_redis_subscribe_data* node_data = (struct my_redis_subscribe_data*)malloc(sizeof(struct my_redis_subscribe_data));
		node_data->channel = (char*)malloc(strlen(channel) + 1);
		strcpy(node_data->channel, channel);
		node_data->callback = callback;
		my_list_struct_push(async_client->subscribe_list, node_data);
	}

	return my_redis_client_async_send_command(async_client, "subscribe %s", channel);
}

