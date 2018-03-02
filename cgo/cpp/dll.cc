#include "dll.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef _WINDOWS
#include <windows.h>

void* so_open(const char* so) {
	if (!so || !so[0])
		return 0;
	HMODULE a = LoadLibrary(so);
	
	if (!a && !strstr(so, ".dll"))
	{
		char* so2 = (char*)malloc(strlen(so) + 10);
		strcpy(so2, so);
		strcat(so2, ".dll");
		a = LoadLibrary(so2);
		free(so2);
	}

	if (!a)
	{
		printf("Fail to LoadLibrary(%s) = 0x%x\n", so, GetLastError());
		return 0;
	}
	return a;
}

void* so_find(void* so, const char* funcName) {
	if (!so || !funcName || !funcName[0])
		return 0;
	return GetProcAddress((HMODULE)so, funcName);
}

void so_free(void* so) {
	if (!so)
		return;
	FreeLibrary((HMODULE)so);
}

#else
#include <dlfcn.h>

void* so_open(const char* so) {
	if (!so || !so[0])
		return 0;

	void* pdlhandle;
	char* pszerror;

	pdlhandle = dlopen("./libtest.so", RTLD_LAZY);
	pszerror = dlerror();

	if (!pdlhandle && !strstr(so, ".so"))
	{
		char* so2 = (char*)malloc(strlen(so) + 32);
		strcpy(so2, so);
		strcat(so2, ".so");
		pdlhandle = dlopen(so2, RTLD_LAZY);

		if (!pdlhandle) {
			strcpy(so2, "lib");
			strcat(so2, so);
			strcat(so2, ".so");

			pdlhandle = dlopen(so2, RTLD_LAZY);
		}

		free(so2);

	}

	if (0 != pszerror)
	{
		printf("%s", pszerror);
		return 0;
	}

	return pdlhandle;
}

void* so_find(void* so, const char* funcName) {
	if (!so || !funcName || !funcName[0])
		return 0;

	void* fn = dlsym(so, funcName);
	pszerror = dlerror();
	if (0 != pszerror)
	{
		printf("%s", pszerror);
		return 0;
	}
	return fn;
}

void so_free(void* so) {
	if (!so)
		return;
	dlclose(so);
}


#endif