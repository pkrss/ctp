#if !defined(__EXT_H__)
#define __EXT_H__

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

char* ExtProfileGetString(char* key);
char* ExtGbkToUtf8(char* gbkStr);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
