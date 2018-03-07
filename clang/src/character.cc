#include "character.h"


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

const char* utf8_to_gbk(const char* src) {
	int len = strlen(src) * 2 + 1;
	char* ret = (char*)malloc(len);

	utf8_to_gb((char*)src, ret, len);

	return ret;
}

const char* gbk_to_utf8(const char* src) {
	int len = strlen(src)*3 + 1;

	char* ret = (char*)malloc(len);

	gb_to_utf8((char*)src, ret, len);

	return ret;
}

#if 0 && defined(_WINDOWS)
#include <windows.h>

void utf8_to_gb(char* src, char* dst, int len)
{
	int ret = 0;
	wchar_t* strA;
	int i = MultiByteToWideChar(CP_UTF8, 0, src, -1, NULL, 0);
	if (i <= 0) {
		printf("ERROR.");
		return;
	}
	strA = (wchar_t*)malloc(i * 2);
	MultiByteToWideChar(CP_UTF8, 0, src, -1, strA, i);
	i = WideCharToMultiByte(CP_ACP, 0, strA, -1, NULL, 0, NULL, NULL);
	if (len >= i) {
		ret = WideCharToMultiByte(CP_ACP, 0, strA, -1, dst, i, NULL, NULL);
		dst[i] = 0;
	}
	if (ret <= 0) {
		free(strA);
		return;
	}

	free(strA);
}

void gb_to_utf8(char* src, char* dst, int len)
{
	int ret = 0;
	wchar_t* strA;
	int i = MultiByteToWideChar(CP_ACP, 0, src, -1, NULL, 0);
	if (i <= 0) {
		printf("ERROR.");
		return;
	}
	strA = (wchar_t*)malloc(i * 2);
	MultiByteToWideChar(CP_ACP, 0, src, -1, strA, i);
	i = WideCharToMultiByte(CP_UTF8, 0, strA, -1, NULL, 0, NULL, NULL);
	if (len >= i) {
		ret = WideCharToMultiByte(CP_UTF8, 0, strA, -1, dst, i, NULL, NULL);
		dst[i] = 0;
	}

	if (ret <= 0) {
		free(strA);
		return;
	}
	free(strA);
}
#else   //Linux

#include "third/iconv/iconv.h"

void codeConvert(const char * from, const char * to, char * src, char * dst, size_t dstSize) {
	size_t sl, dl;
	sl = strlen(src);
	dl = dstSize;
	char * pIn = src;
	char * pOut = dst;
	memset(dst, '\0', dstSize);
	iconv_t conv = iconv_open(to, from);
	iconv(conv, &pIn, &sl, &pOut, &dl);
	iconv_close(conv);
}

// starkwong: In iconv implementations, inlen and outlen should be type of size_t not uint, which is different in length on Mac
void utf8_to_gb(char* src, char* dst, int len)
{
	codeConvert("UTF-8", "GBK", src, dst, len);
}

void gb_to_utf8(char* src, char* dst, int len)
{
	codeConvert("GBK", "UTF-8", src, dst, len);
}
#endif