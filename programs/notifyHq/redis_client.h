#ifndef _REDIS_CLIENT_H__
#define _REDIS_CLIENT_H__ 1

struct my_redis_client;

my_redis_client* my_redis_client_connect_client(const char* ip, int port, const char* psw);
void my_redis_client_free(my_redis_client* client);
int my_redis_client_send_command(my_redis_client* client, const char* format, ...);
int my_redis_client_publish(my_redis_client* client, const char* channel, const char* message);

#endif
