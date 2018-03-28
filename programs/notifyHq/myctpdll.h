#if !defined(__NOTIFY_HQ_DLL_H__)
#define __NOTIFY_HQ_DLL_H__

// #if BUILDING_MYCTP_EXPORT
//     #if defined _MSC_VER || defined _WIN32 || defined __CYGWIN__
//         #define LIBCTP_DLL_EXPORTED __declspec(dllexport)
//     #elif __GNUC__ >= 4
//         #define LIBCTP_DLL_EXPORTED __attribute__((__visibility__("default")))
//     #else
//         #define LIBCTP_DLL_EXPORTED
//     #endif
// #else
//     #if defined _MSC_VER || defined _WIN32 || defined __CYGWIN__
//         #define LIBCTP_DLL_EXPORTED __declspec(dllimport)
//     #elif __GNUC__ >= 4
//         #define LIBCTP_DLL_EXPORTED __attribute__((__visibility__("default")))
//     #else
//         #define LIBCTP_DLL_EXPORTED
//     #endif
// #endif

#define LIBCTP_DLL_EXPORTED extern

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

LIBCTP_DLL_EXPORTED int my_ctp_init();
LIBCTP_DLL_EXPORTED int my_ctp_loop();
LIBCTP_DLL_EXPORTED int my_ctp_uninit();

LIBCTP_DLL_EXPORTED const char* getExchangesJsonStr();

typedef void (*NotifyStringCallback)(const char *);
LIBCTP_DLL_EXPORTED void setRealtimeQuoteNotifyCallback(NotifyStringCallback cb);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif