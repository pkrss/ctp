#ifndef __HQ_REDIS__H__
#define __HQ_REDIS__H__

struct my_redis_client;
struct my_redis_async_client;
class CtpApiMdUser* apiMdUser;

class HqRedis
{
public:
  HqRedis(CtpApiMdUser* apiMdUser);
  void start(char *ip, int port, char *psw, int dbNum);

  void stop();

  int my_redis_tool_publish(const char *channel, const char *message);

private:
  my_redis_client *p_redis_client;
  my_redis_async_client *p_redis_async_client;
  CtpApiMdUser* apiMdUser;
};

#endif

