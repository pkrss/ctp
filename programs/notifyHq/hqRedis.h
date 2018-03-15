#ifndef __HQ_REDIS__H__
#define __HQ_REDIS__H__

class CtpApiMdUser;

namespace cpp_redis{
  class client;
  class subscriber;
}

class HqRedis
{
public:
  HqRedis(CtpApiMdUser* apiMdUser);
  void start(const char *ip, int port, const char *psw, int dbNum);

  void stop();

  int my_redis_tool_publish(const char *channel, const char *message);

private:
  cpp_redis::client *p_redis_client;
  cpp_redis::subscriber *p_redis_async_client;
public:
  CtpApiMdUser* apiMdUser;
};

#endif

