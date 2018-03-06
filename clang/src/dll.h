#ifndef _MY_DLL_H__
#define _MY_DLL_H__

void* so_open(const char* so);
void* so_find(void* so, const char* funcName);
void so_free(void* so);

#endif // !_MY_DLL_H__
