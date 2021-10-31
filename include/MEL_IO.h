#ifndef _MEL_IO_H
#define _MEL_IO_H

#include "MEL_opengl.h"

#define GLColor32(col) ((float)col/255)

size_t MEL_strlen(const char *);
char * MEL_strlcpy(char *, char *, size_t);
char * MEL_strlcat(char *, char *, size_t);

#endif
