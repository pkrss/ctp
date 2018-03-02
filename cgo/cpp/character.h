#ifndef __CHACACTER__H__
#define __CHACACTER__H__


	void utf8_to_gb(char* src, char* dst, int len);
	void gb_to_utf8(char* src, char* dst, int len);

	extern	const char* utf8_to_gbk(const char* src);
extern	const char* gbk_to_utf8(const char* src);



#endif