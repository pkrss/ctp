#include "ext.h"
#include "utils.h"
// mkdir
#include <direct.h>  

// char *profileGetString(const char *key)
// {
//   return ExtProfileGetString((char*)key);
// }

// void profileFreeString(void *p)
// {
//   if (!p)
//     return;
//   free(p);
// }

std::list<std::string> SplitString(const std::string &s, const std::string &c)
{
  std::string::size_type pos1, pos2;
  pos2 = s.find(c);
  pos1 = 0;

  std::list<std::string> v;

  while (std::string::npos != pos2)
  {
    v.push_back(s.substr(pos1, pos2 - pos1));

    pos1 = pos2 + c.size();
    pos2 = s.find(c, pos1);
  }
  if (pos1 != s.length())
    v.push_back(s.substr(pos1));

  return v;
}

void mkdir_r(const char *dir) {
	char* tmp = strdup(dir);
	char *p = NULL;
	size_t len;

	len = strlen(tmp);
	if (tmp[len - 1] == '/')
		tmp[len - 1] = 0;
	for (p = tmp + 1; *p; p++) {
		if (*p == '/') {
			*p = 0;
			_mkdir(tmp);
			*p = '/';
		}
	}
	_mkdir(tmp);
	free(tmp);
}