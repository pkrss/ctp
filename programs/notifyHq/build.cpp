
// #if defined(_WINDOWS)
// #define INCL_WINSOCK_API_TYPEDEFS 1
// #define WINVER _WIN32_WINNT_WS03
// // #include <Winsock2.h>
// #include <Ws2tcpip.h>
// #endif

#include "../../third/cpp_redis/sources/builders/array_builder.cpp"
#include "../../third/cpp_redis/sources/builders/builders_factory.cpp"
#include "../../third/cpp_redis/sources/builders/bulk_string_builder.cpp"
#include "../../third/cpp_redis/sources/builders/error_builder.cpp"
#include "../../third/cpp_redis/sources/builders/integer_builder.cpp"
#include "../../third/cpp_redis/sources/builders/reply_builder.cpp"
#include "../../third/cpp_redis/sources/builders/simple_string_builder.cpp"
#include "../../third/cpp_redis/sources/core/client.cpp"
#include "../../third/cpp_redis/sources/core/reply.cpp"
#include "../../third/cpp_redis/sources/core/sentinel.cpp"
#include "../../third/cpp_redis/sources/core/subscriber.cpp"
#include "../../third/cpp_redis/sources/misc/logger.cpp"
#include "../../third/cpp_redis/sources/network/redis_connection.cpp"
#include "../../third/cpp_redis/sources/network/tcp_client.cpp"

#include "../../third/cpp_redis/tacopie/sources/network/common/tcp_socket.cpp"
#include "../../third/cpp_redis/tacopie/sources/network/io_service.cpp"
#include "../../third/cpp_redis/tacopie/sources/network/tcp_client.cpp"
#include "../../third/cpp_redis/tacopie/sources/network/tcp_server.cpp"
#if defined(_WINDOWS)
#include "../../third/cpp_redis/tacopie/sources/network/windows/windows_self_pipe.cpp"
#include "../../third/cpp_redis/tacopie/sources/network/windows/windows_tcp_socket.cpp"
#else
#include "../../third/cpp_redis/tacopie/sources/network/unix/unix_self_pipe.cpp"
#include "../../third/cpp_redis/tacopie/sources/network/unix/unix_tcp_socket.cpp"
#endif
#include "../../third/cpp_redis/tacopie/sources/utils/error.cpp"
#include "../../third/cpp_redis/tacopie/sources/utils/logger.cpp"
#include "../../third/cpp_redis/tacopie/sources/utils/thread_pool.cpp"


// #include "../../third/jsoncpp/jsoncpp.cpp"