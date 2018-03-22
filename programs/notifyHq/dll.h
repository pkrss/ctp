#if !defined(__NOTIFY_HQ_DLL_H__)
#define __NOTIFY_HQ_DLL_H__

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

int my_ctp_init();
int my_ctp_loop();
int my_ctp_uninit();

const char* getExchangesJsonStr();

const char* getExchangesJsonStr();

typedef void (*NotifyStringCallback)(const char *);
void setRealtimeQuoteNotifyCallback(NotifyStringCallback cb);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif