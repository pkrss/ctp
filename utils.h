#if !defined(__UTILS_H__)
#define __UTILS_H__

#include <list>
#include <string>

// char* profileGetString(const char* key);
// void profileFreeString(void* p);

std::list<std::string> SplitString(const std::string &s, const std::string &c);

void mkdir_r(const char *dir);
#endif