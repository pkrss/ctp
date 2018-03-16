#include "ext.h"
#include "ext.h"
// mkdir
#include <direct.h>  

char *profileGetString(const char *key)
{
  return ExtProfileGetString((char*)key);
}

void profileFreeString(void *p)
{
  if (!p)
    return;
  free(p);
}