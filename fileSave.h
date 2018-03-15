#ifndef _MY_FILE_SAVE_H__
#define _MY_FILE_SAVE_H__

void myFileWrite(const char *path, const char *content);

// return value need call free()
const char *myFileRead(const char *path);

#endif
