#ifndef _REDIS_ASYNC_CLIENT_H__
#define _REDIS_ASYNC_CLIENT_H__ 1

struct my_redis_async_client;

typedef void (my_redis_async_connected_callback)(my_redis_async_client* async_client);
typedef void (my_redis_subscribe_callback)(my_redis_async_client* async_client, const char* channel, const char* message);

my_redis_async_client* my_redis_async_client_connect_client(const char* ip, int port, const char* psw, my_redis_async_connected_callback* connected_callback);
void my_redis_async_client_free(my_redis_async_client* async_client);
int my_redis_async_client_subscribe(my_redis_async_client* async_client, const char* channel, my_redis_subscribe_callback* callback);

#endif
